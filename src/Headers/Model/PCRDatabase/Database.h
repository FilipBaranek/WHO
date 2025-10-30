#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "./ModelWrappers/TestWrapper.h"
#include "../Structures/AVL/AVLTree.h"
#include "./Generator/RandomDataGenerator.h"


class Database
{
private:
	static constexpr const int MIN_TEST_ID = -1;
	static constexpr const int MAX_TEST_ID = 99999999999999;
	static constexpr const int DEFAULT_NUM_VAL = 0;
	static constexpr const char* DEFAULT_STRING_VAL = "\0";
	static constexpr const std::chrono::time_point <std::chrono::system_clock> DEFAULT_TIME_POINT{};
	std::ostringstream m_oss;
	AVLTree<TestWrapper*> m_tests;

public:
	Database() = default;
	void generateRandomData(int dataCount);
	void insert();
	void clear();
	std::string findTestResultByIdAndPatientId(const unsigned int testId, const std::string birthBumber);
	void printAllData();
	void saveToFile(std::string filePath);
	void loadFromFile(std::string filePath);
	~Database();
};