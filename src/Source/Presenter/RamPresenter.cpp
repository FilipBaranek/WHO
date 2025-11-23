#include "../../Headers/Presenter/RamPresenter.h"

//(1)
void RamPresenter::insert(
	unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, bool result,
	double testValue, std::string note, time_point<system_clock> testDate, std::string birthNumber)
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

//(2)
void RamPresenter::findResultByPatientAndTestId(unsigned int testId, std::string birthNumber)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_resultCommand.setParams(testId, birthNumber);
		m_resultCommand.execute(output, recordCount);
		});
}

//(3)
void RamPresenter::findPatientTestOrderByDate(std::string birthNumber)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPatientsTestCommand.setParams(birthNumber);
		m_findPatientsTestCommand.execute(output, recordCount);
		});
}

//(4)
void RamPresenter::findPositiveTestsInDistrict(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::BY_DATE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(5)
void RamPresenter::findAllTestsInDistrict(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findAllTestsByLocationIdCommand.setParams(LOCATION::DISTRICT, from, to, id);
		m_findAllTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(6)
void RamPresenter::findPositiveTestsInRegion(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::BY_DATE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::REGION);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(7)
void RamPresenter::findAllTestsInRegion(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findAllTestsByLocationIdCommand.setParams(LOCATION::REGION, from, to, id);
		m_findAllTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(8)
void RamPresenter::findPositiveTests(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::BY_DATE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::COUNTRY);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(9)
void RamPresenter::findAllTests(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findAllTestsByLocationIdCommand.setParams(LOCATION::COUNTRY, from, to);
		m_findAllTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(10)
void RamPresenter::findSickPeopleInDistrict(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(11)
void RamPresenter::findSickPeopleInDistrictOrderByTestValue(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id, true);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(12)
void RamPresenter::findSickPeopleInRegion(const unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::REGION);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(13)
void RamPresenter::findSickPeople(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::COUNTRY);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(14)
void RamPresenter::findMostSickPersonInDistrict(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::MOST_SICK_PERSON);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(15)
void RamPresenter::findDistrictsOrderedBySickPeopleCount(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::ORDERED_BY_SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(16)
void RamPresenter::findRegionsOrderedBySickPeopleCount(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::ORDERED_BY_SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::REGION);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(17)
void RamPresenter::findAllTestsAtWorkplace(int workplaceId, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findAllTestsByLocationIdCommand.setParams(LOCATION::WORKPLACE, from, to, workplaceId);
		m_findAllTestsByLocationIdCommand.execute(output, recordCount);
		});
}

//(18)
void RamPresenter::findTest(unsigned int testId)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findTestCommand.setParams(testId);
		m_findTestCommand.execute(output, recordCount);
		});
}

//(19)
void RamPresenter::insert(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_insertCommand.setParams(birthNumber, firstName, lastName, birthDay);
		m_insertCommand.execute(output, recordCount);
		});
}

//(20)
void RamPresenter::removeTest(unsigned int testId)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_removeCommand.setParams(REMOVETYPE::TEST);
		m_removeCommand.setParams(testId);
		m_removeCommand.execute(output, recordCount);
		});
}

//(21)
void RamPresenter::removePerson(std::string birthNumber)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_removeCommand.setParams(REMOVETYPE::PERSON);
		m_removeCommand.setParams(birthNumber);
		m_removeCommand.execute(output, recordCount);
		});
}

void RamPresenter::saveToFile()
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_fileCommands.setParams(COMMANDTYPE::WRITE);
		m_fileCommands.execute(output, recordCount);
		});
}

void RamPresenter::loadFromFile()
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_fileCommands.setParams(COMMANDTYPE::READ);
		m_fileCommands.execute(output, recordCount);
		});
}