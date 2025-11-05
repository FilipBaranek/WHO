#include "../../Headers/Presenter/Presenter.h"

void Presenter::execute(std::function<void(std::string& output, std::string& recordCount)> callback)
{
	if (m_isExecuting)
	{
		return;
	}
	m_isExecuting = true;

	std::thread([=, this]() {
		std::string output, recordCount;

		callback(output, recordCount);

		setOutput(output, recordCount);

		m_isExecuting = false;
	}).detach();
}

void Presenter::setOutput(std::string output, std::string recordCount)
{
	std::lock_guard<std::mutex> lock(m_outputMutex);
	m_output = output;
	m_recordCount = recordCount;
}

std::pair<std::string, std::string> Presenter::output()
{
	std::lock_guard<std::mutex> lock(m_outputMutex);
	return { m_output, m_recordCount };
}

void Presenter::generatePeople(int count)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_generateCommand.setPeopleParams(count);
		m_generateCommand.execute(output, recordCount);
	});
}

void Presenter::generateTests(int count)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_generateCommand.setTestParams(count);
		m_generateCommand.execute(output, recordCount);
	});
}

//(1)
void Presenter::insert(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, 
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

//(2)
void Presenter::findResultByPatientAndTestId(unsigned int testId, std::string birthNumber)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_resultCommand.setParams(testId, birthNumber);
		m_resultCommand.execute(output, recordCount);
	});
}

//(3)
void Presenter::findPatientTestOrderByDate(std::string birthNumber)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPatientsTestCommand.setParams(birthNumber);
		m_findPatientsTestCommand.execute(output, recordCount);
	});
}

//(4)
void Presenter::findPositiveTestsInDistrict(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::BY_DATE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(5)
void Presenter::findAllTestsInDistrict(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findAllTestsByLocationIdCommand.setParams(LOCATION::DISTRICT, from, to, id);
		m_findAllTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(6)
void Presenter::findPositiveTestsInRegion(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::BY_DATE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::REGION);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(7)
void Presenter::findAllTestsInRegion(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findAllTestsByLocationIdCommand.setParams(LOCATION::REGION, from, to, id);
		m_findAllTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(8)
void Presenter::findPositiveTests(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::BY_DATE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::COUNTRY);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(9)
void Presenter::findAllTests(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findAllTestsByLocationIdCommand.setParams(LOCATION::COUNTRY, from, to);
		m_findAllTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(10)
void Presenter::findSickPeopleInDistrict(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(11)
void Presenter::findSickPeopleInDistrictOrderByTestValue(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id, true);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(12)
void Presenter::findSickPeopleInRegion(const unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::REGION);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(13)
void Presenter::findSickPeople(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::COUNTRY);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(14)
void Presenter::findMostSickPersonInDistrict(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::MOST_SICK_PERSON);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(15)
void Presenter::findDistrictsOrderedBySickPeopleCount(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::ORDERED_BY_SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(16)
void Presenter::findRegionsOrderedBySickPeopleCount(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::ORDERED_BY_SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::REGION);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(17)
void Presenter::findAllTestsAtWorkplace(int workplaceId, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findAllTestsByLocationIdCommand.setParams(LOCATION::WORKPLACE, from, to, workplaceId);
		m_findAllTestsByLocationIdCommand.execute(output, recordCount);
	});
}

//(18)
void Presenter::findTest(unsigned int testId)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findTestCommand.setParams(testId);
		m_findTestCommand.execute(output, recordCount);
	});
}

//(19)
void Presenter::insert(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_insertCommand.setParams(birthNumber, firstName, lastName, birthDay);
		m_insertCommand.execute(output, recordCount);
	});
}

//(20)
void Presenter::removeTest(unsigned int testId)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_removeCommand.setParams(REMOVETYPE::TEST);
		m_removeCommand.setParams(testId);
		m_removeCommand.execute(output, recordCount);
	});
}

//(21)
void Presenter::removePerson(std::string birthNumber)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_removeCommand.setParams(REMOVETYPE::PERSON);
		m_removeCommand.setParams(birthNumber);
		m_removeCommand.execute(output, recordCount);
	});
}

void Presenter::printAllData()
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_printAllDataCommand.execute(output, recordCount);
	});
}

void Presenter::saveToFile()
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_fileCommands.setParams(COMMANDTYPE::WRITE);
		m_fileCommands.execute(output, recordCount);
	});
}

void Presenter::loadFromFile()
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_fileCommands.setParams(COMMANDTYPE::READ);
		m_fileCommands.execute(output, recordCount);
	});
}