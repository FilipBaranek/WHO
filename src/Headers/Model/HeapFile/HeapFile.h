#pragma once
#define NOMINMAX
#ifdef _WIN32
	#include <Windows.h>
#elif __linux__
	#include <fcntl.h>
	#include <unistd.h> 
#endif
#include <memory>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <fstream>
#include "Block.h"
#include "Helpers/ByteConverter.h"
#include "../HashFile/HashBlock.h"
#include "../Factories/RecordFactory.h"

template<typename T>
class HeapFile
{
private:
	std::list<int> m_partiallyEmptyAddresses;

protected:
	static constexpr const char* FILE_SUFFIX = ".bin";
	static constexpr const char* HEADER_SUFFIX = "_header.bin";

	std::string m_filePath;
	std::fstream m_file;
	std::list<int> m_emptyAddresses;
	int m_clusterSize;
	int m_objectSize;
	bool m_callBaseDestructor;

protected:
	void loadBlock(int& address, uint8_t* buffer, Block<T>* block)
	{
		m_file.seekg(address * block->getSize());
		m_file.read(reinterpret_cast<char*>(buffer), block->getSize());
		block->fromBytes(buffer);
	}

	void writeBlock(int& address, uint8_t* buffer, Block<T>* block)
	{
		block->toBytes(buffer);
		m_file.seekp(address * block->getSize());
		m_file.write(reinterpret_cast<char*>(buffer), block->getSize());
		m_file.flush();
	}

	virtual std::unique_ptr<Block<T>> getBlock()
	{
		return std::make_unique<Block<T>>(m_clusterSize, m_objectSize);
	}

	void readAddresses(std::fstream& headerFile, std::list<int>& addresses)
	{
		int size;
		uint8_t* addressIndex;
		uint8_t sizeBuffer[sizeof(int)];

		headerFile.read(reinterpret_cast<char*>(sizeBuffer), sizeof(int));
		size = ByteConverter::fromByteToPrimitive<int>(sizeBuffer);

		std::vector<uint8_t> addressesBuffer(size * sizeof(int));
		addressIndex = addressesBuffer.data();
		headerFile.read(reinterpret_cast<char*>(addressesBuffer.data()), size * sizeof(int));
		for (int i{}; i < size; ++i)
		{
			addresses.push_back(ByteConverter::fromByteToPrimitive<int>(addressIndex));
			addressIndex += sizeof(int);
		}
	}

	void writeAddresses(uint8_t* index, std::list<int>& addresses)
	{
		index = ByteConverter::toByteFromPrimitive<int>(static_cast<int>(addresses.size()), index);
		for (int address : addresses)
		{
			index = ByteConverter::toByteFromPrimitive<int>(address, index);
		}
	}

#ifdef _WIN32
	void truncateWindowsFile(std::string& filePath, int64_t& newSize, int& sizeBefore)
	{
		HANDLE hFile = CreateFileA(
			filePath.c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			return;
		}

		LARGE_INTEGER li;
		li.QuadPart = newSize;
		if (!SetFilePointerEx(hFile, li, nullptr, FILE_BEGIN))
		{
			CloseHandle(hFile);
			return;
		}

		if (!SetEndOfFile(hFile))
		{
			CloseHandle(hFile);
			return;
		}

		CloseHandle(hFile);
	}
#elif __linux__
	void truncateLinuxFile(std::string& filePath, int64_t& newSize)
	{
		int fd = open(filePath.c_str(), O_WRONLY);
		if (fd == -1)
		{
			throw std::runtime_error("Failed to open file");
		}
		
		if (ftruncate(fd, newSize) != 0)
		{
			close(fd);
			throw std::runtime_error("Failed to truncate file");
		}
		close(fd);
	}
#endif

public:
	HeapFile(std::string filePath, int clusterSize)
	{
		m_clusterSize = clusterSize;
		m_filePath = filePath;
		m_callBaseDestructor = true;

		T* dummy = RecordFactory::createInstance<T>();
		m_objectSize = dummy->getSize();
		delete dummy;
	}

