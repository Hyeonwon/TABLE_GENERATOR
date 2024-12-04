#include "display.h"
#include "schedule_management.h"
#include "user.h"
#include "course.h"
#include "table.h"
#include "table_db.h"
#include "course_db.h"
#include "parser.h"
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
TableDatabase tableDatabase;

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

void mainMenu()
{
    TableDatabase tableDatabase;
    if (!tableDatabase.get_tables().empty())
    {
        displayScheduleTable(tableDatabase.get_tables()[0]);
    }
    int selectedOption = 1;
    while (true)
    {
        system("cls");
        cout << "\n==================================================================================" << endl;
        cout << "[Chung-Ang University Schedule Management System]" << endl;
        cout << "==================================================================================" << endl;
        string options[] = {" 1. User Settings", " 2. Schedule", " 3. Exit"};
        for (int i = 0; i < 3; ++i)
        {
            if (i + 1 == selectedOption)
            {
                cout << "> " << options[i] << endl;
            }
            else
            {
                cout << "  " << options[i] << endl;
            }
        }
        cout << "==================================================================================" << endl;

        char key = _getch();
        if (key == 13)
        {
            switch (selectedOption)
            {
            case 1:
                userSettings();
                break;
            case 2:
                if (!tableDatabase.get_tables().empty())
                {
                    displayScheduleTable(tableDatabase.get_tables()[0]);
                }
                break;
            case 3:
                cout << "Exiting the program." << endl;
                return;
            }
        }
        else if (key == 72)
        {
            selectedOption = (selectedOption == 1) ? 3 : selectedOption - 1;
        }
        else if (key == 80)
        {
            selectedOption = (selectedOption == 3) ? 1 : selectedOption + 1;
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
        vector<string> validDepartments = {"Software", "Other Departments"};
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
    vector<string> options = {"1. Create Schedule", "2. Search and Edit Schedule", "3. Return to Main Menu"};
    int selectedOption = 1;
    while (true)
    {
        system("cls");
        for (int i = 0; i < options.size(); ++i)
        {
            if (i + 1 == selectedOption)
            {
                cout << "> " << options[i] << endl;
            }
            else
            {
                cout << "  " << options[i] << endl;
            }
        }
        char key = _getch();
        if (key == 13)
        {
            switch (selectedOption)
            {
            case 1:
                generateSchedule();
                break;
            case 2:
                modifySchedule();
                break;
            case 3:
                return;
            }
        }
        else if (key == 72)
        {
            selectedOption = (selectedOption == 1) ? options.size() : selectedOption - 1;
        }
        else if (key == 80)
        {
            selectedOption = (selectedOption == options.size()) ? 1 : selectedOption + 1;
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
                cout << "\nPlease select a semester:\n1. Spring (1st Semester)\n2. Summer (Seasonal Semester)\n3. Fall "
                        "(2nd Semester)\n4. Winter (Seasonal Semester)\n> ";
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
                    cout << "Invalid input. Please select either 1 or 2: ";
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
                vector<int> selectedMajorBasicSubjects;          // Major Basics
                vector<int> selectedMajorSubjects;               // Major
                vector<int> selectedMajorRequiredSubjects;       // Major Requirements
                vector<int> selectedGeneralBasicSubjects;        // General Education Basics
                vector<int> selectedCoreChallengeSubjects;       // Core - Challenge
                vector<int> selectedCoreCreativitySubjects;      // Core - Creativity
                vector<int> selectedCoreConvergenceSubjects;     // Core - Convergence
                vector<int> selectedCoreTrustSubjects;           // Core - Trust
                vector<int> selectedCoreCommunicationSubjects;   // Core - Communication
                vector<int> selectedChoiceChallengeSubjects;     // Choice - Challenge
                vector<int> selectedChoiceCreativitySubjects;    // Choice - Creativity
                vector<int> selectedChoiceConvergenceSubjects;   // Choice - Convergence
                vector<int> selectedChoiceTrustSubjects;         // Choice - Trust
                vector<int> selectedChoiceCommunicationSubjects; // Choice - Communication

                while (true)
                {
                    system("cls");
                    int category;
                    cout << "\nPlease select a category:\n";
                    for (int i = 0; i < categories.size(); ++i)
                    {
                        cout << i + 1 << ". " << categories[i] << "\n";
                    }
                    cout << "> ";
                    cin >> category;
                    while (cin.fail() || category < 1 || category > categories.size())
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input. Please select a number between 1 and " << categories.size() << ": ";
                        cin >> category;
                    }
                    if (category == 1)
                    {
                        vector<string> majorOptions = {"1. Major Basics", "2. Major Courses", "3. Major Requirements",
                                                       "4. Go Back"};
                        while (true)
                        { 
                            system("cls");
                            int majorChoice;
                            cout << "\nPlease select a major option:\n";
                            for (int i = 0; i < majorOptions.size(); ++i)
                            {
                                cout << i + 1 << ". " << majorOptions[i] << "\n";
                            }
                            cout << "> ";
                            cin >> majorChoice;
                            while (cin.fail() || majorChoice < 1 || majorChoice > majorOptions.size())
                            {
                                cin.clear();
                                cin.ignore(1000, '\n');
                                cout << "Invalid input. Please select a number between 1 and " << majorOptions.size()
                                     << ": ";
                                cin >> majorChoice;
                            }
                            if (majorChoice == 4)
                            {
                                break;
                            }
                            switch (majorChoice)
                            {
                            case 1: {
                                while (true)
                                {
                                    system("cls");
                                    vector<string> subjects = {"1. Data Structures", "2. Algorithms",
                                                               "3. Programming Basics", "4. Go Back"};
                                    cout << "\nPlease select a Major Basics subject (multiple selection allowed, press "
                                            "ESC to go back):\n";
                                    for (const auto &subject : subjects)
                                    {
                                        cout << subject << "\n";
                                    }
                                    cout << "Currently selected subjects: ";
                                    for (int subject : selectedMajorBasicSubjects)
                                    {
                                        cout << subject << " ";
                                    }
                                    cout << "\n> ";
                                    int subjectChoice;
                                    cin >> subjectChoice;

                                    if (checkForEsc() || subjectChoice == 4)
                                    {
                                        break;
                                    }

                                    if (cin.fail() || subjectChoice < 1 || subjectChoice > subjects.size() - 1)
                                    {
                                        cin.clear();
                                        cin.ignore(1000, '\n');
                                        cout << "Invalid input. Please try again." << endl;
                                    }
                                    else
                                    {
                                        if (find(selectedMajorBasicSubjects.begin(), selectedMajorBasicSubjects.end(),
                                                 subjectChoice) != selectedMajorBasicSubjects.end())
                                        {
                                            cout << "Subject already selected." << endl;
                                        }
                                        else
                                        {
                                            selectedMajorBasicSubjects.push_back(subjectChoice);
                                        }
                                    }
                                }
                            }
                            break;
                            case 2: {
                                while (true)
                                {
                                    system("cls");
                                    vector<string> subjects = {"1. Operating Systems", "2. Networks", "3. Databases",
                                                               "4. Go Back"};
                                    cout << "\nPlease select a Major Course (multiple selection allowed, press ESC to "
                                            "go back):\n";
                                    for (const auto &subject : subjects)
                                    {
                                        cout << subject << "\n";
                                    }
                                    cout << "Currently selected subjects: ";
                                    for (int subject : selectedMajorSubjects)
                                    {
                                        cout << subject << " ";
                                    }
                                    cout << "\n> ";
                                    int subjectChoice;
                                    cin >> subjectChoice;

                                    if (checkForEsc() || subjectChoice == 4)
                                    {
                                        break;
                                    }

                                    if (cin.fail() || subjectChoice < 1 || subjectChoice > subjects.size() - 1)
                                    {
                                        cin.clear();
                                        cin.ignore(1000, '\n');
                                        cout << "Invalid input. Please try again." << endl;
                                    }
                                    else
                                    {
                                        if (find(selectedMajorSubjects.begin(), selectedMajorSubjects.end(),
                                                 subjectChoice) != selectedMajorSubjects.end())
                                        {
                                            cout << "Subject already selected." << endl;
                                        }
                                        else
                                        {
                                            selectedMajorSubjects.push_back(subjectChoice);
                                        }
                                    }
                                }
                            }
                            break;
                            case 3: {
                                while (true)
                                {
                                    system("cls");
                                    vector<string> subjects = {"1. Software Engineering", "2. Computer Architecture",
                                                               "3. Computer Vision", "4. Go Back"};
                                    cout << "\nPlease select a Major Requirement (multiple selection allowed, press "
                                            "ESC to go back):\n";
                                    for (const auto &subject : subjects)
                                    {
                                        cout << subject << "\n";
                                    }
                                    cout << "Currently selected subjects: ";
                                    for (int subject : selectedMajorRequiredSubjects)
                                    {
                                        cout << subject << " ";
                                    }
                                    cout << "\n> ";
                                    int subjectChoice;
                                    cin >> subjectChoice;

                                    if (checkForEsc() || subjectChoice == 4)
                                    {
                                        break;
                                    }

                                    if (cin.fail() || subjectChoice < 1 || subjectChoice > subjects.size() - 1)
                                    {
                                        cin.clear();
                                        cin.ignore(1000, '\n');
                                        cout << "Invalid input. Please try again." << endl;
                                    }
                                    else
                                    {
                                        if (find(selectedMajorRequiredSubjects.begin(),
                                                 selectedMajorRequiredSubjects.end(),
                                                 subjectChoice) != selectedMajorRequiredSubjects.end())
                                        {
                                            cout << "Subject already selected." << endl;
                                        }
                                        else
                                        {
                                            selectedMajorRequiredSubjects.push_back(subjectChoice);
                                        }
                                    }
                                }
                            }
                            break;
                            }
                        }
                    }
                    else if (category == 2)
                    {
                        vector<string> generalOptions = {"1. General Education Basics",
                                                         "2. Core - Challenge",
                                                         "3. Core - Creativity",
                                                         "4. Core - Convergence",
                                                         "5. Core - Trust",
                                                         "6. Core - Communication",
                                                         "7. Choice - Challenge",
                                                         "8. Choice - Creativity",
                                                         "9. Choice - Convergence",
                                                         "10. Choice - Trust",
                                                         "11. Choice - Communication",
                                                         "12. Go Back"};
                        while (true)
                        {
                            system("cls");
                            int generalChoice;
                            cout << "\nPlease select a general education option:\n";
                            for (int i = 0; i < generalOptions.size(); ++i)
                            {
                                cout << i + 1 << ". " << generalOptions[i] << "\n";
                            }
                            cout << "> ";
                            cin >> generalChoice;
                            while (cin.fail() || generalChoice < 1 || generalChoice > generalOptions.size())
                            {
                                cin.clear();
                                cin.ignore(1000, '\n');
                                cout << "Invalid input. Please select a number between 1 and " << generalOptions.size()
                                     << ": ";
                                cin >> generalChoice;
                            }
                            if (generalChoice == 12)
                            {
                                break;
                            }
                            switch (generalChoice)
                            {
                            case 1: {
                                while (true)
                                {
                                    system("cls");
                                    vector<string> subjects = {"1. Philosophy", "2. Psychology", "3. Art History",
                                                               "4. Go Back"};
                                    cout << "\nPlease select a General Education Basics subject (multiple selection "
                                            "allowed, press ESC to go back):\n";
                                    for (const auto &subject : subjects)
                                    {
                                        cout << subject << "\n";
                                    }
                                    cout << "Currently selected subjects: ";
                                    for (int subject : selectedGeneralBasicSubjects)
                                    {
                                        cout << subject << " ";
                                    }
                                    cout << "\n> ";
                                    int subjectChoice;
                                    cin >> subjectChoice;

                                    if (checkForEsc() || subjectChoice == 4)
                                    {
                                        break;
                                    }

                                    if (cin.fail() || subjectChoice < 1 || subjectChoice > subjects.size() - 1)
                                    {
                                        cin.clear();
                                        cin.ignore(1000, '\n');
                                        cout << "Invalid input. Please try again." << endl;
                                    }
                                    else
                                    {
                                        if (find(selectedGeneralBasicSubjects.begin(),
                                                 selectedGeneralBasicSubjects.end(),
                                                 subjectChoice) != selectedGeneralBasicSubjects.end())
                                        {
                                            cout << "Subject already selected." << endl;
                                        }
                                        else
                                        {
                                            selectedGeneralBasicSubjects.push_back(subjectChoice);
                                        }
                                    }
                                }
                            }
                            break;
                            case 2: {
                                while (true)
                                {
                                    system("cls");
                                    vector<string> subjects = {"1. Entrepreneurship Introduction",
                                                               "2. Leadership Development", "3. Public Policy",
                                                               "4. Go Back"};
                                    cout << "\nPlease select a Core - Challenge subject (multiple selection allowed, "
                                            "press ESC to go back):\n";
                                    for (const auto &subject : subjects)
                                    {
                                        cout << subject << "\n";
                                    }
                                    cout << "Currently selected subjects: ";
                                    for (int subject : selectedCoreChallengeSubjects)
                                    {
                                        cout << subject << " ";
                                    }
                                    cout << "\n> ";
                                    int subjectChoice;
                                    cin >> subjectChoice;

                                    if (checkForEsc() || subjectChoice == 4)
                                    {
                                        break;
                                    }

                                    if (cin.fail() || subjectChoice < 1 || subjectChoice > subjects.size() - 1)
                                    {
                                        cin.clear();
                                        cin.ignore(1000, '\n');
                                        cout << "Invalid input. Please try again." << endl;
                                    }
                                    else
                                    {
                                        if (find(selectedCoreChallengeSubjects.begin(),
                                                 selectedCoreChallengeSubjects.end(),
                                                 subjectChoice) != selectedCoreChallengeSubjects.end())
                                        {
                                            cout << "Subject already selected." << endl;
                                        }
                                        else
                                        {
                                            selectedCoreChallengeSubjects.push_back(subjectChoice);
                                        }
                                    }
                                }
                            }
                            break;
                            case 3: {
                                // Select Core - Creativity Subjects (To be implemented)
                            }
                            break;
                            }
                        }
                    }
                    else if (category == 3)
                    {
                        cout << "Proceeding to the next step." << endl;
                        break;
                    }
                }
                break;
            }
            case 8: {
                cout << "\nCase 8: Additional Information Collection." << endl;
                int englishA = 1;
                vector<string> options = {"1. Yes", "2. No"};
                while (true)
                {
                    system("cls");
                    cout << "\nWould you like to select the English A course?\n";
                    for (int i = 0; i < options.size(); ++i)
                    {
                        if (i + 1 == englishA)
                        {
                            cout << "> " << options[i] << endl;
                        }
                        else
                        {
                            cout << "  " << options[i] << endl;
                        }
                    }

                    char key = _getch();
                    if (key == 13)
                    {
                        break;
                    }
                    else if (key == 72)
                    {
                        englishA = (englishA == 1) ? options.size() : englishA - 1;
                    }
                    else if (key == 80)
                    {
                        englishA = (englishA == options.size()) ? 1 : englishA + 1;
                    }
                }
            }
            break;
            case 9: {
                cout << "\nCase 9: Schedule Verification." << endl;
                vector<Weekday> selectedDays;
                vector<int> selectedPeriods;

                while (true)
                {
                    system("cls");
                    cout << "\nPlease set your free days:\n1. Mon\n2. Tue\n3. Wed\n4. Thu\n5. Fri\n0. Done\n ";
                    cout << "\nCurrently selected days: ";
                    for (const auto &day : selectedDays)
                    {
                        cout << static_cast<int>(day) + 1 << " ";
                    }
                    cout << "\n> ";

                    int day;
                    cin >> day;
                    if (day == 0)
                    {
                        break;
                    }
                    while (cin.fail() || day < 1 || day > 5 ||
                           find(selectedDays.begin(), selectedDays.end(), static_cast<Weekday>(day - 1)) !=
                               selectedDays.end())
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Duplicate selection. Please choose a different number: ";
                        cin >> day;
                    }
                    selectedDays.push_back(static_cast<Weekday>(day - 1));
                }

                while (true)
                {
                    system("cls");
                    cout << "\nPlease set your free periods:\n1. Period 1\n2. Period 2\n3. Period 3\n4. Period 4\n5. "
                            "Period 5\n6. Period 6\n7. Period 7\n8. Period 8\n9. Period 9\n10. Period 10\n11. Period "
                            "11\n12. Period 12\n0. Done\n ";
                    cout << "\nCurrently selected periods: ";
                    for (const auto &period : selectedPeriods)
                    {
                        cout << period << " ";
                    }
                    cout << "\n> ";

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
                        cout << "Duplicate selection. Please choose a different number: ";
                        cin >> period;
                    }
                    selectedPeriods.push_back(period);
                }
            }
            break;
            case 10: {
                cout << "\nCase 10: Final Confirmation." << endl;
                int scheduleID = rand() % 9000 + 1000;
                cout << "\nSaving the schedule...\n";
                cout << "The schedule has been saved. Schedule ID: " << scheduleID << endl;
            }
            break;
            default:
                cout << "Invalid step." << endl;
                break;
            }
            currentStep++;
        }
        catch (const runtime_error &e)
        {
            cout << "An error occurred: " << e.what() << endl;
        }
    }
}

