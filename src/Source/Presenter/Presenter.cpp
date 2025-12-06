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

void Presenter::findTest(unsigned int testId)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_findTestCommand.setParams(testId);
		m_findTestCommand.execute(output, recordCount);
	});
}

void Presenter::printAllData()
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_printAllDataCommand.execute(output, recordCount);
	});
}
