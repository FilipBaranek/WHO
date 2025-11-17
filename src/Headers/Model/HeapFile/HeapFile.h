#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "Block.h"
#include "Helpers/ByteConverter.h"
#include "../Factories/RecordFactory.h"
#include "../Factories/BlockFactory.h"

template<typename T>
class HeapFile
{
private:
	static constexpr const char* FILE_NAME = "data.bin";
	static constexpr const char* HEADER_FILE_NAME = "header.bin";

	int m_clusterSize;
	int m_objectSize;
	std::string m_filePath;
	std::fstream m_file;
	std::vector<int> m_partiallyEmptyAddresses;
	std::vector<int> m_emptyAddresses;

public:
	HeapFile(std::string filePath, int clusterSize)
	{
		m_clusterSize = clusterSize;
		m_filePath = filePath;

		T* dummy = RecordFactory::createInstance<T>();
		m_objectSize = dummy->getSize();
		delete dummy;
	}

	void open()
	{
		m_file.open(m_filePath + FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);
		if (!m_file.is_open())
		{
			throw std::runtime_error("Failed to open binary file");
		}

		if (size() > 0)
		{
			readHeader();
		}
		else
		{
			m_emptyAddresses.push_back(0);
		}
	}

	int size()
	{
		m_file.clear();
		m_file.seekg(0, std::ios::end);
		std::streampos pos = m_file.tellg();
		if (pos <= 0)
		{
			return 0;
		}

		auto block = BlockFactory::createInstance<T>(m_clusterSize, m_objectSize);
		int size = static_cast<int>(pos) / block->getSize();
		delete block;

		return size;
	}

	int headerSize()
	{
		return (2 * sizeof(int)) + (m_partiallyEmptyAddresses.size() * sizeof(int)) + (m_emptyAddresses.size() * sizeof(int));
	}

	void writeHeader()
	{
		std::fstream headerFile(m_filePath + HEADER_FILE_NAME, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!headerFile.is_open())
		{
			throw std::runtime_error("Failed to open binary header file");
		}

		int dataSize = headerSize();
		std::vector<uint8_t> buffer(dataSize);
		uint8_t* index = buffer.data();

		index = ByteConverter::toByteFromPrimitive<int>(static_cast<int>(m_partiallyEmptyAddresses.size()), index);
		for (int address : m_partiallyEmptyAddresses)
		{
			index = ByteConverter::toByteFromPrimitive<int>(address, index);
		}
		
		index = ByteConverter::toByteFromPrimitive<int>(static_cast<int>(m_emptyAddresses.size()), index);
		for (int address : m_emptyAddresses)
		{
			index = ByteConverter::toByteFromPrimitive<int>(address, index);
		}

		headerFile.write(reinterpret_cast<char*>(buffer.data()), dataSize);
		headerFile.close();
	}

	void readHeader()
	{
		std::fstream headerFile(m_filePath + HEADER_FILE_NAME, std::ios::in | std::ios::binary);
		if (!headerFile.is_open())
		{
			throw std::runtime_error("Failed to open binary header file");
		}

		int size;
		uint8_t* addressIndex;
		uint8_t sizeBuffer[sizeof(int)];
		
		headerFile.read(reinterpret_cast<char*>(sizeBuffer), sizeof(int));
		size = ByteConverter::fromByteToPrimitive<int>(sizeBuffer);

		std::vector<uint8_t> partiallyEmptyAddressBuffer(size * sizeof(int));
		addressIndex = partiallyEmptyAddressBuffer.data();
		headerFile.read(reinterpret_cast<char*>(partiallyEmptyAddressBuffer.data()), size * sizeof(int));
		for (int i{}; i < size; ++i)
		{
			m_partiallyEmptyAddresses.push_back(ByteConverter::fromByteToPrimitive<int>(addressIndex));
			addressIndex += sizeof(int);
		}

		headerFile.read(reinterpret_cast<char*>(sizeBuffer), sizeof(int));
		size = ByteConverter::fromByteToPrimitive<int>(sizeBuffer);

		std::vector<uint8_t> emptyAddressesBuffer(size * sizeof(int));
		addressIndex = emptyAddressesBuffer.data();
		headerFile.read(reinterpret_cast<char*>(emptyAddressesBuffer.data()), size * sizeof(int));
		for (int i{}; i < size; ++i)
		{
			m_emptyAddresses.push_back(ByteConverter::fromByteToPrimitive<int>(addressIndex));
			addressIndex += sizeof(int);
		}

		headerFile.close();
	}

