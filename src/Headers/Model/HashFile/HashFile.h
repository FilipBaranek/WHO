#pragma once
#include <vector>
#include <type_traits>
#include <string>
#include "../HeapFile/HeapFile.h"
#include "../HeapFile/Helpers/ByteConverter.h"

template<typename T>
class HashFile
{
private:
	static consteval const char* PRIMARY_FILE_PATH = "TODO";///////////////////////////////
	static consteval const char* OVERFLOW_FILE_PATH = "TODO";///////////////////////////////
	static consteval const int32_t FNV_PRIME = 16777619;
	static consteval const int32_t OFFSET = 2166136261;

	HeapFile<T> m_primaryFile;
	HeapFile<T> m_overFlowFile;

public:
	HashFile(int clusterSize, int secondClusterSize = clusterSize) : 
		m_primaryFile(PRIMARY_FILE_PATH, clusterSize), m_overFlowFile(OVERFLOW_FILE_PATH, secondClusterSize) = default;

	void open()
	{
		m_primaryFile.open();
		m_overFlowFile.open();
	}

	template<typename Key>
	int hash(Key& key)
	{
		std::vector<uint8_t> buffer;
		if constexpr (std::is_same<Key, std::string>::value)
		{
			buffer.resize(key.size());
			uint8_t* index = buffer.data();

			for (size_t i{}; i < key.size(); ++i)
			{
				index = ByteConverter::toByteFromPrimitive<char>(key[i], index);
			}
		}
		else
		{
			buffer.resize(sizeof(Key));
			ByteConverter::toByteFromPrimitive<Key>(key, buffer.data());
		}

		int hash = OFFSET;
		for (auto& byte : buffer)
		{
			hash ^= byte;
			hash *= FNV_PRIME;
		}

		return hash;
	}

	void close()
	{
		m_primaryFile.close();
		m_overFlowFile.close();
	}

	~HashFile()
	{
		close();
	}
};