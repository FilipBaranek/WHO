#pragma once
#include <iostream>
#include <list>
#include <algorithm>
#include <cstdint>
#include "Helpers/ByteConverter.h"
#include "../Interfaces/IRecord.h"
#include "../Factories/RecordFactory.h"

template<typename T>
class Block
{
	static_assert(std::is_base_of_v<IRecord, T>, "T must inherit from IRecord");

private:
	int m_clusterSize;
	int m_blockingFactor;
	std::list<T*> m_data;

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

		m_blockingFactor = m_clusterSize / objectSize;
	}

	bool insert(T* object)
	{
		if (m_data.size() < m_blockingFactor)
		{
			m_data.push_back(object);
			return true;
		}
		return false;
	}

	T* find(T* object)
	{
		auto it = std::find_if(m_data.begin(), m_data.end(), [&object](T* data) {
			return object->is(data);
		});

		if (it == m_data.end())
		{
			return nullptr;
		}
		return *it;
	}

	T* remove(T* object)
	{
		auto it = std::find_if(m_data.begin(), m_data.end(), [&object](T* data) {
			return object->is(data);
		});

		if (it == m_data.end())
		{
			return nullptr;
		}

		T* deletedObject = *it;
		m_data.erase(it);
		
		return deletedObject;
	}

	void toBytes(uint8_t* outputBuffer)
	{
		if (validByteCount() == 0)
		{
			return;
		}

		int validCount = validByteCount();
		uint8_t* index = outputBuffer;
		ByteConverter::toByteFromPrimitive<int>(validCount, index);
		index += sizeof(validCount);
		
		for (auto& object : m_data)
		{
			object->toBytes(index);
			index += object->getSize();
		}
	}

	void fromBytes(uint8_t* buffer)
	{
		if (m_data.size() > 0)
		{
			clear();
		}

		uint8_t* index = buffer;
		int validCount = ByteConverter::fromByteToPrimitive<int>(index);
		index += sizeof(validCount);
		
		for (int i{}; i < validCount; ++i)
		{
			m_data.push_back(RecordFactory::createInstance<T>(index));
			index += m_data.back()->getSize();
		}
	}

	inline std::list<T*>& objects() { return m_data; }

	inline int validByteCount() { return m_data.size(); }

	inline int getSize() { return m_clusterSize; }

	void clear()
	{
		for (auto& object : m_data)
		{
			delete object;
		}
		m_data.clear();
	}

	~Block()
	{
		clear();
	}
};