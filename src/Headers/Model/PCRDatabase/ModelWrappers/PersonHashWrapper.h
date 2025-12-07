#pragma once
#include <array>
#include <chrono>
#include "../Models/Person.h"
#include "../../Interfaces/IRecord.h"

class PersonHashWrapper : public IRecord
{
private:
	static constexpr const int MAX_TEST_COUNT = 6;

	Person* m_person;
	int m_validTestCount;
	std::array<unsigned int, MAX_TEST_COUNT> m_tests;

public:
	PersonHashWrapper(Person* person) : m_person(person), m_tests{}, m_validTestCount(0) {};

	inline void removePerson() { m_person = nullptr; }

	inline Person* getData() { return m_person; }

	inline void setTestCount(int testCount) { m_validTestCount = testCount; }

	inline int testCount() { return m_validTestCount; }

	inline void inserTest(unsigned int test)
	{
		m_tests[m_validTestCount] = test;
		++m_validTestCount;
	}

	inline std::array<unsigned int, MAX_TEST_COUNT>& tests() { return m_tests; }

	std::string toString() override;

	IRecord* clone() override;

	bool equals(IRecord* other) override;

	bool is(IRecord* other) override;

	int getSize() override;

	bool toBytes(uint8_t* bytesOutput) override;

	IRecord* fromBytes(uint8_t* byteBuffer) override;

	size_t hash() override;

	static PersonHashWrapper* dummyInstance();

	static PersonHashWrapper* createInstance(uint8_t* byteBuffer);

	~PersonHashWrapper();
};