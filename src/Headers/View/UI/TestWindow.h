#pragma once
#include <string>
#include "Window.h"

class TestWindow : public Window
{
private:
	int m_testId = 0;
	int m_workplaceId = 0;
	int m_districtId = 0;
	int m_regionId = 0;
	bool m_result = false;
	double m_testValue = 0;
	std::string m_note;
	std::chrono::time_point<std::chrono::system_clock> m_testDate;
	std::string m_birthNumber;

	static constexpr const int CHARACTER_SIZE = 30;
	char noteBuf[CHARACTER_SIZE];
	char birthNumberBuf[CHARACTER_SIZE];

public:
	TestWindow(Presenter* presenter) : Window(presenter) {}
	void renderWindow() override;
	~TestWindow() override = default;
};