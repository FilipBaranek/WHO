#include "../../Headers/Presenter/DiskPresenter.h"


void DiskPresenter::insert(
	unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId,
	bool result, double testValue, std::string note, time_point<system_clock> testDate, std::string birthNumber)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_insertCommand.setParams(
			testId,
			workplaceId,
			districtId,
			regionId,
			result,
			testValue,
			note,
			testDate,
			birthNumber
		);
		m_insertCommand.execute(output, recordCount);
	});
}

void DiskPresenter::findPerson(std::string birthNumber)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPersonCommand.setParams(birthNumber);
		m_findPersonCommand.execute(output, recordCount);
	});
}

void DiskPresenter::insert(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_insertCommand.setParams(birthNumber, firstName, lastName, birthDay);
		m_insertCommand.execute(output, recordCount);
	});
}

void DiskPresenter::findPersonToEdit(std::string birthNumber)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_editPersonCommand.setParams(birthNumber);
		m_editPersonCommand.execute(output, recordCount);
		m_person = m_editPersonCommand.person();
	});
}

void DiskPresenter::editPerson(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_editPersonCommand.setParams(birthNumber, firstName, lastName, birthDay);
		m_editPersonCommand.executeEdit(output, recordCount);
	});
}

void DiskPresenter::findTestToEdit(unsigned int testId)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_editTestCommand.setParams(testId);
		m_editTestCommand.execute(output, recordCount);
		m_test = m_editTestCommand.test();
	});
}

void DiskPresenter::editTest(unsigned int testId, bool result, double testValue, std::string note, time_point<system_clock> testDate)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_editTestCommand.setParams(testId, result, testValue, note, testDate);
		m_editTestCommand.executeEdit(output, recordCount);
	});
}

void DiskPresenter::clear()
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_clearCommand.execute(output, recordCount);
	});
}

