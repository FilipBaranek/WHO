#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/TestWrapper.h"
#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/PersonWrapper.h"

int TestWrapper::compare(IComparable* other) const
{
	TestWrapper* otherTestWrapper = static_cast<TestWrapper*>(other);
	PCRTest* otherTest = otherTestWrapper->getData();

	if (m_pcrTest->testId() > otherTest->testId())
	{
		return -1;
	}
	else if (m_pcrTest->testId() < otherTest->testId())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

std::string TestWrapper::writeLine()
{
	return m_pcrTest->toCsvFormat();
}

IStorable* TestWrapper::loadLine(std::string line)
{
	std::vector<std::string> attributes;
	std::stringstream ss(line);
	std::string token;

	while (std::getline(ss, token, ';'))
	{
		attributes.push_back(token);
	}

	std::tm tm = {};
	std::istringstream dateStream(attributes[7]);
	dateStream >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
	std::chrono::system_clock::time_point testDate = std::chrono::system_clock::from_time_t(std::mktime(&tm));

	PCRTest* test = new PCRTest(
		std::stoul(attributes.at(0)),       // testId
		std::stoul(attributes.at(4)),       // workplaceId
		std::stoul(attributes.at(5)),       // districtId
		std::stoul(attributes.at(6)),       // regionId
		std::stoi(attributes[2]) == 1,		// result
		std::stod(attributes.at(3)),        // testValue
		attributes.at(8),                   // note
		testDate,							// testDate
		attributes.at(1)                    // birthNumber
	);

	return new TestWrapper(test);
}