void modifySchedule()
{
    try
    {
        system("cls");
        cout << "\nSearch and Modify Schedule" << endl;
        cout << "Enter the necessary information to view the schedules to search." << endl;

        string userName = getInput("User Name (leave blank for all searches): ");
        int year = getOptionalIntInput("Year (1-4, leave blank for all searches): ", 1, 4);
        Semester semester = getOptionalSemesterInput(
            "Semester (1: Spring, 2: Summer, 3: Fall, 4: Winter, leave blank for all searches): ");
        int scheduleID = getOptionalIntInput("Schedule ID (leave blank for all searches): ", 0, 9999);

        vector<Table> foundSchedules =
            tableDatabase.query({to_string(scheduleID), semester, year == -1 ? "" : to_string(year), "", userName});

        if (foundSchedules.empty())
        {
            cout << "No schedules found matching the criteria. Please try again." << endl;
            waitForEnterOrEsc();
            return;
        }

        cout << "\nList of Found Schedules:" << endl;
        for (size_t i = 0; i < foundSchedules.size(); ++i)
        {
            cout << i + 1 << ") Schedule ID: " << foundSchedules[i].get_id()
                 << ", Year: " << foundSchedules[i].get_year()
                 << ", Semester: " << encode_semester(foundSchedules[i].get_semester())
                 << ", User Name: " << foundSchedules[i].get_user_id() << endl;
        }

        int selectedScheduleIndex =
            getOptionalIntInput("Select a schedule to modify (number): ", 1, foundSchedules.size()) - 1;
        modifySelectedSchedule(foundSchedules[selectedScheduleIndex].get_id());
    }
    catch (runtime_error &)
    {
        cout << "Schedule modification cancelled." << endl;
    }
}