	void loadBlock(int& address, uint8_t* buffer, Block<T>& block)
	{
		m_file.seekg(address * block.getSize());
		m_file.read(reinterpret_cast<char*>(buffer), block.getSize());
		block.fromBytes(buffer);
	}

	void writeBlock(int& address, uint8_t* buffer, Block<T>& block)
	{
		block.toBytes(buffer);
		m_file.seekp(address * block.getSize());
		m_file.write(reinterpret_cast<char*>(buffer), block.getSize());
		m_file.flush();
	}
	
	int insert(T* object)
	{
		bool newBlock = false;
		int address;
		if (m_partiallyEmptyAddresses.size() > 0)
		{
			address = *m_partiallyEmptyAddresses.begin();
		}
		else if (m_emptyAddresses.size() > 0)
		{
			address = *m_emptyAddresses.begin();
			newBlock = true;
		}

		Block<T> block(m_clusterSize, m_objectSize);
		std::vector<uint8_t> buffer(block.getSize());
		
		if (!newBlock)
		{
			loadBlock(address, buffer.data(), block);
		}

		bool inserted = block.insert(object);
		writeBlock(address, buffer.data(), block);

		if (newBlock && inserted && !block.isFull())
		{
			std::swap(*m_emptyAddresses.begin(), *m_emptyAddresses.rbegin());
			m_emptyAddresses.pop_back();
			m_partiallyEmptyAddresses.push_back(address);
		}
		else if (inserted && block.isFull())
		{
			auto partiallyEmptyIt = std::find(m_partiallyEmptyAddresses.begin(), m_partiallyEmptyAddresses.end(), address);

			if (partiallyEmptyIt != m_partiallyEmptyAddresses.end())
			{
				std::swap(*partiallyEmptyIt, m_partiallyEmptyAddresses.back());
				m_partiallyEmptyAddresses.pop_back();
			}

			Block<T> newBlock(m_clusterSize, m_objectSize);
			std::vector<uint8_t> newBuffer(newBlock.getSize());
			int newAddress = size();
			writeBlock(newAddress, newBuffer.data(), newBlock);
			m_emptyAddresses.push_back(newAddress);
		}
		
		return inserted ? address : -1;
	}

	T* find(int address, T* key)
	{
		if (address < 0 || address >= size())
		{
			return nullptr;
		}

		Block<T> block(m_clusterSize, m_objectSize);
		std::vector<uint8_t> buffer(block.getSize());

		loadBlock(address, buffer.data(), block);

		return block.find(key);
	}

	T* remove(int address, T* key)
	{
		if (address < 0 || address >= size())
		{
			return nullptr;
		}

		Block<T> block(m_clusterSize, m_objectSize);
		std::vector<uint8_t> buffer(block.getSize());

		loadBlock(address, buffer.data(), block);

		T* removedObject = block.remove(key);
		writeBlock(address, buffer.data(), block);

		if (block.isEmpty() && removedObject != nullptr)
		{
			m_emptyAddresses.push_back(address);
		}
		else if (!block.isEmpty() && !block.isFull() && removedObject != nullptr &&
				 std::find(m_partiallyEmptyAddresses.begin(), m_partiallyEmptyAddresses.end(), address) == m_partiallyEmptyAddresses.end())
		{
			m_partiallyEmptyAddresses.push_back(address);
		}

		return removedObject;
	}

	~HeapFile()
	{
		///TIETO IFY MAJU BYT PRE UNIVERZALNU VELKOST - MOZE BYT PRAZDNA/GARBAGE BLOK AJ NA KONCI
		if (m_emptyAddresses.size() == 1 && *m_emptyAddresses.begin() == size() - 1)
		{
			Block<T> block(m_clusterSize, m_objectSize);
			std::vector<uint8_t> buffer(block.getSize());

			int address = 0;
			loadBlock(address, buffer.data(), block);

			if (block.isEmpty())
			{
				m_file.close();
				m_file.open(m_filePath + FILE_NAME, std::ios::out | std::ios::binary | std::ios::trunc);

				std::fstream headerFile(m_filePath + HEADER_FILE_NAME, std::ios::out | std::ios::binary | std::ios::trunc);
				headerFile.close();
			}
		}
		//K TOMUTO SA VRATIT
		writeHeader();

		std::cout << size() << "\n";
		
		m_file.close();
	}
};