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

void PersonWrapper::writeLine()
{
}

IStorable* PersonWrapper::loadLine()
{
	return nullptr;
}
