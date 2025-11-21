#pragma once
#define NOMINMAX
#include <Windows.h>
#include <string>
#include <list>
#include <vector>
#include <fstream>
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
	std::list<int> m_partiallyEmptyAddresses;
	std::list<int> m_emptyAddresses;

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

	void insertEmptyAddress(int address)
	{
		auto iterator = m_emptyAddresses.begin();
		while (iterator != m_emptyAddresses.end() && *iterator < address)
		{
			++iterator;
		}
		m_emptyAddresses.insert(iterator, address);
	}

	void truncateFile(std::string& filePath, int64_t newSize)
	{
		HANDLE hFile = CreateFileA(
			filePath.c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			std::cerr << "\nFailed to open file\n";
			return;
		}

		LARGE_INTEGER li;
		li.QuadPart = newSize;
		if (!SetFilePointerEx(hFile, li, nullptr, FILE_BEGIN))
		{
			std::cerr << "Failed to move file pointer\n";
			CloseHandle(hFile);
			return;
		}

		if (!SetEndOfFile(hFile))
		{
			std::cerr << "Failed to truncate file\n";
			CloseHandle(hFile);
			return;
		}

		CloseHandle(hFile);
	}


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
	}

	void close()
	{
		if (!m_file.is_open())
		{
			throw std::runtime_error("Trying to close already closed file");
		}

		int totalBlocks = size();
		if (totalBlocks == 0)
		{
			return;
		}

		int lastValidBlock = totalBlocks - 1;
		while (true)
		{
			Block<T> block(m_clusterSize, m_objectSize);
			std::vector<uint8_t> buffer(block.getSize());
			loadBlock(lastValidBlock, buffer.data(), block);
			if (!block.isEmpty())
			{
				break;
			}
			m_emptyAddresses.remove(lastValidBlock);
			--lastValidBlock;
		}

		Block<T> dummy(m_clusterSize, m_objectSize);
		long long newFileSize = static_cast<long long>((lastValidBlock + 1) * dummy.getSize());
		std::string filePath = m_filePath + FILE_NAME;
			
		m_file.close();
		truncateFile(filePath, newFileSize);
		writeHeader();
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
		else
		{
			address = size();
			insertEmptyAddress(address);
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

		if (newBlock && inserted)
		{
			m_emptyAddresses.remove(address);
			m_partiallyEmptyAddresses.push_back(address);
		}
		if (inserted && block.isFull())
		{
			m_partiallyEmptyAddresses.remove(address);
		}
		
		return inserted ? address : -1;
	}

	T* find(int address, T* key)
	{
		int fileSize = size();
		if (address < 0 || address >= fileSize || fileSize == 0)
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
		int fileSize = size();
		if (address < 0 || address >= fileSize || fileSize == 0)
		{
			return nullptr;
		}

		Block<T> block(m_clusterSize, m_objectSize);
		std::vector<uint8_t> buffer(block.getSize());

		loadBlock(address, buffer.data(), block);

		T* removedObject = block.remove(key);
		writeBlock(address, buffer.data(), block);

		if (removedObject != nullptr && block.isEmpty())
		{
			m_partiallyEmptyAddresses.remove(address);
			insertEmptyAddress(address);
		}
		else if (removedObject != nullptr && !block.isEmpty() &&
				 std::find(m_partiallyEmptyAddresses.begin(), m_partiallyEmptyAddresses.end(), address) == m_partiallyEmptyAddresses.end())
		{
			m_partiallyEmptyAddresses.push_back(address);
		}

		return removedObject;
	}

	void printFile()
	{
		int fileSize = size();

		for (int i{}; i < fileSize; ++i)
		{
			Block<T> block(m_clusterSize, m_objectSize);
			std::vector<uint8_t> buffer(block.getSize());

			loadBlock(i, buffer.data(), block);

			std::cout << "Block[" << i << "]\n" << block.toString() << "\n";
		}
	}

	void printAddresses()
	{
		std::cout << "Partially empty addresses:\n";
		for (int address : m_partiallyEmptyAddresses)
		{
			std::cout << address << " ";
		}
		std::cout << "\n\nEmpty addresses:\n";
		for (int address : m_emptyAddresses)
		{
			std::cout << address << " ";
		}
	}

	void clear()
	{
		if (m_file.is_open())
		{
			m_file.close();
		}
		m_file.open(m_filePath + FILE_NAME, std::ios::out | std::ios::binary | std::ios::trunc);
		m_emptyAddresses.clear();
		m_partiallyEmptyAddresses.clear();

		close();
	}

	~HeapFile()
	{
		if (m_file.is_open())
		{
			close();
		}
	}
};