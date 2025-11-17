#pragma once
#include "../HeapFile/Block.h"

class BlockFactory
{
public:
	template<typename T>
	static Block<T>* createInstance()
	{

	}

	BlockFactory() = delete;
	~BlockFactory() = delete;
};