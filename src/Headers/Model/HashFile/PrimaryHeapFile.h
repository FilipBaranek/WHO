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

	int headerSize() override
	{
		return 4 * sizeof(int);
	}

protected:
	void readHeader(std::vector<int*>& hashAttributesOutput)
	{
		std::fstream headerFile(this->m_filePath + this->HEADER_SUFFIX, std::ios::in | std::ios::binary);
		if (!headerFile.is_open())
		{
			throw std::runtime_error("Failed to open header file");
		}

		int size = headerSize();
		
		std::vector<uint8_t> buffer(size);
		uint8_t* index = buffer.data();

		headerFile.read(reinterpret_cast<char*>(buffer.data()), size);
		for (int i{}; i < hashAttributesOutput.size(); ++i)
		{
			*hashAttributesOutput[i] = ByteConverter::fromByteToPrimitive<int>(index);
			index += sizeof(int);
		}

		headerFile.close();
	}

	void writeHeader(std::vector<int>& hashAttributes)
	{
		std::fstream headerFile(this->m_filePath + this->HEADER_SUFFIX, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!headerFile.is_open())
		{
			throw std::runtime_error("Failed to open header file");
		}

		int size = headerSize();
		std::vector<uint8_t> buffer(size);
		uint8_t* index = buffer.data();

		for (int attribute : hashAttributes)
		{
			index = ByteConverter::toByteFromPrimitive<int>(attribute, index);
		}

		headerFile.write(reinterpret_cast<char*>(buffer.data()), size);
		headerFile.flush();
		headerFile.close();
	}

public:
	PrimaryHeapFile(std::string filePath, int clusterSize, int groupSize) : 
		HeapFile<T>(filePath, clusterSize), m_groupSize(groupSize)
	{}

	void open(std::vector<int*>& hashAttributesOutput)
	{
		this->m_file.open(this->m_filePath + this->FILE_SUFFIX, std::ios::in | std::ios::out | std::ios::binary);
		if (!this->m_file.is_open())
		{
			throw std::runtime_error("Failed to open binary file");
		}

		if (this->size() == 0)
		{
			for (int i{}; i < m_groupSize; ++i)
			{
				addBlock(i);
			}
		}
		else
		{
			readHeader(hashAttributesOutput);
		}
	}

	void close(std::vector<int>& hashAttributes)
	{
		writeHeader(hashAttributes);
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
			this->writeBlock(address, buffer.data(), hashBlock);
			nextBlock = hashBlock->nextBlock();
		}
		else if (!block->isFull())
		{
			inserted = hashBlock->insert(record);
			if (inserted)
			{
				this->writeBlock(address, buffer.data(), hashBlock);
			}
		}

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

	bool execute(int& address, T* key, std::function<void(T*)> callback)
	{
		auto block = getBlock();
		std::vector<uint8_t> buffer(block->getSize());
		this->loadBlock(address, buffer.data(), block.get());

		bool executed = block->execute(key, callback);
		if (!executed)
		{
			address = dynamic_cast<HashBlock<T>*>(block.get())->nextBlock();
		}
		else
		{
			this->writeBlock(address, buffer.data(), block.get());
		}
		return executed;
	}

	void clear() override
	{
		HeapFile<T>::clear();
		for (int i{}; i < m_groupSize; ++i)
		{
			addBlock(i);
		}
	}

	~PrimaryHeapFile()
	{
		this->m_callBaseDestructor = false;
	}
};