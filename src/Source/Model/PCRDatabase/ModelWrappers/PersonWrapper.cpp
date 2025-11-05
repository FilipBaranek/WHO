#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/PersonWrapper.h"

int PersonWrapper::compare(IComparable* other) const
{
	PersonWrapper* otherPersonWrapper = static_cast<PersonWrapper*>(other);
	Person* otherPerson = otherPersonWrapper->getData();

	if (m_person->birthNumber() > otherPerson->birthNumber())
	{
		return -1;
	}
	else if (m_person->birthNumber() < otherPerson->birthNumber())
	{
		return 1;
	}
	else
	{
		return 0;
	}

	return 0;
}

std::string PersonWrapper::writeLine()
{
	return m_person->toCsvFormat();
}

IStorable* PersonWrapper::loadLine(std::string line)
{
	std::vector<std::string> attributes;
	std::stringstream ss(line);
	std::string token;

	while (std::getline(ss, token, ';'))
	{
		attributes.push_back(token);
	}

	std::istringstream iss(attributes.at(3));
	std::chrono::sys_days tp;
	iss >> std::chrono::parse("%Y-%m-%d", tp);

	Person* person = new Person(
		attributes.at(0),
		attributes.at(1),
		attributes.at(2),
		std::chrono::year_month_day{ tp }
	);

	return new PersonWrapper(person);
}
