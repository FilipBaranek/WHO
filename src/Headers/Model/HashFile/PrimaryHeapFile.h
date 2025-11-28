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

	void split(int splitAddressPointer, int groupSize, int level, std::function<int(T*)> hash)
	{
		int newAddress = splitAddressPointer + (groupSize * static_cast<int>(std::pow(2, level)));
		auto newBlock = addBlock(newAddress);
		std::vector<uint8_t> newBlockBuffer(newBlock->getSize());

		auto splitBlock = getBlock();
		std::vector<uint8_t> splitBlockBuffer(splitBlock->getSize());
		this->loadBlock(splitAddressPointer, splitBlockBuffer.data(), splitBlock.get());

		T** records = splitBlock->objects();
		for (int i = splitBlock->validBlocks() - 1; i >=0 ; --i)
		{
			T* record = records[i];
			if (hash(record) != splitAddressPointer)
			{
				splitBlock->remove(record);
				newBlock->insert(record);
			}
		}
		this->writeBlock(newAddress, newBlockBuffer.data(), newBlock.get());
		this->writeBlock(splitAddressPointer, splitBlockBuffer.data(), splitBlock.get());
	}

	bool insert(int address, T* record, int& nextBlock, bool newBlock = false)
	{
		auto block = getBlock();
		std::vector<uint8_t> buffer(block->getSize());
		
		if (!newBlock)
		{
			this->loadBlock(address, buffer.data(), block.get());
		}

		bool inserted = block->insert(record);
		this->writeBlock(address, buffer.data(), block.get());

		nextBlock = static_cast<HashBlock<T>*>(block.get())->nextBlock();

		return inserted;
	}
};