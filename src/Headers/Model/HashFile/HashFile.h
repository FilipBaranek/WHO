#pragma once
#include "Hash.h"
#include "PrimaryHeapFile.h"
#include "../Factories/RecordFactory.h"

template<typename T>
class HashFile
{
	static_assert(std::is_base_of_v<IRecord, T>, "T must inherit from IRecord");

private:
	static constexpr const char* PRIMARY_FILE_PATH = "../../../data/HashFile/primary";
	static constexpr const char* OVERFLOW_FILE_PATH = "../../../data/HashFile/overflow";
	static constexpr const int GROUP_SIZE = 2;
	static constexpr const float MAX_DENSITY = 0.8;
	static constexpr const float MIN_DENSITY = 0.64;

	PrimaryHeapFile<T> m_primaryFile;
	HeapFile<T> m_overFlowFile;

	int m_level;
	int m_splitPointer;
	int m_capacity;
	int m_recordCount;

private:
	int address(T* record)
	{
		int hashValue = record->hash();
		int addr = hashValue % (GROUP_SIZE * (static_cast<int>(std::pow(2, m_level))));

		if (addr < m_splitPointer)
		{
			addr = hashValue % (GROUP_SIZE * (static_cast<int>(std::pow(2, m_level + 1))));
		}
		return addr;
	}

public:
	HashFile(int clusterSize, int secondClusterSize = clusterSize) :
		m_primaryFile(PRIMARY_FILE_PATH, clusterSize, GROUP_SIZE), m_overFlowFile(OVERFLOW_FILE_PATH, secondClusterSize),
		m_level{}, m_splitPointer{}, m_capacity{}, m_recordCount{}
	{}

	void open()
	{
		m_primaryFile.open();
		m_overFlowFile.open();
	}

	void close()
	{
		m_primaryFile.close();
		m_overFlowFile.close();
	}

	void insert(T* record)
	{
		int addr = address(record);
		int nextBlock;

		bool inserted = m_primaryFile.insert(addr, record, nextBlock);

		double density = static_cast<double>(m_recordCount) / static_cast<double>(m_capacity);
		while (m_capacity > 0 && density > MAX_DENSITY)
		{
			m_primaryFile.split(m_splitPointer, GROUP_SIZE, m_level, [this](T* record) {
				int hashValue = record->hash();
				return hashValue % (GROUP_SIZE * (static_cast<int>(std::pow(2, m_level + 1))));
			});
			
			++m_splitPointer;
			if (m_splitPointer >= GROUP_SIZE * (static_cast<int>(std::pow(2, m_level))))
			{
				m_splitPointer = 0;
				++m_level;
			}

			T* dummy = RecordFactory::createInstance<T>();
			m_capacity += dummy->getSize();
			delete dummy;

			density = static_cast<double>(m_recordCount) / static_cast<double>(m_capacity);
		}

		if (inserted)
		{
			
			++m_recordCount;
		}
		else
		{
			//TODO - INSERT TO OVERFLOW BUFF
		}
	}

	~HashFile()
	{
		close();
	}
};