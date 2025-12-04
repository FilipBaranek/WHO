#pragma once
#include <iostream>
#include "PrimaryHeapFile.h"
#include "OverFlowHeapFile.h"
#include "../Factories/RecordFactory.h"


template<typename T>
class HashFile
{
	static_assert(std::is_base_of_v<IRecord, T>, "T must inherit from IRecord");

private:
	static constexpr const char* PRIMARY_FILE_PATH = "../../../data/HashFile/primary";
	static constexpr const char* OVERFLOW_FILE_PATH = "../../../data/HashFile/overflow";
	static constexpr const int GROUP_SIZE = 2;
	static constexpr const float MAX_DENSITY = 0.8;
	static constexpr const float MIN_DENSITY = 0.64;

	PrimaryHeapFile<T> m_primaryFile;
	OverflowHeapFile<T> m_overFlowFile;

	int m_level;
	int m_splitPointer;
	int m_capacity;
	int m_recordCount;

private:
	int address(T* record)
	{
		size_t hashValue = record->hash();
		int addr = hashValue % (GROUP_SIZE * (static_cast<int>(std::pow(2, m_level))));

		if (addr < m_splitPointer)
		{
			addr = hashValue % (GROUP_SIZE * (static_cast<int>(std::pow(2, m_level + 1))));
		}

		return addr;
	}

	void rearrangeOldBlock(HashBlock<T>* oldBlock, HashBlock<T>* newBlock, std::function<bool(T*)> hasNewAddress)
	{
		T** oldBlockRecords = oldBlock->objects();
		for (int i = oldBlock->validBlocks() - 1; i >= 0; --i)
		{
			if (hasNewAddress(oldBlockRecords[i]))
			{
				newBlock->insert(oldBlockRecords[i]);
				delete oldBlock->remove(oldBlockRecords[i]);
			}
		}
	}

	void insertInto(HashBlock<T>* block, std::vector<std::unique_ptr<T>>& records)
	{
		int space = block->blockingFactor() - block->validBlocks();
		for (int i{}; i < space && records.size() > 0; ++i)
		{
			block->insert(records.back().get());
			records.pop_back();
		}
	}

