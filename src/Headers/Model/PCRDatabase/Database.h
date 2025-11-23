#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std::chrono;

class Database
{
public:
	Database() = default;
	
	virtual void generateRandomPeople(int peopleCount) = 0;
	
	virtual bool generateRandomTests(int testCount) = 0;
	
	virtual std::pair<std::string, int> printAllData() = 0;

	virtual void clear() = 0;
	
	virtual ~Database() = default;
};