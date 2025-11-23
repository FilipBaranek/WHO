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
	static constexpr const int REPLICATIONS = 1'000'000;
	static constexpr const int CHECKPOINT = 50'000;
	
	std::random_device m_rd;
	std::mt19937 m_gen{ 456 };

	std::unordered_map<Person*, int> m_data;
	HeapFile<Person> m_heapFile;

public:
	FileTester(std::string filePath, int clusterSize, int dataCount = 0);

	inline void testSize() { m_heapFile.testSize(); }

	void insert();

	void find();
	
	void remove();

	void duplicityTest();

	void printOut(int& inserts, int& deletes);
	
	void runTests();
	
	~FileTester();
};