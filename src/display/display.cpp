#include "display.h"
#include "schedule_management.h"
#include "user.h"
#include <algorithm>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

User current_user;

bool checkForEsc()
{
    if (_kbhit())
    {
        char key = _getch();
        if (key == 27) 
        {
            return true;
        }
    }
    return false;
}

void waitForEnterOrEsc()
{
    cout << "\n(Press Enter to complete or press ESC to go back.)" << endl;
    while (true)
    {
        if (_kbhit())
        {
            char key = _getch();
            if (key == 13)
            {
                break;
            }
            else if (key == 27)
            {
                throw runtime_error(" Back ");
            }
        }
    }
}


void userSettings()
{
    try
    {
        system("cls");
        cout << "\nUser Settings" << endl;
        cout << "Name: ";
        cin >> current_user.name;
        while (current_user.name.empty())
        {
            cout << "The name is invalid. Please re-enter it: ";
            cin >> current_user.name;
        }
        cout << "Student ID: ";
        cin >> current_user.id;
        while (cin.fail() || to_string(current_user.id).length() != 8)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "The student ID is invalid. Please enter it in an 8-digit numeric format.: ";
            cin >> current_user.id;
        }
        cout << "Year (1-4): ";
        cin >> current_user.year;
        while (cin.fail() || current_user.year < 1 || current_user.year > 4)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Please enter the year as a number between 1 and 4: ";
            cin >> current_user.year;
        }
        cout << "Department: ";
        cin >> current_user.department;
        vector<string> validDepartments = {" Software ", " Other Departments "};
        while (find(validDepartments.begin(), validDepartments.end(), current_user.department) ==
               validDepartments.end())
        {
            cout << "The department name is invalid. Please re-enter it.: ";
            cin >> current_user.department;
        }
        cout << "User information has been saved.\n" << endl;
        waitForEnterOrEsc();
    }
    catch (runtime_error &)
    {
        cout << "User settings have been canceled.\n" << endl;
    }
}

void displayScheduleTable(const Table &table)
{
    system("cls");
    cout << "\n==================================================================================" << endl;
    cout << "Period |      Mon      |      Tue      |      Wed      |       Thur      |      Fri      |" << endl;
    cout << "====================================================================================" << endl;
    for (int i = 1; i <= 12; ++i)
    {
        cout << i << "Period | ";
        for (const Weekday &day : {Weekday::Mon, Weekday::Tue, Weekday::Wed, Weekday::Thu, Weekday::Fri})
        {
            bool slotFound = false;
            for (const auto &course : table.get_course())
            {
                for (const auto &time : course.get_times())
                {
                    if (time.weekday == day && time.time == i)
                    {
                        cout << left << setw(12) << (course.get_name() + "/" + course.get_professor()) << " | ";
                        slotFound = true;
                        break;
                    }
                }
                if (slotFound)
                    break;
            }
            if (!slotFound)
            {
                cout << left << setw(12) << "" << " | ";
            }
        }
        cout << "\n------------------------------------------------------------------------------------" << endl;
    }
}

void displayScheduleMenu()
{
    while (true)
    {
        system("cls");
        cout << "\nSchedule Management Menu:" << endl;
        cout << " 1. Create Schedule \n 2. Search and Edit Schedule \n 3. Return to Main Menu " << endl;
        int subChoice;
        cout << " Please select an option : ";
        cin >> subChoice;

        switch (subChoice)
        {
        case 1:
            generateSchedule();
            break;
        case 2:
            // modifySchedule();
            break;
        case 3:
            return;
        default:
            cout << "Invalid input. Please try again." << endl;
            break;
        }
    }
}

