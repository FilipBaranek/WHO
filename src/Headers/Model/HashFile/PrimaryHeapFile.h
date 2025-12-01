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

	void rearrange(
		int& splitAddressPointer, Block<T>* splitBlock, Block<T>* newBlock,
		std::function<int(T*)>& hash, bool& redirect, Block<T>* primaryBlock = nullptr
	)
	{
		T** records = splitBlock->objects();
		for (int i = splitBlock->validBlocks() - 1; i >= 0; --i)
		{
			bool inserted = false;
			T* record = records[i];
			if (hash(record) != splitAddressPointer)
			{
				inserted = newBlock->insert(record);
				if (!inserted)
				{
					redirect = true;
				}
			}
			else if (primaryBlock != nullptr)
			{
				inserted = primaryBlock->insert(record);
			}
			if (inserted)
			{
				splitBlock->remove(record);
				delete record;
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
		std::function<void(int, HashBlock<T>*)> writeOverflowBlock,
		std::function<void(int)> addEmptyAddress
	)
	{
		int newAddress = splitAddressPointer + (groupSize * static_cast<int>(std::pow(2, level)));
		auto newBlock = addBlock(newAddress);
		std::vector<uint8_t> newBlockBuffer(newBlock->getSize());

		auto splitBlock = getBlock();
		std::vector<uint8_t> splitBlockBuffer(splitBlock->getSize());
		this->loadBlock(splitAddressPointer, splitBlockBuffer.data(), splitBlock.get());

		int newBlockLastPtr = -1;
		bool dummy;
		rearrange(splitAddressPointer, splitBlock.get(), newBlock.get(), hash, dummy);

		HashBlock<T>* prevBlock = dynamic_cast<HashBlock<T>*>(splitBlock.get());
		int overflowPtr = prevBlock->nextBlock();
		int prevAddress = splitAddressPointer;
		while (overflowPtr != -1)
		{
			bool skip = false, redirect = false;
			std::unique_ptr<HashBlock<T>> overflowBlock = nextOverflowedBlock(overflowPtr);
			rearrange(splitAddressPointer, overflowBlock.get(), newBlock.get(), hash, redirect, splitBlock.get());

			int next = overflowBlock->nextBlock();
			if (overflowBlock->isEmpty())
			{
				skip = true;
				prevBlock->nextBlock(overflowBlock->nextBlock());
				overflowBlock->nextBlock(-1);

				if (prevBlock != splitBlock.get())
				{
					writeOverflowBlock(prevAddress, prevBlock);
				}
				addEmptyAddress(overflowPtr);
			}
			else if (redirect)
			{
				if (newBlockLastPtr == -1)
				{
					dynamic_cast<HashBlock<T>*>(newBlock.get())->nextBlock(overflowPtr);
				}
				else
				{
					auto newBlockLast = nextOverflowedBlock(newBlockLastPtr);
					newBlockLast->nextBlock(overflowPtr);
					writeOverflowBlock(newBlockLastPtr, newBlockLast.get());
				}
				newBlockLastPtr = overflowPtr;
			}

			writeOverflowBlock(overflowPtr, overflowBlock.get());
			prevAddress = overflowPtr;
			overflowPtr = next;
			if (prevBlock != splitBlock.get() && !skip)
			{
				delete prevBlock;
			}
			if (!skip)
			{
				prevBlock = overflowBlock.release();
			}

		}
		if (prevBlock != splitBlock.get())
		{
			delete prevBlock;
		}
		this->writeBlock(splitAddressPointer, splitBlockBuffer.data(), splitBlock.get());
		this->writeBlock(newAddress, newBlockBuffer.data(), newBlock.get());
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

	T* find(int address, T* key, std::function<std::unique_ptr<HashBlock<T>>(int)> nextOverflowedBlock)
	{
		auto block = getBlock();
		std::vector<uint8_t> buffer(block->getSize());
		this->loadBlock(address, buffer.data(), block.get());

		T* record = block->find(key);
		if (record == nullptr)
		{
			int overflowPtr = dynamic_cast<HashBlock<T>*>(block.get())->nextBlock();
			while (record == nullptr && overflowPtr != -1)
			{
				std::unique_ptr<HashBlock<T>> overflowBlock = nextOverflowedBlock(overflowPtr);
				record = overflowBlock->find(key);
				overflowPtr = overflowBlock->nextBlock();
			}
		}
		return record;
	}
};