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

void Presenter::insert(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_insertCommand.setParams(birthNumber, firstName, lastName, birthDay);
		m_insertCommand.execute(output, recordCount);
	});
}

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

void Presenter::printAllData()
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_printAllDataCommand.execute(output, recordCount);
	});
}

void Presenter::findResultByPatientAndTestId(unsigned int testId, std::string birthNumber, bool printPerson)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_resultCommand.setParams(testId, birthNumber, printPerson);
		m_resultCommand.execute(output, recordCount);
	});
}

void Presenter::findPatientTestOrderByDate(std::string birthNumber)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPatientsTestCommand.setParams(birthNumber);
		m_findPatientsTestCommand.execute(output, recordCount);
	});
}

void Presenter::findPositiveTestsInDistrictCommand(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::BY_DATE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

void Presenter::findAllTestsInDistrictCommand(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findAllTestsByLocationIdCommand.setParams(LOCATION::DISTRICT, from, to, id);
		m_findAllTestsByLocationIdCommand.execute(output, recordCount);
	});
}

void Presenter::findPositiveTestsInRegionCommand(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::BY_DATE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::REGION);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

void Presenter::findAllTestsInRegionCommand(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findAllTestsByLocationIdCommand.setParams(LOCATION::REGION, from, to, id);
		m_findAllTestsByLocationIdCommand.execute(output, recordCount);
	});
}

void Presenter::findPositiveTests(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::BY_DATE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::COUNTRY);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

void Presenter::findAllTests(time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findAllTestsByLocationIdCommand.setParams(LOCATION::COUNTRY, from, to);
		m_findAllTestsByLocationIdCommand.execute(output, recordCount);
	});
}

void Presenter::findSickPeopleInDistrictCommand(unsigned int id, time_point<system_clock> from, time_point<system_clock> to)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findPositiveTestsByLocationIdCommand.setParams(SEARCHTYPE::SICK_PEOPLE);
		m_findPositiveTestsByLocationIdCommand.setParams(LOCATION::DISTRICT);
		m_findPositiveTestsByLocationIdCommand.setParams(from, to, id);
		m_findPositiveTestsByLocationIdCommand.execute(output, recordCount);
	});
}

void Presenter::findTest(unsigned int testId)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findTestCommand.setParams(testId);
		m_findTestCommand.execute(output, recordCount);
	});
}

void Presenter::removeTest(unsigned int testId)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_removeCommand.setParams(REMOVETYPE::TEST);
		m_removeCommand.setParams(testId);
		m_removeCommand.execute(output, recordCount);
	});
}

void Presenter::removePerson(std::string birthNumber)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_removeCommand.setParams(REMOVETYPE::PERSON);
		m_removeCommand.setParams(birthNumber);
		m_removeCommand.execute(output, recordCount);
	});
}
