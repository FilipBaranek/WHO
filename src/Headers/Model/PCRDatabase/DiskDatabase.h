#pragma once
#include "Database.h"

class DiskDatabase : public Database
{
public:
	DiskDatabase() = default;

	void generateRandomPeople(int peopleCount) override;

	bool generateRandomTests(int testCount) override;

	std::pair<std::string, int> printAllData() override;

	void clear() override;

	~DiskDatabase() = default;
};