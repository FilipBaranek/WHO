#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <sstream>
#include "Helpers/ByteConverter.h"
#include "../Interfaces/IRecord.h"
#include "../Factories/RecordFactory.h"

template<typename T>
class Block
{
	static_assert(std::is_base_of_v<IRecord, T>, "T must inherit from IRecord");

private:
	int m_clusterSize;
	int m_validBlockCount;
	int m_blockingFactor;
	T** m_data;

public:
	Block(int clusterSize, int objectSize)
	{
		if (clusterSize >= objectSize)
		{
			m_clusterSize = clusterSize;
		}
		else
		{
			m_clusterSize = objectSize;
			std::cout << "Cluster size was too low, size was adjusted to " << m_clusterSize << " as bare minimum\n";
		}

		m_validBlockCount = 0;
		m_blockingFactor = m_clusterSize / objectSize;
		m_data = new T * [m_blockingFactor];
	}

	bool insert(T* object)
	{
		if (m_validBlockCount < m_blockingFactor)
		{
			m_data[m_validBlockCount] = dynamic_cast<T*>(object->clone());
			++m_validBlockCount;
			return true;
		}
		return false;
	}

	T* find(T* object)
	{
		T* foundObject = nullptr;
		for (int i{}; i < m_validBlockCount; ++i)
		{
			if (m_data[i]->equals(object))
			{
				foundObject = m_data[i];
				break;
			}
			else if (m_data[i]->is(object))
			{
				foundObject = m_data[i];
			}
		}
		return foundObject != nullptr ? dynamic_cast<T*>(foundObject->clone()) : nullptr;
	}

	T* remove(T* object)
	{
		int index = -1;
		for (int i{}; i < m_validBlockCount; ++i)
		{
			if (m_data[i]->equals(object))
			{
				index = i;
				break;
			}
			else if (m_data[i]->is(object))
			{
				index = i;
			}
		}

		if (index != -1)
		{
			if (index != m_validBlockCount - 1)
			{
				std::swap(m_data[index], m_data[m_validBlockCount - 1]);
			}
			T* removedObject = m_data[m_validBlockCount - 1];
			m_data[m_validBlockCount - 1] = nullptr;
			--m_validBlockCount;

			return removedObject;
		}
		return nullptr;
	}

	virtual void toBytes(uint8_t* outputBuffer)
	{
		uint8_t* index = outputBuffer;
		ByteConverter::toByteFromPrimitive<int>(m_validBlockCount, index);
		index += sizeof(m_validBlockCount);

		for (int i{}; i < m_validBlockCount; ++i)
		{
			m_data[i]->toBytes(index);
			index += m_data[i]->getSize();
		}
		T* dummy = RecordFactory::createInstance<T>();
		for (int i = m_validBlockCount; i < m_blockingFactor; ++i)
		{
			dummy->toBytes(index);
			index += dummy->getSize();
		}
		delete dummy;
	}

	virtual void fromBytes(uint8_t* buffer)
	{
		if (m_validBlockCount > 0)
		{
			clear();
		}

		uint8_t* index = buffer;
		m_validBlockCount = ByteConverter::fromByteToPrimitive<int>(index);
		index += sizeof(m_validBlockCount);

		for (int i{}; i < m_validBlockCount; ++i)
		{
			m_data[i] = RecordFactory::createInstance<T>(index);
			index += m_data[i]->getSize();
		}
	}

	virtual inline int getSize() { return sizeof(m_validBlockCount) + m_clusterSize; }

	inline int validBlocks() { return m_validBlockCount; }

	inline int blockingFactor() { return m_blockingFactor; }

	inline bool isFull() { return m_validBlockCount == m_blockingFactor; }

	inline bool isEmpty() { return m_validBlockCount == 0; }

	inline T** objects() { return m_data; }

	std::string toString()
	{
		std::ostringstream oss;
		oss << "Valid data:\n";
		for (int i{}; i < m_validBlockCount; ++i)
		{
			oss << m_data[i]->toString();
		}
		oss << "\nCluster size: " << m_clusterSize << " Valid block count: " << m_validBlockCount << " Blocking factor: " << m_blockingFactor << "\n\n";

		return oss.str();
	}

	static Block<T>* createInstance(int clusterSize, int objectSize)
	{
		return new Block<T>(clusterSize, objectSize);
	}

	virtual void clear()
	{
		for (int i{}; i < m_validBlockCount; ++i)
		{
			delete m_data[i];
			m_data[i] = nullptr;
		}
		m_validBlockCount = 0;
	}

	~Block()
	{
		clear();
		delete[] m_data;
	}
};