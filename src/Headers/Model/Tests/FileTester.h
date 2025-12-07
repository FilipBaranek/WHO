#pragma once
#include <random>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "../HeapFile/HeapFile.h"
#include "../PCRDatabase/Generator/RandomDataGenerator.h"


class FileTester
{
private:
	static constexpr const int REPLICATIONS = 100'000;
	static constexpr const int CHECKPOINT = 1'000;
	
	std::random_device m_rd;
	std::mt19937 m_gen{ 456 };

	std::unordered_map<PersonHashWrapper*, int> m_data;
	HeapFile<PersonHashWrapper> m_heapFile;

public:
	FileTester(std::string filePath, int clusterSize, int dataCount = 0);

	inline void testSize() { m_heapFile.testSize(); }

	void insert();

	void find();
	
	void remove();

	void printOut(int& inserts, int& deletes);
	
	void runTests();
	
	~FileTester();
};