string getInput(const string &prompt)
{
    cout << prompt;
    string input;
    getline(cin, input);
    return input;
}

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
            cout << "Input value is out of range." << endl;
            return getOptionalIntInput(prompt, min, max);
        }
    }
    catch (...)
    {
        cout << "Please enter a valid number." << endl;
        return getOptionalIntInput(prompt, min, max);
    }
}

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
            cout << "Please enter a valid semester." << endl;
            return getOptionalSemesterInput(prompt);
        }
    }
    catch (...)
    {
        cout << "Please enter a valid semester." << endl;
        return getOptionalSemesterInput(prompt);
    }
}

void modifySelectedSchedule(int scheduleID)
{
    Table *selectedSchedule = &tableDatabase.get_tables()[scheduleID - 1];
    if (!selectedSchedule)
    {
        cout << "Schedule not found." << endl;
        return;
    }
    displayLectureList(*selectedSchedule);

    int editChoice = 1;

    cout << " \n1. Set as Main Schedule\n2. Delete Schedule\n3. Modify Schedule\n4. Return to Menu " << endl;
    cin >> editChoice;

    switch (editChoice)
    {
    case 1:
        // setMainSchedule(scheduleID);
        cout << "Set as main schedule." << endl;
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
        cout << "Invalid input." << endl;
        break;
    }
}

