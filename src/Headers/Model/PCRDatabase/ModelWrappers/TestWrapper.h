#pragma once
#include "../../Interfaces/IComparable.h"
#include "../../Interfaces/IStorable.h"
#include "../Models/PCRTest.h"

class PersonWrapper;

class TestWrapper : public IComparable, public IStorable
{
protected:
	PCRTest* m_pcrTest;
	PersonWrapper* m_person;

public:
	TestWrapper(PCRTest* pcrTest, PersonWrapper* person = nullptr) : m_pcrTest(pcrTest), m_person(person) {}
	inline PCRTest* getData() { return m_pcrTest; }
	inline PersonWrapper* person() { return m_person; }
	int compare(IComparable* other) const override;
	std::string writeLine() override;
	static IStorable* loadLine(std::string line);
	~TestWrapper() = default;
};