void generateSchedule()
{
    srand(time(0));
    int currentStep = 1;
    int totalSteps = 10;
    while (currentStep <= totalSteps)
    {
        try
        {
            system("cls");
            cout << "\nSchedule Creation Step " << currentStep << "/" << totalSteps << endl;
            switch (currentStep)
            {
            case 1: {
                cout << "Please enter the year for the schedule (e.g., 2023, 2024): ";
                string year;
                cin >> year;
                while (cin.fail() || year.length() != 4 || !isdigit(year[0]))
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Please enter a 4-digit number (e.g., 2023, 2024): ";
                    cin >> year;
                }
            }
            break;
            case 2: {
                cout << "\nPlease select your year:\n1. 1st Year\n2. 2nd Year\n3. 3rd Year\n4. 4th Year\n> ";
                int yearOption;
                cin >> yearOption;
                while (cin.fail() || yearOption < 1 || yearOption > 4)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Please select a number between 1 and 4: ";
                    cin >> yearOption;
                }
            }
            break;
            case 3: {
                cout << "\nPlease select a semester:\n1. Spring (1st Semester)\n2.  Summer (Seasonal Semester)\n3.  Fall (2nd Semester)\n4. Winter (Seasonal Semester)\n> ";
                int semester;
                cin >> semester;
                while (cin.fail() || semester < 1 || semester > 4)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Please select a number between 1 and 4: ";
                    cin >> semester;
                }
            }
            break;
            case 4: {
                cout << "\nPlease select a department:\n1. Software Department\n2. Other Department\n> ";
                int department;
                cin >> department;
                while (cin.fail() || department < 1 || department > 2)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "nvalid input. Please select either 1 or 2: ";
                    cin >> department;
                }
            }
            break;
            case 5: {
                cout << "\nPlease enter the number of credits you will take this semester (maximum 24 credits): ";
                int maxCredits;
                cin >> maxCredits;
                while (cin.fail() || maxCredits < 1 || maxCredits > 24)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Please enter a number of credits between 1 and 24: ";
                    cin >> maxCredits;
                }
            }
            break;
            case 6: {
                cout << "\nPlease select a professor priority:\n1. Professor A\n2. Professor B\n3. Professor C\n> ";
                int professor;
                cin >> professor;
                while (cin.fail() || professor < 1 || professor > 3)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Please select a number between 1 and 3: ";
                    cin >> professor;
                }
            }
            break;
            case 7: {
                vector<string> categories = {"1. Major", "2. General Education", "3. Next Step"};
                vector<int> selectedMajorBasicSubjects;
                vector<int> selectedMajorSubjects;
                vector<int> selectedMajorRequiredSubjects;
                vector<int> selectedGeneralBasicSubjects;
                while (true)
                {
                    cout << "\nPlease select a category:\n1. Major\n2. General Education\n3. Next Step\n> ";
                    int category;
                    cin >> category;
                    if (category == 3)
                    {
                        cout << "Proceeding to the next step." << endl;
                        break;
                    }
                    if (cin.fail() || category < 1 || category > 3)
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input. Please try again." << endl;
                        continue;
                    }
                }
            }
            break;
            case 8: {
                cout << "\nWould you like to select the English A course?\n1. Yes\n2. No\n> ";
                int englishA;
                cin >> englishA;
                while (cin.fail() || (englishA != 1 && englishA != 2))
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Please select 1 (Yes) or 2 (No): ";
                    cin >> englishA;
                }
            }
            break;
            case 9: {
                cout << "\nPlease set your free time:\n1. Mon\n2. Tue\n3. Wed\n4. Thur\n5. Fri\n0. Done\n> ";
                vector<int> selectedDays;
                while (true)
                {
                    int day;
                    cin >> day;
                    if (day == 0)
                    {
                        break;
                    }
                    while (cin.fail() || day < 1 || day > 5 ||
                           find(selectedDays.begin(), selectedDays.end(), day) != selectedDays.end())
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input. Please select a number between 1 and 5, or note that the day has already been selected. Try again: ";
                        cin >> day;
                    }
                    selectedDays.push_back(day);
                }
                cout << "Selected free days: ";
                for (int d : selectedDays)
                {
                    cout << d << " ";
                }
                cout << endl;

                cout << "\nPlease select your free time periods:\n1. Period 1\n2. Period 2\n3. Period 3\n4. Period 4\n5. Period 5\n6. Period 6\n7. Period 7\n8. Period 8\n9. Period 9\n10. Period 10\n11. Period 11\n12. Period 12\n0. Done\n> ";
                vector<int> selectedPeriods;
                while (true)
                {
                    int period;
                    cin >> period;
                    if (period == 0)
                    {
                        break;
                    }
                    while (cin.fail() || period < 1 || period > 12 ||
                           find(selectedPeriods.begin(), selectedPeriods.end(), period) != selectedPeriods.end())
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input. Please select a number between 1 and 12, or note that the period has already been selected. Try again: ";
                        cin >> period;
                    }
                    selectedPeriods.push_back(period);
                }
                cout << "Selected Free Periods: ";
                for (int p : selectedPeriods)
                {
                    cout << p << " ";
                }
                cout << endl;
            }
            break;
            case 10: {
                int scheduleID = rand() % 1000;
                cout << "\nSaving the schedule...\n";
                cout << "The schedule has been saved. Schedule ID: " << scheduleID << endl;
            }
            break;
            default: {
                cout << "Step completed." << endl;
            }
            break;
            }
            currentStep++;
        }
        catch (runtime_error &)
        {
            cout << "\nReturning to the schedule menu..." << endl;
            return;
        }
    }
    cout << "\nSchedule creation is complete." << endl;
    waitForEnterOrEsc();
}



