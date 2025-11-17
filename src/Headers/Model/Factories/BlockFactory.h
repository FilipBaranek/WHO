#pragma once
#include "../HeapFile/Block.h"

class BlockFactory
{
public:
	template<typename T>
	static Block<T>* createInstance(int clusterSize, int objectSize)
	{
		return Block<T>::createInstance(clusterSize, objectSize);
	}

	BlockFactory() = delete;
	~BlockFactory() = delete;
};