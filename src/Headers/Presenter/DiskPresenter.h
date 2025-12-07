#pragma once
#include "Presenter.h"
#include "Commands/FindPersonCommand.h"
#include "Commands/EditPersonCommand.h"
#include "Commands/EditTestCommand.h"
#include "Commands/ClearCommand.h"
#include "../../Headers/Model/PCRDatabase/DiskDatabase.h"


class DiskPresenter : public Presenter
{
private:
	PersonView* m_person;
	TestView* m_test;

	FindPersonCommand m_findPersonCommand;
	EditPersonCommand m_editPersonCommand;
	EditTestCommand m_editTestCommand;
	ClearCommand m_clearCommand;

public:
	DiskPresenter() : 
		Presenter(new DiskDatabase()), m_findPersonCommand(m_database),
		m_editPersonCommand(m_database), m_editTestCommand(m_database),
		m_clearCommand(m_database), m_person(nullptr), m_test(nullptr)
	{}

	inline void removePerson()
	{
		delete m_person;
		m_person = nullptr;
	}

	inline PersonView* person() { return m_person; }

	inline void removeTest()
	{
		delete m_test;
		m_test = nullptr;
	}

	inline TestView* test() { return m_test; }

	//(1)
	void insert(unsigned int testId, unsigned int workplaceId, unsigned int districtId,
				unsigned int regionId, bool result, double testValue, std::string note,
				time_point<system_clock> testDate, std::string birthNumber) override;

	//(2)
	void findPerson(std::string birthNumber);

	//(4)
	void insert(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay) override;

	//(7)
	void findPersonToEdit(std::string birthBumber);

	void editPerson(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay);

	//(8)
	void findTestToEdit(unsigned int testId);

	void editTest(unsigned int testId, bool result, double testValue, std::string note, time_point<system_clock> testDate);

	void clear();


	~DiskPresenter() { delete m_database; };
};