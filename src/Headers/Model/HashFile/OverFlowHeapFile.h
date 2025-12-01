#pragma once
#include <string>
#include <vector>
#include <memory>
#include "HashBlock.h"
#include "../HeapFile/HeapFile.h"

template<typename T>
class OverflowHeapFile : public HeapFile<T>
{
private:
	std::unique_ptr<Block<T>> getBlock() override
	{
		return std::make_unique<HashBlock<T>>(this->m_clusterSize, this->m_objectSize);
	}

public:
	OverflowHeapFile(std::string filePath, int clusterSize) : 
		HeapFile<T>(filePath, clusterSize)
	{}

	int truncate()
	{
		int lastBlock = this->size() - 1;
		auto block = getBlock();
		std::vector<uint8_t> buffer(block->getSize());
		this->loadBlock(lastBlock, buffer.data(), block.get());

		int sizeBefore = lastBlock + 1;
		int64_t newSize = sizeBefore;

		if (lastBlock > 0 && block->isEmpty())
		{
			std::string fileName = this->m_filePath + this->FILE_SUFFIX;
			newSize = lastBlock;

			while (lastBlock >= 0 && block->isEmpty())
			{
				--newSize;
				--lastBlock;
				block->clear();
				this->loadBlock(lastBlock, buffer.data(), block.get());
			}

			newSize = (newSize + 1) * block->getSize();

			this->m_file.close();
#ifdef _WIN32
			this->truncateWindowsFile(fileName, newSize, sizeBefore);
#elif __linux__
			this->truncateLinuxFile(fileName, newSize);
#else
			throw std::runtime_error("Unsupported OS");
#endif
			this->m_file.open(this->m_filePath + this->FILE_SUFFIX, std::ios::in | std::ios::out | std::ios::binary);
		}

		return sizeBefore - this->size();
	}

	std::unique_ptr<HashBlock<T>> blockAt(int address)
	{
		auto block = getBlock();
		Block<T>* rawBlockPtr = block.release();
		HashBlock<T>* rawHashBlockPtr = dynamic_cast<HashBlock<T>*>(rawBlockPtr);
		std::unique_ptr<HashBlock<T>> hashBlock(rawHashBlockPtr);
		std::vector<uint8_t> buffer(hashBlock->getSize());
		this->loadBlock(address, buffer.data(), hashBlock.get());
		
		return hashBlock;
	}

	void writeAt(int address, HashBlock<T>* block)
	{
		std::vector<uint8_t> buffer(block->getSize());
		this->writeBlock(address, buffer.data(), block);
	}

	void addEmptyAddress(int address = -1)
	{
		if (address == -1)
		{
			address = nextAddress();
		}

		auto iterator = this->m_emptyAddresses.begin();
		while (iterator != this->m_emptyAddresses.end() && *iterator < address + 1)
		{
			++iterator;
		}
		this->m_emptyAddresses.insert(iterator, address);
	}

	int nextAddress()
	{
		int address;
		if (this->m_emptyAddresses.size() > 0)
		{
			address = this->m_emptyAddresses.front();
			this->m_emptyAddresses.pop_front();
		}
		else
		{
			address = this->size();
		}
		return address;
	}

	void split(std::pair<HashBlock<T>*, HashBlock<T>*>& splitBlocks, std::function<int(T*)> hash)
	{



		
	}

	void insert(int address, T* record, bool& newBlock)
	{
		auto block = getBlock();
		HashBlock<T>* hashBlock = dynamic_cast<HashBlock<T>*>(block.get());
		std::vector<uint8_t> buffer(hashBlock->getSize());

		int addressToInsert = address;
		if (!newBlock)
		{
			this->loadBlock(addressToInsert, buffer.data(), hashBlock);
			while (hashBlock->isFull() && hashBlock->nextBlock() != -1)
			{
				addressToInsert = hashBlock->nextBlock();
				hashBlock->clear();
				this->loadBlock(addressToInsert, buffer.data(), hashBlock);
			}

			if (hashBlock->isFull())
			{
				newBlock = true;
				int newAddress = nextAddress();
				hashBlock->nextBlock(newAddress);
				this->writeBlock(addressToInsert, buffer.data(), hashBlock);

				hashBlock->clear();
				hashBlock->nextBlock(-1);
				addressToInsert = newAddress;
			}
		}

		bool inserted = hashBlock->insert(record);
		if (inserted)
		{
			this->writeBlock(addressToInsert, buffer.data(), hashBlock);
		}
	}

	T* find(int address, T* key)
	{
		int overflowPtr = address;
		T* record = nullptr;

		while (record == nullptr && overflowPtr != -1)
		{
			std::unique_ptr<HashBlock<T>> overflowBlock = blockAt(overflowPtr);
			record = overflowBlock->find(key);
			overflowPtr = overflowBlock->nextBlock();
		}
		return record;
	}
};