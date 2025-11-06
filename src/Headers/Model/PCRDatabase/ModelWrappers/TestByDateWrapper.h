#pragma once
#include "../../Interfaces/IComparable.h"
#include "../Models/PCRTest.h"

class PersonWrapper;

class TestByDateWrapper : public IComparable
{
private:
	PCRTest* m_pcrTest;
	PersonWrapper* m_person;

public:
	TestByDateWrapper(PCRTest* pcrTest, PersonWrapper* person = nullptr) : m_pcrTest(pcrTest), m_person(person) {}
	inline PCRTest* getData() { return m_pcrTest; }
	inline PersonWrapper* person() { return m_person; }
	inline void setPerson(PersonWrapper* person) { m_person = person; }
	int compare(IComparable* other) const override;
	~TestByDateWrapper() = default;
};