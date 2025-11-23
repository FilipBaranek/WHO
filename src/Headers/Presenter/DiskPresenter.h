#pragma once
#include "Presenter.h"
#include "Commands/GenerateCommand.h"
#include "../../Headers/Model/PCRDatabase/DiskDatabase.h"


class DiskPresenter : public Presenter
{
private:
	DiskDatabase* m_database;
	GenerateCommand m_generateCommand;

public:
	DiskPresenter() : m_database(new DiskDatabase()), m_generateCommand(m_database) {}

	void generatePeople(int count) override;

	void generateTests(int count) override;

	void insert(unsigned int testId, unsigned int workplaceId, unsigned int districtId,
				unsigned int regionId, bool result, double testValue, std::string note,
				time_point<system_clock> testDate, std::string birthNumber) override;

	void insert(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay) override;

	void printAllData() override;

	~DiskPresenter() { delete m_database; };
};