	int split()
	{
		int newSplitAddress = m_splitPointer + (GROUP_SIZE * static_cast<int>(std::pow(2, m_level)));
		auto hasNewAddress = [this](T* record) {
			size_t hashValue = record->hash();
			return m_splitPointer != hashValue % (GROUP_SIZE * (static_cast<int>(std::pow(2, m_level + 1))));
		};

		std::vector<std::unique_ptr<T>> recordsToOldBlock;
		std::vector<std::unique_ptr<T>> recordsToNewBlock;

		std::unique_ptr<HashBlock<T>> oldBlock = m_primaryFile.blockAt(m_splitPointer);
		std::unique_ptr<HashBlock<T>> newBlock = m_primaryFile.block();
		
		//Rearrange oldBlock
		rearrangeOldBlock(oldBlock.get(), newBlock.get(), hasNewAddress);

		//Rearrange overflowed sequence
		int emptiedBlocks = m_overFlowFile.loadSequence(oldBlock->nextBlock(), recordsToOldBlock, recordsToNewBlock, hasNewAddress);
		std::unique_ptr<HashBlock<T>> prevOldBlock = nullptr, prevNewBlock = nullptr;
		std::unique_ptr<HashBlock<T>> currentBlock = m_overFlowFile.block();
		int prevOldBlockAddress = -1, prevNewBlockAddress = -1;
		int recordToOldIndex = 0, recordToNewIndex = 0;
		oldBlock->nextBlock(-1);

		insertInto(oldBlock.get(), recordsToOldBlock);
		insertInto(newBlock.get(), recordsToNewBlock);


		int oldBlockCount = recordsToOldBlock.size() / currentBlock->blockingFactor();
		int newBlockCount = recordsToNewBlock.size() / currentBlock->blockingFactor();
		if (recordsToOldBlock.size() > 0 && recordsToOldBlock.size() % currentBlock->blockingFactor() != 0)
		{
			++oldBlockCount;
		}
		if (recordsToNewBlock.size() > 0 && recordsToNewBlock.size() % currentBlock->blockingFactor() != 0)
		{
			++newBlockCount;
		}

		int address = -1;
		for (int i{}; i < oldBlockCount; ++i)
		{
			address = m_overFlowFile.nextAddress();
			currentBlock = std::move(m_overFlowFile.block());
			for (int j{}; j < currentBlock->blockingFactor() && recordToOldIndex < recordsToOldBlock.size(); ++j)
			{
				currentBlock->insert(recordsToOldBlock[recordToOldIndex].get());
				++recordToOldIndex;
			}
			if (prevOldBlock.get() == nullptr)
			{
				oldBlock->nextBlock(address);
			}
			else
			{
				prevOldBlock->nextBlock(address);
				m_overFlowFile.writeAt(prevOldBlockAddress, prevOldBlock.get());
			}
			prevOldBlockAddress = address;
			prevOldBlock = std::move(currentBlock);
		}
		if (prevOldBlock.get() != nullptr)
		{
			prevOldBlock->nextBlock(-1);
			m_overFlowFile.writeAt(prevOldBlockAddress, prevOldBlock.get());
		}

		address = -1;
		for (int i{}; i < newBlockCount; ++i)
		{
			address = m_overFlowFile.nextAddress();
			currentBlock = std::move(m_overFlowFile.block());
			for (int j{}; j < currentBlock->blockingFactor() && recordToNewIndex < recordsToNewBlock.size(); ++j)
			{
				currentBlock->insert(recordsToNewBlock[recordToNewIndex].get());
				++recordToNewIndex;
			}
			if (prevNewBlock.get() == nullptr)
			{
				newBlock->nextBlock(address);
			}
			else
			{
				prevNewBlock->nextBlock(address);
				m_overFlowFile.writeAt(prevNewBlockAddress, prevNewBlock.get());
			}
			prevNewBlockAddress = address;
			prevNewBlock = std::move(currentBlock);
		}
		if (prevNewBlock.get() != nullptr)
		{
			prevNewBlock->nextBlock(-1);
			m_overFlowFile.writeAt(prevNewBlockAddress, prevNewBlock.get());
		}

		m_primaryFile.writeAt(m_splitPointer, oldBlock.get());
		m_primaryFile.writeAt(newSplitAddress, newBlock.get());

		int trimmedBlocks = emptiedBlocks - (oldBlockCount + newBlockCount);
		return trimmedBlocks > 0 ? trimmedBlocks : 0;
	}

public:
	HashFile(int clusterSize, int secondClusterSize = clusterSize) :
		m_primaryFile(PRIMARY_FILE_PATH, clusterSize, GROUP_SIZE), m_overFlowFile(OVERFLOW_FILE_PATH, secondClusterSize),
		m_level{}, m_splitPointer{}, m_capacity{}, m_recordCount{}
	{}

	void open()
	{
		m_primaryFile.open();
		m_overFlowFile.open();
		m_capacity = GROUP_SIZE * m_primaryFile.blockingFactor();
	}

	void close()
	{
		m_primaryFile.close();
		m_overFlowFile.close();
	}

	void insert(T* record)
	{
		int addr = address(record);
		int possibleOverflowAddress = m_overFlowFile.nextAddress();
		int nextBlock;
		bool newBlock = false;

		bool inserted = m_primaryFile.insert(addr, record, nextBlock, newBlock, possibleOverflowAddress);
		if (!inserted)
		{
			m_overFlowFile.insert(nextBlock, record, newBlock);
			if (newBlock)
			{
				m_capacity += m_overFlowFile.blockingFactor();
			}
		}
		++m_recordCount;

		double density = static_cast<double>(m_recordCount) / static_cast<double>(m_capacity);
		while (m_capacity > 0 && density > MAX_DENSITY)
		{
			int trimmedBlocks = split();
			m_capacity -= trimmedBlocks * m_overFlowFile.blockingFactor();

			m_overFlowFile.truncate();

			++m_splitPointer;
			if (m_splitPointer >= GROUP_SIZE * (static_cast<int>(std::pow(2, m_level))))
			{
				m_splitPointer = 0;
				++m_level;
			}
			m_capacity += m_primaryFile.blockingFactor();

			density = static_cast<double>(m_recordCount) / static_cast<double>(m_capacity);
		}
	}

	T* find(T* key)
	{
		int addr = address(key);
		T* record = m_primaryFile.find(addr, key);

		if (record == nullptr)
		{
			record = m_overFlowFile.find(addr, key);
		}
		return record;
	}

	void printOut()
	{
		std::cout << "PRIMARY FILE:\n";
		std::cout << m_primaryFile.printFile();
		std::cout << "\nOVERFLOW FILE:\n";
		std::cout << m_overFlowFile.printFile();
	}

	void clear()
	{
		m_primaryFile.clear();
		m_overFlowFile.clear();
	}

	~HashFile()
	{
		close();
	}
};