/* void modifySchedule()
{
    try
    {
        system("cls");
        cout << "\n시간표 검색 및 수정" << endl;
        cout << "필요한 정보만을 입력 후, 검색할 시간표를 확인하세요." << endl;

        string userName = getInput("사용자 이름 (입력하지 않으면 전체 검색): ");
        int year = getOptionalIntInput("학년 (1-4, 입력하지 않으면 전체 검색): ", 1, 4);
        Semester semester =
            getOptionalSemesterInput("학기 (1: 봄, 2: 여름, 3: 가을, 4: 겨울, 입력하지 않으면 전체 검색): ");
        int scheduleID = getOptionalIntInput("시간표 ID (입력하지 않으면 전체 검색): ", 0, 9999);

        vector<Table> foundSchedules = tableDatabase.queryTables(userName, year, semester, scheduleID);
        if (foundSchedules.empty())
        {
            cout << "조건에 맞는 시간표가 없습니다. 다시 검색해주세요." << endl;
            waitForEnterOrEsc();
            return;
        }

        // 검색된 시간표 목록을 사용자에게 출력
        cout << "\n검색된 시간표 목록:" << endl;
        int index = 1;
        for (const Table &table : foundSchedules)
        {
            cout << index << ") 시간표 ID: " << table.get_id() << ", 학년: " << table.get_year()
                 << ", 학기: " << encodeSemester(table.get_semester()) << ", 사용자 이름: " << table.get_user_name()
                 << endl;
            index++;
        }

        int selectedScheduleIndex = navigateMenu(foundSchedules.size());
        if (selectedScheduleIndex == -1)
        {
            return;
        }

        modifySelectedSchedule(foundSchedules[selectedScheduleIndex - 1].get_id());
    }
    catch (runtime_error &)
    {
        cout << "시간표 수정을 취소하였습니다.\n" << endl;
    }
}

// Function to navigate through menu options
int navigateMenu(const vector<string> &options, int selectedOption)
{
    while (true)
    {
        system("cls");
        cout << "\n옵션을 선택해주세요:" << endl;
        for (int i = 0; i < options.size(); ++i)
        {
            if (i + 1 == selectedOption)
            {
                cout << "-> " << options[i] << endl;
            }
            else
            {
                cout << "   " << options[i] << endl;
            }
        }

        char input = getch();
        switch (input)
        {
        case 'w': // 위로 이동
            if (selectedOption > 1)
            {
                selectedOption--;
            }
            break;
        case 's': // 아래로 이동
            if (selectedOption < options.size())
            {
                selectedOption++;
            }
            break;
        case '\r': // 선택 (Enter 키)
            return selectedOption;
        case 27: // ESC 키로 뒤로가기
            return -1;
        default:
            break;
        }
    }
}

// Function to get input from the user
string getInput(const string &prompt)
{
    cout << prompt;
    string input;
    getline(cin, input);
    return input;
}

// Function to get optional integer input with validation
int getOptionalIntInput(const string &prompt, int min, int max)
{
    string input = getInput(prompt);
    if (input.empty())
    {
        return -1;
    }
    try
    {
        int value = stoi(input);
        if (value >= min && value <= max)
        {
            return value;
        }
        else
        {
            cout << "입력값이 범위를 벗어났습니다." << endl;
            return getOptionalIntInput(prompt, min, max);
        }
    }
    catch (...)
    {
        cout << "올바른 숫자를 입력해주세요." << endl;
        return getOptionalIntInput(prompt, min, max);
    }
}

// Function to get optional semester input
Semester getOptionalSemesterInput(const string &prompt)
{
    string input = getInput(prompt);
    if (input.empty())
    {
        return static_cast<Semester>(-1);
    }
    try
    {
        int value = stoi(input);
        if (value >= 1 && value <= 4)
        {
            return static_cast<Semester>(value - 1);
        }
        else
        {
            cout << "올바른 학기를 입력해주세요." << endl;
            return getOptionalSemesterInput(prompt);
        }
    }
    catch (...)
    {
        cout << "올바른 학기를 입력해주세요." << endl;
        return getOptionalSemesterInput(prompt);
    }
}

// Function to modify a selected schedule
void modifySelectedSchedule(int scheduleID)
{
    Table *selectedSchedule = tableDatabase.getTableById(scheduleID);
    if (!selectedSchedule)
    {
        cout << "시간표를 찾을 수 없습니다." << endl;
        return;
    }
    displayLectureList(*selectedSchedule);

    vector<string> editOptions = {" 1. 메인 시간표 지정 ", " 2. 시간표 삭제 ", " 3. 시간표 수정 ", " 4. 뒤로가기 "};
    int editChoice = navigateMenu(editOptions);

    switch (editChoice)
    {
    case 1:
        tableDatabase.setMainSchedule(scheduleID);
        cout << "메인 시간표로 지정되었습니다." << endl;
        break;
    case 2:
        deleteSchedule(scheduleID);
        break;
    case 3:
        modifyExistingSchedule(*selectedSchedule);
        break;
    case 4:
        return;
    default:
        cout << "잘못된 입력입니다." << endl;
        break;
    }
}

// Function to delete a schedule
void deleteSchedule(int scheduleID)
{
    if (tableDatabase.removeTable(scheduleID))
    {
        cout << "시간표가 삭제되었습니다." << endl;
    }
    else
    {
        cout << "시간표 삭제에 실패하였습니다." << endl;
    }
    waitForEnterOrEsc();
}

// Function to modify an existing schedule
void modifyExistingSchedule(Table &selectedSchedule)
{
    while (true)
    {
        vector<string> modifyOptions = {"1. 강의 추가", "2. 강의 삭제", "3. 완료"};
        int modifyChoice = navigateMenu(modifyOptions);

        switch (modifyChoice)
        {
        case 1:
            addLectureToSchedule(selectedSchedule);
            break;
        case 2:
            deleteLectureFromSchedule(selectedSchedule);
            break;
        case 3:
            return;
        default:
            cout << "잘못된 입력입니다." << endl;
            break;
        }
    }
}

// Function to add a lecture to a schedule
void addLectureToSchedule(Table &schedule)
{
    cout << "\n추가할 강의 정보를 입력해주세요." << endl;
    string lectureName = getInput("강의명: ");
    int lectureDay = getOptionalIntInput("강의 요일 (1: 월요일, 2: 화요일, ... 5: 금요일): ", 1, 5);
    int lecturePeriod = getOptionalIntInput("강의 교시 (1-12): ", 1, 12);

    Course newCourse(lectureName, static_cast<Weekday>(lectureDay - 1), lecturePeriod);
    if (schedule.addCourse(newCourse))
    {
        cout << "강의가 성공적으로 추가되었습니다." << endl;
    }
    else
    {
        cout << "강의 추가에 실패하였습니다." << endl;
    }
    waitForEnterOrEsc();
}

// Function to delete a lecture from a schedule
void deleteLectureFromSchedule(Table &schedule)
{
    cout << "\n삭제할 강의명을 입력해주세요." << endl;
    string lectureName = getInput("강의명: ");
    if (schedule.removeCourse(lectureName))
    {
        cout << "강의가 성공적으로 삭제되었습니다." << endl;
    }
    else
    {
        cout << "강의 삭제에 실패하였습니다." << endl;
    }
    waitForEnterOrEsc();
}

// Function to display the lecture list of a schedule
void displayLectureList(const Table &table)
{
    cout << "\n선택한 시간표(ID: " << table.get_id() << ")의 현재 강의 목록입니다:" << endl;
    cout << "번호 | 강의명         | 교수명      | 학점 | 요일/교시" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    const auto &courses = table.get_courses();
    for (size_t i = 0; i < courses.size(); ++i)
    {
        const Course &course = courses[i];
        cout << setw(4) << (i + 1) << " | " << setw(12) << course.get_name() << " | " << setw(10)
             << course.get_professor() << " | " << setw(4) << course.get_grade() << " | ";
        for (const auto &time : course.get_times())
        {
            cout << encodeWeekday(time.weekday) << "/" << time.time << "  ";
        }
        cout << endl;
    }
    waitForEnterOrEsc();
}*/