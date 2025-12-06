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

	int headerSize() override
	{
		return sizeof(int) + (this->m_emptyAddresses.size() * sizeof(int));
	}

protected:
	void readHeader() override
	{
		std::fstream headerFile(this->m_filePath + this->HEADER_SUFFIX, std::ios::in | std::ios::binary);
		if (!headerFile.is_open())
		{
			throw std::runtime_error("Failed to open header file");
		}

		this->readAddresses(headerFile, this->m_emptyAddresses);

		headerFile.close();
	}

	void writeHeader() override
	{
		std::fstream headerFile(this->m_filePath + this->HEADER_SUFFIX, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!headerFile.is_open())
		{
			throw std::runtime_error("Failed to open header file");
		}

		int dataSize = headerSize();
		std::vector<uint8_t> buffer(dataSize);
		uint8_t* index = buffer.data();

		this->writeAddresses(index, this->m_emptyAddresses);

		headerFile.write(reinterpret_cast<char*>(buffer.data()), dataSize);
		headerFile.close();
	}

public:
	OverflowHeapFile(std::string filePath, int clusterSize) : 
		HeapFile<T>(filePath, clusterSize)
	{}

	void addEmptyAddress(int address)
	{
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

	int loadSequence(
		int startingAddress, std::vector<std::unique_ptr<T>>& recordsToOldBlock,
		std::vector<std::unique_ptr<T>>& recordsToNewBlock, std::function<bool(T*)> hasNewAddress
	)
	{
		int blockCount = 0;
		int address = startingAddress;
		while (address != -1)
		{
			auto block = getBlock();
			std::vector<uint8_t> buffer(block->getSize());
			this->loadBlock(address, buffer.data(), block.get());
			
			T** records = block->objects();
			for (int i = block->validBlocks() - 1; i >= 0; --i)
			{
				if (hasNewAddress(records[i]))
				{
					recordsToNewBlock.push_back(std::unique_ptr<T>(block->remove(records[i])));
				}
				else
				{
					recordsToOldBlock.push_back(std::unique_ptr<T>(block->remove(records[i])));
				}
			}

			addEmptyAddress(address);
			address = dynamic_cast<HashBlock<T>*>(block.get())->nextBlock();
			++blockCount;
		}
		return blockCount;
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
			std::unique_ptr<HashBlock<T>> overflowBlock = this->blockAt(overflowPtr);
			record = overflowBlock->find(key);
			overflowPtr = overflowBlock->nextBlock();
		}
		return record;
	}

	~OverflowHeapFile()
	{
		this->m_callBaseDestructor = true;
	}
};