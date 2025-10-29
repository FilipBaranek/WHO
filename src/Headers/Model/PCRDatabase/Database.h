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
	std::ostringstream m_oss;
	AVLTree<TestWrapper*> m_tests;

public:
	Database() = default;
	void generateRandomData(int dataCount);
	void insert();
	void clear();
	std::string findTestResultByIdAndPatientId();
	void printAllData();
	void saveToFile(std::string filePath);
	void loadFromFile(std::string filePath);
	~Database();
};