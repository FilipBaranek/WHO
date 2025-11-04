#pragma once
#include "../../Interfaces/IComparable.h"
#include "../../Interfaces/IStorable.h"
#include "../Models/PCRTest.h"

class PersonWrapper;

class TestByDateWrapper : public IComparable, public IStorable
{
private:
	PCRTest* m_pcrTest;
	PersonWrapper* m_person;

public:
	TestByDateWrapper(PCRTest* pcrTest, PersonWrapper* person = nullptr) : m_pcrTest(pcrTest), m_person(person) {}
	inline PCRTest* getData() { return m_pcrTest; }
	inline PersonWrapper* person() { return m_person; }
	int compare(IComparable* other) const override;
	void writeLine() override;
	IStorable* loadLine() override;
	~TestByDateWrapper() = default;
};