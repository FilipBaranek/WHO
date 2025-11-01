#include "../../Headers/Presenter/Presenter.h"

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

void Presenter::insert(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay)
{
	if (m_isExecuting)
	{
		return;
	}
	m_isExecuting = true;

	std::thread([=, this]() {
		std::string output, recordCount;

		m_insertCommand.setParams(birthNumber, firstName, lastName, birthDay);
		m_insertCommand.execute(output, recordCount);

		setOutput(output, recordCount);

		m_isExecuting = false;
	}).detach();
}

void Presenter::insert(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, 
					   bool result, double testValue, std::string note, std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber)
{
	if (m_isExecuting)
	{
		return;
	}
	m_isExecuting = true;

	std::thread([=, this]() {
		std::string output, recordCount;

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

		setOutput(output, recordCount);

		m_isExecuting = false;
	}).detach();
}

void Presenter::findResultByPatientAndTestId(unsigned int testId, std::string birthNumber, bool printPerson)
{
	m_resultCommand.setParams(testId, birthNumber, printPerson);
	m_resultCommand.execute(m_output, m_recordCount);
}
