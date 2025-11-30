#pragma once
#include <vector>
#include <exception>
#include "../HashFile/HashFile.h"
#include "../PCRDatabase/Models/Person.h"
#include "../PCRDatabase/Generator/RandomDataGenerator.h"

class HashFileTester
{
private:
	static constexpr const int REPLICATIONS = 100;
	static constexpr const int CHECKPOINT = 50'000;

	std::random_device m_rd;
	std::mt19937 m_gen{ m_rd() };

	std::vector<Person*> m_data;
	HashFile<Person> m_hashFile;

public:
	HashFileTester(int primaryFileClusterSize, int overflowFileClusterSize, int pregeneratedDataCount = 0);

	void insert();

	void find();

	void remove();

	void runTests();

	~HashFileTester();
};