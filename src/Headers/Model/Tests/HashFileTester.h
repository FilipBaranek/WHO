#pragma once
#include <vector>
#include <exception>
#include "../HashFile/HashFile.h"
#include "../PCRDatabase/Models/Person.h"
#include "../PCRDatabase/Generator/RandomDataGenerator.h"

class HashFileTester
{
private:
	static constexpr const int REPLICATIONS = 500'000;
	static constexpr const int CHECKPOINT = 1'000;

	std::random_device m_rd;

	int seed;

	std::mt19937 m_gen;

	std::vector<Person*> m_data;
	HashFile<Person> m_hashFile;

public:
	HashFileTester(int primaryFileClusterSize, int overflowFileClusterSize, int pregeneratedDataCount = 0);

	void insert(int operation);

	void find(int operation);

	void runTests();

	~HashFileTester();
};