void deleteSchedule(int scheduleID)
{
    try
    {
        tableDatabase.remove(scheduleID);
        cout << "Schedule has been deleted." << endl;
    }
    catch (const std::exception &e)
    {
        cout << "Failed to delete schedule: " << e.what() << endl;
    }
    waitForEnterOrEsc();
}

void modifyExistingSchedule(Table &selectedSchedule)
{
    while (true)
    {
        int modifyChoice = 1;
        cout << "\n1. Add Lecture\n2. Delete Lecture\n3. Exit" << endl;
        cin >> modifyChoice;

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
            cout << "Invalid input." << endl;
            break;
        }
    }
}

void addLectureToSchedule(Table &schedule)
{
    cout << "Enter the lecture information to add." << endl;
    string lectureName = getInput("Lecture Name: ");
    string professorName = getInput("Professor Name: ");
    int credits = getOptionalIntInput("Credits (1-5): ", 1, 5);
    int lectureDay =
        getOptionalIntInput("Lecture Day (1: Monday, 2: Tuesday, 3: Wednesday, 4: Thursday, 5: Friday): ", 1, 5);
    int lecturePeriod = getOptionalIntInput("Lecture Period (1-12): ", 1, 12);

    vector<CourseTime> times = {CourseTime{static_cast<Weekday>(lectureDay - 1), lecturePeriod}};
    string encodedCourseInfo = lectureName + "|" + professorName + "|" + to_string(credits);
    Course newCourse(encodedCourseInfo);

    if (isLectureAlreadyAdded(schedule, newCourse))
    {
        cout << "The lecture is already added." << endl;
    }
    else if (isTimeConflict(schedule, newCourse))
    {
        cout << "There is a time conflict in the schedule." << endl;
    }
    else
    {
        schedule.insert_course(newCourse);
        cout << "Lecture added successfully." << endl;
    }
    waitForEnterOrEsc();
}

