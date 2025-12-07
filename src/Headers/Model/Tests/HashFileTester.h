#pragma once
#include <vector>
#include <exception>
#include "../HashFile/HashFile.h"
#include "../PCRDatabase/ModelWrappers/PersonHashWrapper.h"
#include "../PCRDatabase/Generator/RandomDataGenerator.h"

class HashFileTester
{
private:
	static constexpr const int REPLICATIONS = 100'000;
	static constexpr const int CHECKPOINT = 1'000;

	int seed;
	std::random_device m_rd;
	std::mt19937 m_gen;

	std::vector<PersonHashWrapper*> m_data;
	HashFile<PersonHashWrapper> m_hashFile;

public:
	HashFileTester(std::string filePath, int initialGroupSize, int primaryFileClusterSize, int overflowFileClusterSize, int pregeneratedDataCount = 0);

	void insert(int operation);

	void find(int operation);

	void execute(int operation);

	void runTests();

	~HashFileTester();
};