#pragma once
#include "../../Interfaces/IComparable.h"
#include "../Models/Person.h"

class PersonWrapper : public IComparable
{
private:
	Person* m_person;

public:
	PersonWrapper(Person* person) : m_person(person) {};
	inline Person* getData() { return m_person; }
	int compare(IComparable* other) const override;
	~PersonWrapper() = default;
};