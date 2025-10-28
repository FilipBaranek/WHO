#pragma once
#include <string>
#include <chrono>
#include <iostream>

class Person
{
private:
	std::string m_birthNumber;
	std::string m_firstName;
	std::string m_lastName;
	std::chrono::year_month_day m_birthDay;

public:
	Person(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay);
	inline std::string birthNumber();
	inline std::string firstName();
	inline std::string lastName();
	inline std::chrono::year_month_day birthDay();
	inline void print();
	~Person() = default;
};