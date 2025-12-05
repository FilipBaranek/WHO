#pragma once
#include <string>
#include "../../Interfaces/IRecord.h"
#include "../Models/ReducedPCRTest.h"
#include "../../HeapFile/Helpers/BinaryString.h"
#include "../../HeapFile/Helpers/ByteConverter.h"

class TestHashWrapper : public IRecord
{
private:
	static constexpr const int BIRTH_NUMBER_MAX_SIZE = 10;

protected:
	ReducedPCRTest* m_pcrTest;
	std::string m_person;

	BinaryString m_binPerson;

public:
	TestHashWrapper(ReducedPCRTest* pcrTest, std::string person = "") : 
		m_pcrTest(pcrTest), m_person(person), m_binPerson{BIRTH_NUMBER_MAX_SIZE, static_cast<int>(m_person.size()), m_person}

	{}
	
	inline ReducedPCRTest* getData() { return m_pcrTest; }
	
	inline std::string person() { return m_person; }
	
	inline void setPerson(std::string person) { m_person = person; }

	BinaryString& binPerson() { return m_binPerson; }

	std::string toString() override;

	IRecord* clone() override;

	bool equals(IRecord* other) override;

	bool is(IRecord* other) override;

	int getSize() override;

	bool toBytes(uint8_t* bytesOutput) override;

	IRecord* fromBytes(uint8_t* byteBuffer) override;

	size_t hash() override;

	static TestHashWrapper* dummyInstance();

	static TestHashWrapper* createInstance(uint8_t* byteBuffer);

	~TestHashWrapper();
};