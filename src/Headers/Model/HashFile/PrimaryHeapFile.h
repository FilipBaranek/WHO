#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "HashBlock.h"
#include "../HeapFile/HeapFile.h"

template<typename T>
class PrimaryHeapFile : public HeapFile<T>
{
private:
	int m_groupSize;

private:
	std::unique_ptr<Block<T>> getBlock() override
	{
		return std::make_unique<HashBlock<T>>(this->m_clusterSize, this->m_objectSize);
	}

	std::unique_ptr<Block<T>> addBlock(int address)
	{
		auto block = getBlock();
		std::vector<uint8_t> buffer(block->getSize());
		this->writeBlock(address, buffer.data(), block.get());

		return std::move(block);
	}

	void rearrange(int& splitAddressPointer, Block<T>* splitBlock, Block<T>* newBlock, std::function<int(T*)>& hash, Block<T>* primaryBlock = nullptr)
	{
		T** records = splitBlock->objects();
		for (int i = splitBlock->validBlocks() - 1; i >= 0; --i)
		{
			T* record = records[i];
			if (hash(record) != splitAddressPointer)
			{
				splitBlock->remove(record);
				newBlock->insert(record);
				delete record;
			}
			else if (primaryBlock != nullptr)
			{
				splitBlock->remove(record);
				primaryBlock->insert(record);
			}
		}
	}

public:
	PrimaryHeapFile(std::string filePath, int clusterSize, int groupSize) : 
		HeapFile<T>(filePath, clusterSize), m_groupSize(groupSize)
	{}

	void open() override
	{
		HeapFile<T>::open();
		for (int i{}; i < m_groupSize; ++i)
		{
			addBlock(i);
		}
	}

	void split(
		int splitAddressPointer, int groupSize, int level,
		std::function<int(T*)> hash, 
		std::function<std::unique_ptr<HashBlock<T>>(int)> nextOverflowedBlock,
		std::function<void(int, HashBlock<T>*)> writeOverflowBlock
	)
	{
		int newAddress = splitAddressPointer + (groupSize * static_cast<int>(std::pow(2, level)));
		auto newBlock = addBlock(newAddress);
		std::vector<uint8_t> newBlockBuffer(newBlock->getSize());

		auto splitBlock = getBlock();
		std::vector<uint8_t> splitBlockBuffer(splitBlock->getSize());
		this->loadBlock(splitAddressPointer, splitBlockBuffer.data(), splitBlock.get());

		rearrange(splitAddressPointer, splitBlock.get(), newBlock.get(), hash);

		int overflowPtr = dynamic_cast<HashBlock<T>*>(splitBlock.get())->nextBlock();
		while (overflowPtr != -1)
		{
			std::unique_ptr<HashBlock<T>> overflowBlock = nextOverflowedBlock(overflowPtr);
			rearrange(splitAddressPointer, overflowBlock.get(), newBlock.get(), hash, splitBlock.get());
			writeOverflowBlock(overflowPtr, overflowBlock.get());
			overflowPtr = overflowBlock->nextBlock();
		}
		this->writeBlock(splitAddressPointer, splitBlockBuffer.data(), splitBlock.get());
		this->writeBlock(newAddress, newBlockBuffer.data(), newBlock.get());
	}

	bool insert(int address, T* record, int& nextBlock, bool& newBlock, int& possibleNextBlock)
	{
		auto block = getBlock();
		HashBlock<T>* hashBlock = dynamic_cast<HashBlock<T>*>(block.get());
		std::vector<uint8_t> buffer(hashBlock->getSize());
		
		this->loadBlock(address, buffer.data(), hashBlock);
		bool inserted = hashBlock->insert(record);
		if (!inserted && hashBlock->nextBlock() == -1)
		{
			hashBlock->nextBlock(possibleNextBlock);
			newBlock = true;
		}
		this->writeBlock(address, buffer.data(), hashBlock);
		nextBlock = hashBlock->nextBlock();

		return inserted;
	}
};