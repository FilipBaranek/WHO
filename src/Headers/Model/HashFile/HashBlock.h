#pragma once
#include "../HeapFile/Block.h"

template<typename T>
class HashBlock : public Block<T>
{
private:
	int m_nextBlockAddress;

public:
	HashBlock(int clusterSize, int objectSize) : 
		Block<T>(clusterSize, objectSize), m_nextBlockAddress(-1)
	{}

	inline int nextBlock() { return m_nextBlockAddress; }

	inline void nextBlock(int nextBlockAddress) { m_nextBlockAddress = nextBlockAddress; }

	inline int getSize() override { return sizeof(m_nextBlockAddress) + Block<T>::getSize(); }

	void clear() override
	{
		m_nextBlockAddress = -1;
		Block<T>::clear();
	}

	void toBytes(uint8_t* outputBuffer) override
	{
		uint8_t* index = outputBuffer;
		index = ByteConverter::toByteFromPrimitive<int>(m_nextBlockAddress, index);
		Block<T>::toBytes(index);
	}

	void fromBytes(uint8_t* buffer) override
	{
		uint8_t* index = buffer;
		m_nextBlockAddress = ByteConverter::fromByteToPrimitive<int>(index);
		index += sizeof(m_nextBlockAddress);
		Block<T>::fromBytes(index);
	}
};