bool isLectureAlreadyAdded(const Table &schedule, const Course &newCourse)
{
    for (const auto &course : schedule.get_course())
    {
        if (course.get_name() == newCourse.get_name() && course.get_professor() == newCourse.get_professor())
        {
            return true;
        }
    }
    return false;
}

bool isTimeConflict(const Table &schedule, const Course &newCourse)
{
    for (const auto &course : schedule.get_course())
    {
        for (const auto &newTime : newCourse.get_times())
        {
            for (const auto &existingTime : course.get_times())
            {
                if (newTime.weekday == existingTime.weekday && newTime.time == existingTime.time)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void deleteLectureFromSchedule(Table &schedule)
{
    cout << "Enter the name of the lecture to delete." << endl;
    string lectureName = getInput("Lecture Name: ");
    Course *course = nullptr;
    for (Course &c : schedule.get_course())
    {
        if (c.get_name() == lectureName)
        {
            course = &c;
            break;
        }
    }
    if (!course)
    {
        cout << "Lecture not found." << endl;
    }
    else
    {
        schedule.remove_course(*course);
        cout << "Lecture deleted successfully." << endl;
    }
    waitForEnterOrEsc();
}

void displayLectureList(const Table &table)
{
    cout << "\nCurrent lecture list of the selected schedule (ID: " << table.get_id() << "):" << endl;
    cout << "No. | Lecture Name   | Professor   | Credits | Day/Period" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    const auto &courses = table.get_course();
    for (size_t i = 0; i < courses.size(); ++i)
    {
        const Course &course = courses[i];
        cout << setw(4) << (i + 1) << " | " << setw(12) << course.get_name() << " | " << setw(10)
             << course.get_professor() << " | " << setw(4) << course.get_grade() << " | ";
        for (const auto &time : course.get_times())
        {
            cout << encode_weekday(time.weekday) << "/" << time.time << "  ";
        }
        cout << endl;
    }
    waitForEnterOrEsc();
}