	void truncate()
	{
		int lastBlock = size() - 1;
		int sizeBefore = lastBlock;

		if (m_emptyAddresses.size() > 0 && lastBlock == m_emptyAddresses.back())
		{
			while (lastBlock >= 0 && m_emptyAddresses.size() > 0 && lastBlock == m_emptyAddresses.back())
			{
				m_emptyAddresses.pop_back();
				--lastBlock;
			}

			auto dummy = getBlock();
			int64_t newSize = (lastBlock + 1) * dummy->getSize();
			std::string fileName = m_filePath + FILE_SUFFIX;

			m_file.close();
#ifdef _WIN32
			truncateWindowsFile(fileName, newSize, sizeBefore);
#elif __linux__
			truncateLinuxFile(fileName, newSize);
#else
			throw std::runtime_error("Unsupported OS");
#endif
			m_file.open(m_filePath + FILE_SUFFIX, std::ios::in | std::ios::out | std::ios::binary);
		}
	}

	void open()
	{
		m_file.open(m_filePath + FILE_SUFFIX, std::ios::in | std::ios::out | std::ios::binary);
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

		if (m_emptyAddresses.size() > 0)
		{
			truncate();
		}
		writeHeader();
		m_file.close();
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

		auto dummy = getBlock();
		int size = static_cast<int>(pos) / dummy->getSize();

		return size;
	}

	virtual int headerSize()
	{
		return (2 * sizeof(int)) + (m_partiallyEmptyAddresses.size() * sizeof(int)) + (m_emptyAddresses.size() * sizeof(int));
	}

	virtual void writeHeader()
	{
		std::fstream headerFile(m_filePath + HEADER_SUFFIX, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!headerFile.is_open())
		{
			throw std::runtime_error("Failed to open header file");
		}

		int dataSize = headerSize();
		std::vector<uint8_t> buffer(dataSize);
		uint8_t* index = buffer.data();
		
		writeAddresses(index, m_partiallyEmptyAddresses);
		writeAddresses(index, m_emptyAddresses);

		headerFile.write(reinterpret_cast<char*>(buffer.data()), dataSize);
		headerFile.close();
	}

	virtual void readHeader()
	{
		std::fstream headerFile(m_filePath + HEADER_SUFFIX, std::ios::in | std::ios::binary);
		if (!headerFile.is_open())
		{
			throw std::runtime_error("Failed to open header file");
		}

		readAddresses(headerFile, m_partiallyEmptyAddresses);
		readAddresses(headerFile, m_emptyAddresses);

		headerFile.close();
	}

	std::unique_ptr<HashBlock<T>> block()
	{
		auto newBlock = getBlock();
		Block<T>* rawBlockPtr = newBlock.release();
		HashBlock<T>* rawHashBlockPtr = dynamic_cast<HashBlock<T>*>(rawBlockPtr);
		std::unique_ptr<HashBlock<T>> hashBlock(rawHashBlockPtr);

		return hashBlock;
	}

	std::unique_ptr<HashBlock<T>> blockAt(int address)
	{
		std::unique_ptr<HashBlock<T>> hashBlock = block();
		std::vector<uint8_t> buffer(hashBlock->getSize());
		loadBlock(address, buffer.data(), hashBlock.get());

		return hashBlock;
	}

	void writeAt(int address, HashBlock<T>* block)
	{
		std::vector<uint8_t> buffer(block->getSize());
		writeBlock(address, buffer.data(), block);
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
			m_emptyAddresses.push_back(address);
			newBlock = true;
		}

		auto block = getBlock();
		std::vector<uint8_t> buffer(block->getSize());
		
		if (!newBlock)
		{
			loadBlock(address, buffer.data(), block.get());
		}

