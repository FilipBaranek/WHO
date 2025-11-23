#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <functional>
#include <chrono>

using namespace std::chrono;

class Presenter
{
private:
	bool m_isExecuting;
	std::mutex m_outputMutex;
	std::string m_output;
	std::string m_recordCount;

protected:
	void execute(std::function<void(std::string& output, std::string& recordCount)> callback);

public:
	Presenter() : m_isExecuting(false) {}
	
	inline bool isExecuting() { return m_isExecuting; }

	void setOutput(std::string output, std::string recordCount);
	
	std::pair<std::string, std::string> output();

	virtual void generatePeople(int count) = 0;

	virtual void generateTests(int count) = 0;
	
	virtual void insert(unsigned int testId, unsigned int workplaceId, unsigned int districtId,
						unsigned int regionId, bool result, double testValue, std::string note,
						time_point<system_clock> testDate, std::string birthNumber) = 0;

	virtual void insert(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay) = 0;

	virtual void printAllData() = 0;

	virtual ~Presenter() = default;
};