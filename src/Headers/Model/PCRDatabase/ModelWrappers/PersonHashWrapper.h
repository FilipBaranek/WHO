#pragma once
#include <vector>
#include <chrono>
#include "../Models/Person.h"
#include "../../Interfaces/IRecord.h"

class PersonHashWrapper : public IRecord
{
private:
	static constexpr const int MAX_TEST_COUNT = 6;

	Person* m_person;
	std::vector<unsigned int> m_tests;

public:
	PersonHashWrapper(Person* person) : m_person(person) {};

	inline Person* getData() { return m_person; }

	inline std::vector<unsigned int>& tests() { return m_tests; }

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