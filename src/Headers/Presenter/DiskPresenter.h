#pragma once
#include "Presenter.h"
#include "Commands/FindPersonCommand.h"
#include "../../Headers/Model/PCRDatabase/DiskDatabase.h"


class DiskPresenter : public Presenter
{
private:
	FindPersonCommand m_findPersonCommand;

public:
	DiskPresenter() : Presenter(new DiskDatabase()), m_findPersonCommand(m_database) {}

	//(1)
	void insert(unsigned int testId, unsigned int workplaceId, unsigned int districtId,
				unsigned int regionId, bool result, double testValue, std::string note,
				time_point<system_clock> testDate, std::string birthNumber) override;

	//(2)
	void findPerson(std::string birthNumber);

	//(4)
	void insert(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay) override;


	~DiskPresenter() { delete m_database; };
};