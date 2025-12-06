#pragma once
#include <iostream>
#include <sstream>
#include "PrimaryHeapFile.h"
#include "OverFlowHeapFile.h"


template<typename T>
class HashFile
{
	static_assert(std::is_base_of_v<IRecord, T>, "T must inherit from IRecord");

private:
	static constexpr const int GROUP_SIZE = 4;
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

	void tryInsertInto(HashBlock<T>* block, std::vector<std::unique_ptr<T>>& records)
	{
		int space = block->blockingFactor() - block->validBlocks();
		for (int i{}; i < space && records.size() > 0; ++i)
		{
			block->insert(records.back().get());
			records.pop_back();
		}
	}

	int rearrangeOverflowedBlocks(HashBlock<T>* primaryBlock, std::vector<std::unique_ptr<T>>& records)
	{
		std::unique_ptr<HashBlock<T>> currentBlock = m_overFlowFile.block(), prevBlock = nullptr;
		int blockCount = records.size() / currentBlock->blockingFactor();
		int recordIndex = 0;

		if (records.size() > 0 && records.size() % currentBlock->blockingFactor() != 0)
		{
			++blockCount;
		}

		int address = -1, prevAddress = -1;
		for (int i{}; i < blockCount; ++i)
		{
			address = m_overFlowFile.nextAddress();
			currentBlock = std::move(m_overFlowFile.block());
			for (int j{}; j < currentBlock->blockingFactor() && recordIndex < records.size(); ++j)
			{
				currentBlock->insert(records[recordIndex].get());
				++recordIndex;
			}
			if (prevBlock.get() == nullptr)
			{
				primaryBlock->nextBlock(address);
			}
			else
			{
				prevBlock->nextBlock(address);
				m_overFlowFile.writeAt(prevAddress, prevBlock.get());
			}
			prevAddress = address;
			prevBlock = std::move(currentBlock);
		}
		if (prevBlock.get() != nullptr)
		{
			prevBlock->nextBlock(-1);
			m_overFlowFile.writeAt(prevAddress, prevBlock.get());
		}

		return blockCount;
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

		tryInsertInto(oldBlock.get(), recordsToOldBlock);
		tryInsertInto(newBlock.get(), recordsToNewBlock);

		int blockCount = 0;
		blockCount += rearrangeOverflowedBlocks(oldBlock.get(), recordsToOldBlock);
		blockCount += rearrangeOverflowedBlocks(newBlock.get(), recordsToNewBlock);

		m_primaryFile.writeAt(m_splitPointer, oldBlock.get());
		m_primaryFile.writeAt(newSplitAddress, newBlock.get());

		int trimmedBlocks = emptiedBlocks - blockCount;
		return trimmedBlocks > 0 ? trimmedBlocks : 0;
	}

public:
	HashFile(std::string filePath, int clusterSize, int secondClusterSize = clusterSize) :
		m_primaryFile(filePath + "primary", clusterSize, GROUP_SIZE),
		m_overFlowFile(filePath + "overflow", secondClusterSize),
		m_level{}, m_splitPointer{}, m_capacity{}, m_recordCount{}
	{}

	void open()
	{
		std::vector<int*> attributes = { &m_level, &m_splitPointer, &m_capacity, &m_recordCount };

		m_primaryFile.open(attributes);
		m_overFlowFile.open();

		if (m_capacity == 0)
		{
			m_capacity = GROUP_SIZE * m_primaryFile.blockingFactor();
		}
	}

	void close()
	{
		std::vector<int> attributes = { m_level, m_splitPointer, m_capacity, m_recordCount };

		m_primaryFile.close(attributes);
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

	inline int size() { return m_recordCount; }

	std::string printOut()
	{
		std::ostringstream oss;
		oss << "PRIMARY FILE:\n";
		oss << m_primaryFile.printFile();
		oss << "\nOVERFLOW FILE:\n";
		oss << m_overFlowFile.printFile();
		oss << "\n\n";

		return oss.str();
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