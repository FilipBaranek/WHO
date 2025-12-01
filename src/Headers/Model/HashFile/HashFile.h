#pragma once
#include <iostream>
#include "Hash.h"
#include "PrimaryHeapFile.h"
#include "OverFlowHeapFile.h"
#include "../Factories/RecordFactory.h"


template<typename T>
class HashFile
{
	static_assert(std::is_base_of_v<IRecord, T>, "T must inherit from IRecord");

private:
	static constexpr const char* PRIMARY_FILE_PATH = "../../../data/HashFile/primary";
	static constexpr const char* OVERFLOW_FILE_PATH = "../../../data/HashFile/overflow";
	static constexpr const int GROUP_SIZE = 4;
	static constexpr const float MAX_DENSITY = 0.8;
	static constexpr const float MIN_DENSITY = 0.64;

	PrimaryHeapFile<T> m_primaryFile;
	OverflowHeapFile<T> m_overFlowFile;

	int m_level;
	int m_splitPointer;
	int m_capacity;
	int m_recordCount;

private:
	int address(T* record)
	{
		uint32_t hashValue = record->hash();
		int addr = hashValue % (GROUP_SIZE * (1 << m_level));

		if (addr < m_splitPointer)
		{
			addr = hashValue % (GROUP_SIZE * (1 << (m_level + 1)));
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
		m_capacity = GROUP_SIZE * m_primaryFile.blockingFactor();
	}

	void close()
	{
		m_primaryFile.close();
		m_overFlowFile.close();
	}

	void insert(T* record)
	{
		int addr = address(record);
		int possibleOverflowAddress = m_overFlowFile.nextAddress();
		int nextBlock;
		bool newBlock = false;

		bool inserted = m_primaryFile.insert(addr, record, nextBlock, newBlock, possibleOverflowAddress);
		if (!inserted)
		{
			m_overFlowFile.insert(nextBlock, record, newBlock);
			if (newBlock)
			{
				m_capacity += m_overFlowFile.blockingFactor();
			}
		}
		++m_recordCount;

		double density = static_cast<double>(m_recordCount) / static_cast<double>(m_capacity);
		while (m_capacity > 0 && density > MAX_DENSITY)
		{
			m_primaryFile.split(m_splitPointer, GROUP_SIZE, m_level,
				[this](T* record) {
					int hashValue = record->hash();
					return hashValue % (GROUP_SIZE * (static_cast<int>(std::pow(2, m_level + 1))));
				}, [this](int address) {
					return m_overFlowFile.blockAt(address);
				}, [this](int address, HashBlock<T>* block) {
					return m_overFlowFile.writeAt(address, block);
				}, [this](int address) {
					return m_overFlowFile.addEmptyAddress(address);
			});

			int removedBlocks = m_overFlowFile.truncate();
			m_capacity -= removedBlocks * m_overFlowFile.blockingFactor();

			++m_splitPointer;
			if (m_splitPointer >= GROUP_SIZE * (static_cast<int>(std::pow(2, m_level))))
			{
				m_splitPointer = 0;
				++m_level;
			}
			m_capacity += m_primaryFile.blockingFactor();

			density = static_cast<double>(m_recordCount) / static_cast<double>(m_capacity);
		}
	}

	T* find(T* key)
	{
		int addr = address(key);
		return m_primaryFile.find(addr, key,
			[this](int address) {
				return m_overFlowFile.blockAt(address);
		});
	}

	void printOut()
	{
		std::cout << "PRIMARY FILE:\n";
		std::cout << m_primaryFile.printFile();
		std::cout << "\nOVERFLOW FILE:\n";
		std::cout << m_overFlowFile.printFile();
	}

	void clear()
	{
		m_primaryFile.clear();
		m_overFlowFile.clear();
	}

	~HashFile()
	{
		close();
	}
};