		bool inserted = block->insert(object);
		writeBlock(address, buffer.data(), block.get());

		if (newBlock && inserted)
		{
			m_emptyAddresses.pop_front();
			m_partiallyEmptyAddresses.push_back(address);
		}
		if (inserted && block->isFull())
		{
			m_partiallyEmptyAddresses.pop_front();
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

		auto block = getBlock();
		std::vector<uint8_t> buffer(block->getSize());

		loadBlock(address, buffer.data(), block.get());

		return block->find(key);
	}

	T* remove(int address, T* key)
	{
		int fileSize = size();
		if (address < 0 || address >= fileSize || fileSize == 0)
		{
			return nullptr;
		}

		auto block = getBlock();
		std::vector<uint8_t> buffer(block->getSize());

		loadBlock(address, buffer.data(), block.get());

		T* removedObject = block->remove(key);
		if (removedObject != nullptr)
		{
			writeBlock(address, buffer.data(), block.get());
		}

		if (removedObject != nullptr && block->isEmpty())
		{
			m_partiallyEmptyAddresses.remove(address);
			auto iterator = m_emptyAddresses.begin();
			while (iterator != m_emptyAddresses.end() && *iterator < address + 1)
			{
				++iterator;
			}
			m_emptyAddresses.insert(iterator, address);

			if (m_emptyAddresses.size() > 0 && address == m_emptyAddresses.back())
			{
				truncate();
			}
		}
		else if (removedObject != nullptr && !block->isEmpty() &&
				 std::find(m_partiallyEmptyAddresses.begin(), m_partiallyEmptyAddresses.end(), address) == m_partiallyEmptyAddresses.end())
		{
			auto iterator = m_partiallyEmptyAddresses.begin();
			while (iterator != m_partiallyEmptyAddresses.end() && *iterator < address + 1)
			{
				++iterator;
			}
			m_partiallyEmptyAddresses.insert(iterator, address);
		}

		return removedObject;
	}

	std::string printFile()
	{
		std::ostringstream oss;
		int fileSize = size();

		for (int i{}; i < fileSize; ++i)
		{
			if (std::find_if(m_emptyAddresses.begin(), m_emptyAddresses.end(), [&i](int address){ return i == address; }) != m_emptyAddresses.end())
			{
				continue;
			}
			auto block = getBlock();
			std::vector<uint8_t> buffer(block->getSize());

			loadBlock(i, buffer.data(), block.get());

			oss << "\nBlock[" << i << "]\n" << block->toString() << "\n";
		}

		return oss.str();
	}

	std::string printAddresses()
	{
		std::ostringstream oss;

		oss << "Partially empty addresses:\n";
		for (int address : m_partiallyEmptyAddresses)
		{
			oss << address << " ";
		}
		oss << "\n\nEmpty addresses:\n";
		for (int address : m_emptyAddresses)
		{
			oss << address << " ";
		}

		return oss.str();
	}

	int blockingFactor()
	{
		auto block = getBlock();
		return block->blockingFactor();
	}

	void testSize()
	{
		auto lastBlock = getBlock();
		std::vector<uint8_t> buffer(lastBlock->getSize());
		int lastAddress = size() - 1;

		if (lastAddress >= 0)
		{
			loadBlock(lastAddress, buffer.data(), lastBlock.get());
			if (lastBlock->validBlocks() == 0)
			{
				throw std::runtime_error("Invalid size - last empty block must be truncate");
			}
		}
	}

	virtual void clear()
	{
		if (m_file.is_open())
		{
			m_file.close();
		}
		m_file.open(m_filePath + FILE_SUFFIX, std::ios::out | std::ios::binary | std::ios::trunc);
		m_emptyAddresses.clear();
		m_partiallyEmptyAddresses.clear();
	}

	virtual ~HeapFile()
	{
		if (m_callBaseDestructor)
		{
			if (m_file.is_open())
			{
				close();
			}
		}
	}
};