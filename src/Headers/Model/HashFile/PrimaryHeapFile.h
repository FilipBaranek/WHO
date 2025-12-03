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

protected:
	void readHeader() override
	{

	}

	void writeHeader() override
	{

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

	std::unique_ptr<HashBlock<T>> block()
	{
		auto block = getBlock();
		Block<T>* rawBlockPtr = block.release();
		HashBlock<T>* rawHashBlockPtr = dynamic_cast<HashBlock<T>*>(rawBlockPtr);
		std::unique_ptr<HashBlock<T>> hashBlock(rawHashBlockPtr);

		return hashBlock;
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

	bool insert(int address, T* record, int& nextBlock, bool& newBlock, int& possibleNextBlock)
	{
		auto block = getBlock();
		HashBlock<T>* hashBlock = dynamic_cast<HashBlock<T>*>(block.get());
		std::vector<uint8_t> buffer(hashBlock->getSize());
		
		bool inserted = false;
		this->loadBlock(address, buffer.data(), hashBlock);

		if (block->isFull() && hashBlock->nextBlock() != -1)
		{
			newBlock = false;
			nextBlock = hashBlock->nextBlock();
			return inserted;
		}
		else if (block->isFull() && hashBlock->nextBlock() == -1)
		{
			newBlock = true;
			hashBlock->nextBlock(possibleNextBlock);
			nextBlock = hashBlock->nextBlock();
		}
		else if (!block->isFull())
		{
			inserted = hashBlock->insert(record);
		}
		this->writeBlock(address, buffer.data(), hashBlock);

		return inserted;
	}

	T* find(int& address, T* key)
	{
		auto block = getBlock();
		std::vector<uint8_t> buffer(block->getSize());
		this->loadBlock(address, buffer.data(), block.get());

		T* record = block->find(key);
		if (record == nullptr)
		{
			address = dynamic_cast<HashBlock<T>*>(block.get())->nextBlock();
		}
		return record;
	}
};