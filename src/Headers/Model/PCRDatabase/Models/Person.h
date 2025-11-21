#pragma once
#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include "../../Interfaces/IRecord.h"
#include "../../HeapFile/Helpers/BinaryString.h"
#include "../ModelWrappers/TestWrapper.h"
#include "../../Structures/AVL/AVLTree.h"
#include "../../HeapFile/Helpers/ByteConverter.h"

class Person : public IRecord
{
private:
	static constexpr const int MAX_FIRST_NAME_SIZE = 15;
	static constexpr const int MAX_LAST_NAME_SIZE = 14;
	static constexpr const int MAX_BIRTHNUMBER_SIZE = 10;

	std::string m_birthNumber;
	std::string m_firstName;
	std::string m_lastName;
	std::chrono::year_month_day m_birthDay;

	BinaryString m_birthNumberBin;
	BinaryString m_firstNameBin;
	BinaryString m_lastNameBin;

public:
	Person(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay) :
		m_birthNumber(birthNumber), m_firstName(firstName), m_lastName(lastName), m_birthDay(birthDay),
		m_birthNumberBin{MAX_BIRTHNUMBER_SIZE, static_cast<int>(m_birthNumber.size()), m_birthNumber},
		m_firstNameBin{MAX_FIRST_NAME_SIZE, static_cast<int>(m_firstName.size()), m_firstName},
		m_lastNameBin{MAX_LAST_NAME_SIZE, static_cast<int>(m_lastName.size()), m_lastName}
	{}
	
	inline void setBirthNumber(std::string birthNumber) { m_birthNumber = birthNumber; }
	
	inline std::string birthNumber() { return m_birthNumber; }
	
	inline std::string firstName() { return m_firstName; }
	
	inline std::string lastName() { return m_lastName; }
	
	inline std::chrono::year_month_day birthDay() { return m_birthDay; }
	
	std::string toCsvFormat();
	
	std::string toString();

	IRecord* clone() override;
	
	bool equals(IRecord* other) override;

	bool is(IRecord* other) override;
	
	int getSize() override;
	
	bool toBytes(uint8_t* bytesOutput) override;
	
	IRecord* fromBytes(uint8_t* byteBuffer) override;

	static Person* dummyInstance();

	static Person* createInstance(uint8_t* byteBuffer);

	~Person() = default;
};