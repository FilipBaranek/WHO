#pragma once
#include <string>
#include <vector>
#include "Window.h"

class TestWindow : public Window
{
private:
    static constexpr const unsigned int MAX_WORKPLACE_CODE = 158;
    static constexpr const unsigned int MAX_DISTRICT_CODE = 79;
    static constexpr const unsigned int MAX_REGION_CODE = 8;

    int m_testId = 0;
    int m_workplaceId = 0;
    int m_districtId = 0;
    int m_regionId = 0;
    bool m_result = false;
    double m_testValue = 0;
    std::string m_note;
    std::string m_birthNumber;
    std::chrono::time_point<std::chrono::system_clock> m_testDate;

    static constexpr const int CHARACTER_SIZE = 30;
    char noteBuf[CHARACTER_SIZE];
    char birthNumberBuf[CHARACTER_SIZE];

    int m_selectedRegion = 0;
    int m_selectedDistrict = 0;
    int m_selectedWorkplace = 0;

    std::vector<std::string> m_regions;
    std::vector<std::vector<std::string>> m_districts;
    std::vector<std::vector<std::vector<std::string>>> m_workplaces;

    void displayRegionCombobox();
    void displayDistrictCombobox();
    void displayWorkplaceCombobox();

public:
    TestWindow(Presenter* presenter) : Window(presenter) {};
    void renderWindow() override;
    ~TestWindow() = default;
};
