#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include "../../HeapFile/Helpers/BinaryString.h"

class ReducedPCRTest
{
private:
	static constexpr const int NODE_MAX_LENGTH = 11;

protected:
	unsigned int m_testId;
	bool m_result;
	double m_testValue;
	std::chrono::time_point<std::chrono::system_clock> m_testDate;
	std::string m_note;

	BinaryString m_noteBin;

public:
	ReducedPCRTest(unsigned int testId, bool result, double testValue, std::string note, std::chrono::time_point<std::chrono::system_clock> testDate) :
		m_testId(testId), m_result(result), m_testValue(testValue), m_note(note), m_testDate(testDate),
		m_noteBin{NODE_MAX_LENGTH, static_cast<int>(m_note.size()), m_note}
	{};

	inline unsigned int testId() { return m_testId; }

	inline bool result() { return m_result; }

	inline double testValue() { return m_testValue; }

	inline std::string note() { return m_note; }

	inline std::chrono::time_point<std::chrono::system_clock> testDate() { return m_testDate; }

	BinaryString& binNote() { return m_noteBin; }

	virtual std::string toCsvFormat();

	virtual std::string toString();

	~ReducedPCRTest() = default;
};