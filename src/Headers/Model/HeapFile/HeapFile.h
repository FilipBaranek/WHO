#pragma once
#include <list>
#include <string>
#include <fstream>
#include <vector>
#include "Block.h"
#include "Helpers/ByteConverter.h"
#include "../Factories/RecordFactory.h"

template<typename T>
class HeapFile
{
private:
	static constexpr const char* FILE_NAME = "data.bin";
	static constexpr const char* HEADER_FILE_NAME = "header.bin";

	int m_clusterSize;
	std::string m_filePath;
	std::fstream m_file;
	std::list<int> m_partiallyEmptyAddresses;
	std::list<int> m_emptyAddresses;

public:
	HeapFile(std::string filePath, int clusterSize)
	{
		m_clusterSize = clusterSize;
		m_filePath = filePath;
	}

	void open()
	{
		m_file.open(m_filePath + FILE_NAME, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
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
		m_file.seekg(0, std::ios::end);

		return m_file.tellg();
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

		headerFile.write(buffer.data(), dataSize);
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
	
	bool insert(int address, T* object)
	{
		return false;
	}

	T* find(int address, T* key)
	{
		return nullptr;
	}

	T* remove(int address, T* key)
	{
		return nullptr;
	}

	~HeapFile()
	{
		m_file.close();
	}
};