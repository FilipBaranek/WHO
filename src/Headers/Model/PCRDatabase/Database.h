#pragma once
#include <string>
#include <sstream>
#include "./ModelWrappers/TestWrapper.h"
#include "../Structures/AVL/AVLTree.h"


class Database
{
private:
	std::ostringstream m_oss;
	AVLTree<TestWrapper*> m_tests;

public:
	Database();
	void generateRandomData();
	void insert();
	void clear();
	std::string findTestResultByIdAndPatientId();
	void saveToFile(std::string filePath);
	void loadFromFile(std::string filePath);
	~Database();
};