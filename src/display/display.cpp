#include "display.h"
#include "user.h"
#include "schedule_management.h"
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <algorithm>

User current_user;

bool checkForEsc() {
    if (_kbhit()) {
        char key = _getch();
        if (key == 27) {
            return true;
        }
    }
    return false;
}

void waitForEnterOrEsc() {
    cout << "\n(Enter 키를 눌러 완료하거나 ESC 키를 눌러 뒤로 갈 수 있습니다.)" << endl;
    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 13) {
                break;
            }
            else if (key == 27) {
                throw runtime_error("뒤로가기");
            }
        }
    }
}

void userSettings() {
    try {
        system("cls");
        cout << "\n사용자 설정" << endl;
        cout << "이름: ";
        cin >> current_user.name;
        while (current_user.name.empty()) {
            cout << "이름이 올바르지 않습니다. 다시 입력해주세요: ";
            cin >> current_user.name;
        }
        cout << "학번: ";
        cin >> current_user.id;
        while (cin.fail() || to_string(current_user.id).length() != 8) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "학번이 올바르지 않습니다. 숫자 8자리 형식으로 입력해주세요: ";
            cin >> current_user.id;
        }
        cout << "학년 (1-4): ";
        cin >> current_user.year;
        while (current_user.year < 1 || current_user.year > 4) {
            cout << "학년은 1에서 4 사이의 숫자로 입력해주세요: ";
            cin >> current_user.year;
        }
        cout << "학과: ";
        cin >> current_user.department;
        vector<string> validDepartments = { "소프트웨어", "기타 학과" };
        while (find(validDepartments.begin(), validDepartments.end(), current_user.department) == validDepartments.end()) {
            cout << "학과명이 올바르지 않습니다. 다시 입력해주세요: ";
            cin >> current_user.department;
        }
        cout << "사용자 정보가 저장되었습니다.\n" << endl;
        waitForEnterOrEsc();
    }
    catch (runtime_error&) {
        cout << "사용자 설정을 취소하였습니다.\n" << endl;
    }
}

void displayScheduleTable(const Table& table) {
    system("cls");
    cout << "\n==================================================================================" << endl;
    cout << "교시 |      월      |      화      |      수      |      목      |      금      |" << endl;
    cout << "====================================================================================" << endl;
    for (int i = 1; i <= 12; ++i) {
        cout << i << "교시 | ";
        for (const Weekday& day : { Weekday::Mon, Weekday::Tue, Weekday::Wed, Weekday::Thu, Weekday::Fri }) {
            bool slotFound = false;
            for (const auto& course : table.get_course()) {
                for (const auto& time : course.get_times()) {
                    if (time.weekday == day && time.time == i) {
                        cout << left << setw(12) << (course.get_name() + "/" + course.get_professor()) << " | ";
                        slotFound = true;
                        break;
                    }
                }
                if (slotFound) break;
            }
            if (!slotFound) {
                cout << left << setw(12) << "" << " | ";
            }
        }
        cout << "\n------------------------------------------------------------------------------------" << endl;
    }
}

void displayScheduleMenu() {
    vector<string> options = { "1. 시간표 생성", "2. 시간표 검색 및 수정", "3. 메인 메뉴로 돌아가기" };
    while (true) {
        int subChoice = navigateMenu(options);
        switch (subChoice) {
        case 1:
            generateSchedule();
            break;
        case 2:
            modifySchedule();
            break;
        case 3:
            return;
        default:
            cout << "잘못된 입력입니다." << endl;
            break;
        }
    }
}


void generateSchedule() {
    srand(time(0));
    int currentStep = 1;
    int totalSteps = 10;
    while (currentStep <= totalSteps) {
        try {
            system("cls");
            cout << "\n시간표 생성 단계 " << currentStep << "/" << totalSteps << endl;
            switch (currentStep) {
            case 1: {
                cout << "생성할 시간표의 년도를 입력해주세요 (예: 2023, 2024): ";
                string year;
                cin >> year;
                while (cin.fail() || year.length() != 4 || !isdigit(year[0])) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "잘못된 입력입니다. 숫자 4자리로 입력해주세요 (예: 2023, 2024): ";
                    cin >> year;
                }
                break;
            }
            case 2: {
                cout << "\n학년을 선택해주세요:\n1. 1학년\n2. 2학년\n3. 3학년\n4. 4학년\n> ";
                int year;
                cin >> year;
                while (cin.fail() || year < 1 || year > 4) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "잘못된 입력입니다. 1에서 4 사이의 번호를 선택해주세요: ";
                    cin >> year;
                }
                break;
            }
            case 3: {
                cout << "\n학기를 선택해주세요:\n1. 봄 (1학기)\n2. 여름 (계절학기)\n3. 가을 (2학기)\n4. 겨울 (계절학기)\n> ";
                int semester;
                cin >> semester;
                while (cin.fail() || semester < 1 || semester > 4) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "잘못된 입력입니다. 1에서 4 사이의 번호를 선택해주세요: ";
                    cin >> semester;
                }
                break;
            }
            case 4: {
                cout << "\n학과를 선택해주세요:\n1. 소프트웨어 학과\n2. 기타 학과\n> ";
                int department;
                cin >> department;
                while (cin.fail() || department < 1 || department > 2) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "잘못된 입력입니다. 1 또는 2를 선택해주세요: ";
                    cin >> department;
                }
                break;
            }
            case 5: {
                cout << "\n이번 학기에 수강할 학점 수를 입력해주세요 (최대 24학점): ";
                int maxCredits;
                cin >> maxCredits;
                while (cin.fail() || maxCredits < 1 || maxCredits > 24) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "잘못된 입력입니다. 1에서 24 사이의 학점을 입력해주세요: ";
                    cin >> maxCredits;
                }
                break;
            }
            case 6: {
                cout << "\n교수님 우선순위를 선택해주세요:\n1. 교수님 A\n2. 교수님 B\n3. 교수님 C\n> ";
                int professor;
                cin >> professor;
                while (cin.fail() || professor < 1 || professor > 3) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "잘못된 입력입니다. 1에서 3 사이의 번호를 선택해주세요: ";
                    cin >> professor;
                }
                break;
            }
            case 7: {
                vector<string> categories = { "1. 전공", "2. 교양", "3. 다음단계" };
                vector<int> selectedMajorBasicSubjects;
                vector<int> selectedMajorSubjects;
                vector<int> selectedMajorRequiredSubjects;
                vector<int> selectedGeneralBasicSubjects;
                vector<int> selectedCoreChallengeSubjects;
                vector<int> selectedCoreCreativitySubjects;
                vector<int> selectedCoreConvergenceSubjects;
                vector<int> selectedCoreTrustSubjects;
                vector<int> selectedCoreCommunicationSubjects;
                vector<int> selectedChoiceChallengeSubjects;
                vector<int> selectedChoiceCreativitySubjects;
                vector<int> selectedChoiceConvergenceSubjects;
                vector<int> selectedChoiceTrustSubjects;
                vector<int> selectedChoiceCommunicationSubjects;

                while (true) {
                    int category = navigateMenu(categories);
                    if (category == 1) {
                        vector<string> majorOptions = { "1. 전공기초", "2. 전공", "3. 전공필수", "4. 돌아가기" };
                        while (true) {
                            int majorChoice = navigateMenu(majorOptions);
                            if (majorChoice == 4) {
                                break;
                            }
                            switch (majorChoice) {
                            case 1: {
                                while (true) {
                                    system("cls");
                                    vector<string> subjects = { "1. 자료구조", "2. 알고리즘", "3. 프로그래밍 기초", "4. 돌아가기" };
                                    cout << "\n전공기초 과목을 선택해주세요 (중복 가능, ESC로 뒤로가기):\n";
                                    for (const auto& subject : subjects) {
                                        cout << subject << "\n";
                                    }
                                    cout << "현재 선택된 과목 번호: ";
                                    for (int subject : selectedMajorBasicSubjects) {
                                        cout << subject << " ";
                                    }
                                    cout << "\n> ";
                                    int subjectChoice;
                                    cin >> subjectChoice;

                                    if (checkForEsc() || subjectChoice == 4) {
                                        break;
                                    }

                                    if (cin.fail() || subjectChoice < 1 || subjectChoice > subjects.size() - 1) {
                                        cin.clear();
                                        cin.ignore(1000, '\n');
                                        cout << "잘못된 입력입니다. 다시 시도해주세요." << endl;
                                    }
                                    else {
                                        if (find(selectedMajorBasicSubjects.begin(), selectedMajorBasicSubjects.end(), subjectChoice) != selectedMajorBasicSubjects.end()) {
                                            cout << "이미 추가된 과목입니다." << endl;
                                        }
                                        else {
                                            selectedMajorBasicSubjects.push_back(subjectChoice);
                                        }
                                    }
                                }
                                break;
                            }
                            case 2: {
                                while (true) {
                                    system("cls");
                                    vector<string> subjects = { "1. 운영체제", "2. 네트워크", "3. 데이터베이스", "4. 돌아가기" };
                                    cout << "\n전공 과목을 선택해주세요 (중복 가능, ESC로 뒤로가기):\n";
                                    for (const auto& subject : subjects) {
                                        cout << subject << "\n";
                                    }
                                    cout << "현재 선택된 과목 번호: ";
                                    for (int subject : selectedMajorSubjects) {
                                        cout << subject << " ";
                                    }
                                    cout << "\n> ";
                                    int subjectChoice;
                                    cin >> subjectChoice;

                                    if (checkForEsc() || subjectChoice == 4) {
                                        break;
                                    }

                                    if (cin.fail() || subjectChoice < 1 || subjectChoice > subjects.size() - 1) {
                                        cin.clear();
                                        cin.ignore(1000, '\n');
                                        cout << "잘못된 입력입니다. 다시 시도해주세요." << endl;
                                    }
                                    else {
                                        if (find(selectedMajorSubjects.begin(), selectedMajorSubjects.end(), subjectChoice) != selectedMajorSubjects.end()) {
                                            cout << "이미 추가된 과목입니다." << endl;
                                        }
                                        else {
                                            selectedMajorSubjects.push_back(subjectChoice);
                                        }
                                    }
                                }
                                break;
                            }
                            case 3: {
                                while (true) {
                                    system("cls");
                                    vector<string> subjects = { "1. 소프트웨어 공학", "2. 컴퓨터 구조", "3. 컴퓨터 비전", "4. 돌아가기" };
                                    cout << "\n전공필수 과목을 선택해주세요 (중복 가능, ESC로 뒤로가기):\n";
                                    for (const auto& subject : subjects) {
                                        cout << subject << "\n";
                                    }
                                    cout << "현재 선택된 과목 번호: ";
                                    for (int subject : selectedMajorRequiredSubjects) {
                                        cout << subject << " ";
                                    }
                                    cout << "\n> ";
                                    int subjectChoice;
                                    cin >> subjectChoice;

                                    if (checkForEsc() || subjectChoice == 4) {
                                        break;
                                    }

                                    if (cin.fail() || subjectChoice < 1 || subjectChoice > subjects.size() - 1) {
                                        cin.clear();
                                        cin.ignore(1000, '\n');
                                        cout << "잘못된 입력입니다. 다시 시도해주세요." << endl;
                                    }
                                    else {
                                        if (find(selectedMajorRequiredSubjects.begin(), selectedMajorRequiredSubjects.end(), subjectChoice) != selectedMajorRequiredSubjects.end()) {
                                            cout << "이미 추가된 과목입니다." << endl;
                                        }
                                        else {
                                            selectedMajorRequiredSubjects.push_back(subjectChoice);
                                        }
                                    }
                                }
                                break;
                            }
                            }
                        }
                    }
                    else if (category == 2) {
                        vector<string> generalOptions = { "1. 일반교양", "2. 핵심-도전", "3. 핵심-창의", "4. 핵심-융합", "5. 핵심-신뢰", "6. 핵심-소통", "7. 선택-도전", "8. 선택-창의", "9. 선택-융합", "10. 선택-신뢰", "11. 선택-소통", "12. 돌아가기" };
                        while (true) {
                            int generalChoice = navigateMenu(generalOptions);
                            if (generalChoice == 12) {
                                break;
                            }
                            switch (generalChoice) {
                            case 1: {
                                while (true) {
                                    system("cls");
                                    vector<string> subjects = { "1. 철학", "2. 심리학", "3. 예술사", "4. 돌아가기" };
                                    cout << "\n일반교양 과목을 선택해주세요 (중복 가능, ESC로 뒤로가기):\n";
                                    for (const auto& subject : subjects) {
                                        cout << subject << "\n";
                                    }
                                    cout << "현재 선택된 과목 번호: ";
                                    for (int subject : selectedGeneralBasicSubjects) {
                                        cout << subject << " ";
                                    }
                                    cout << "\n> ";
                                    int subjectChoice;
                                    cin >> subjectChoice;

                                    if (checkForEsc() || subjectChoice == 4) {
                                        break;
                                    }

                                    if (cin.fail() || subjectChoice < 1 || subjectChoice > subjects.size() - 1) {
                                        cin.clear();
                                        cin.ignore(1000, '\n');
                                        cout << "잘못된 입력입니다. 다시 시도해주세요." << endl;
                                    }
                                    else {
                                        if (find(selectedGeneralBasicSubjects.begin(), selectedGeneralBasicSubjects.end(), subjectChoice) != selectedGeneralBasicSubjects.end()) {
                                            cout << "이미 추가된 과목입니다." << endl;
                                        }
                                        else {
                                            selectedGeneralBasicSubjects.push_back(subjectChoice);
                                        }
                                    }
                                }
                                break;
                            }
                            }
                                }
                    }
                    else if (category == 3) {
                        cout << "다음 단계로 이동합니다." << endl;
                        break;
                    }
                }
                break;
            }

            case 8: {
                int englishA = 1;
                vector<string> options = { "1. 예", "2. 아니오" };
                while (true) {
                    system("cls");
                    cout << "\n영어 A 과목을 선택하시겠습니까?\n";
                    for (int i = 0; i < options.size(); ++i) {
                        if (i + 1 == englishA) {
                            cout << "> " << options[i] << endl;
                        }
                        else {
                            cout << "  " << options[i] << endl;
                        }
                    }

                    char key = _getch();
                    if (key == 13) {
                        break;
                    }
                    else if (key == 72) {
                        englishA = (englishA == 1) ? options.size() : englishA - 1;
                    }
                    else if (key == 80) {
                        englishA = (englishA == options.size()) ? 1 : englishA + 1;
                    }
                }
                break;
            }

            case 9: {
                cout << "\n공강 시간을 설정해주세요:\n";
                vector<Weekday> selectedDays;
                vector<int> selectedPeriods;

                // 요일 선택
                while (true) {
                    system("cls");
                    cout << "\n공강 시간을 설정해주세요 (중복 선택 불가):\n"
                        << "1. 월\n2. 화\n3. 수\n4. 목\n5. 금\n0. 완료\n";
                    cout << "\n현재 선택된 요일 번호: ";
                    for (const auto& day : selectedDays) {
                        cout << static_cast<int>(day) + 1 << " ";  // 저장된 번호는 0부터 시작하므로 +1
                    }
                    cout << "\n> ";

                    int day;
                    cin >> day;
                    if (cin.fail() || (day < 0 || day > 5)) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "잘못된 입력입니다. 1에서 5 사이의 번호를 선택하거나 0을 입력해 완료하세요." << endl;
                        continue;
                    }
                    if (day == 0) { // 완료
                        break;
                    }

                    Weekday selectedDay = static_cast<Weekday>(day - 1);
                    if (find(selectedDays.begin(), selectedDays.end(), selectedDay) != selectedDays.end()) {
                        cout << "이미 선택된 요일입니다. 다른 요일을 선택하세요." << endl;
                        continue;
                    }
                    selectedDays.push_back(selectedDay);
                }

                // 교시 선택
                while (true) {
                    system("cls");
                    cout << "\n교시를 선택해주세요 (중복 선택 불가):\n"
                        << "1. 1교시(09:00~10:00)\n2. 2교시(10:00~11:00)\n3. 3교시(11:00~12:00)\n"
                        << "4. 4교시(12:00~13:00)\n5. 5교시(13:00~14:00)\n6. 6교시(14:00~15:00)\n"
                        << "7. 7교시(15:00~16:00)\n8. 8교시(16:00~17:00)\n9. 9교시(17:00~18:00)\n"
                        << "10. 10교시(18:00~19:00)\n11. 11교시(19:00~20:00)\n12. 12교시(20:00~21:00)\n0. 완료\n";
                    cout << "\n현재 선택된 교시 번호: ";
                    for (const auto& period : selectedPeriods) {
                        cout << period << " ";
                    }
                    cout << "\n> ";

                    int period;
                    cin >> period;
                    if (cin.fail() || (period < 0 || period > 12)) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "잘못된 입력입니다. 1에서 12 사이의 번호를 선택하거나 0을 입력해 완료하세요." << endl;
                        continue;
                    }
                    if (period == 0) { // 완료
                        break;
                    }

                    if (find(selectedPeriods.begin(), selectedPeriods.end(), period) != selectedPeriods.end()) {
                        cout << "이미 선택된 교시입니다. 다른 교시를 선택하세요." << endl;
                        continue;
                    }
                    selectedPeriods.push_back(period);
                }

                break;
            }

            case 10: {
                int scheduleID = rand() % 1000; // 0 ~ 999 사이의 랜덤 ID 생성
                cout << "\n시간표 저장 중...\n";
                cout << "시간표가 저장되었습니다. 시간표 ID: " << scheduleID << endl;
                break;
            }
            }
            currentStep++;
        }
        catch (runtime_error&) {
            cout << "\n시간표 메뉴로 돌아갑니다...\n";
            return;
        }
    }
    cout << "\n시간표 생성이 완료되었습니다.\n";
    waitForEnterOrEsc();
}   

void modifySchedule() {
    try {
        system("cls");
        cout << "\n시간표 검색 및 수정" << endl;
        cout << "필요한 정보만을 입력 후, 검색할 시간표를 확인하세요." << endl;

        string userName = getInput("사용자 이름 (입력하지 않으면 전체 검색): ");
        int year = getOptionalIntInput("학년 (1-4, 입력하지 않으면 전체 검색): ", 1, 4);
        Semester semester = getOptionalSemesterInput("학기 (1: 봄, 2: 여름, 3: 가을, 4: 겨울, 입력하지 않으면 전체 검색): ");
        int scheduleID = getOptionalIntInput("시간표 ID (입력하지 않으면 전체 검색): ", 0, 9999);

        TableQuery query = {scheduleID == -1 ? "" : to_string(scheduleID), semester, year == -1 ? "" : to_string(year), "", userName};
        vector<Table> foundSchedules = tableDatabase.query(query);
        if (foundSchedules.empty()) {
            cout << "조건에 맞는 시간표가 없습니다. 다시 검색해주세요." << endl;
            waitForEnterOrEsc();
            return;
        }

        vector<int> scheduleIDs;
        for (const Table& table : foundSchedules) {
            scheduleIDs.push_back(table.get_id());
        }

        int selectedScheduleIndex = navigateSchedules(scheduleIDs);
        if (selectedScheduleIndex == -1) {
            return; 
        }

        modifySelectedSchedule(scheduleIDs[selectedScheduleIndex]);
    } catch (runtime_error&) {
        cout << "시간표 수정을 취소하였습니다.\n" << endl;
    }
}

string getInput(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin, input);
    return input;
}

int getOptionalIntInput(const string& prompt, int min, int max) {
    string input = getInput(prompt);
    if (input.empty()) {
        return -1; 
    }
    try {
        int value = stoi(input);
        if (value >= min && value <= max) {
            return value;
        } else {
            cout << "입력값이 범위를 벗어났습니다." << endl;
            return getOptionalIntInput(prompt, min, max); 
        }
    } catch (...) {
        cout << "올바른 숫자를 입력해주세요." << endl;
        return getOptionalIntInput(prompt, min, max);  
    }
}

Semester getOptionalSemesterInput(const string& prompt) {
    string input = getInput(prompt);
    if (input.empty()) {
        return static_cast<Semester>(-1);  
    }
    try {
        int value = stoi(input);
        if (value >= 1 && value <= 4) {
            return static_cast<Semester>(value - 1);
        } else {
            cout << "올바른 학기를 입력해주세요." << endl;
            return getOptionalSemesterInput(prompt);
        }
    } catch (...) {
        cout << "올바른 학기를 입력해주세요." << endl;
        return getOptionalSemesterInput(prompt);
    }
}

void modifySelectedSchedule(int scheduleID) {
    vector<Table> tables = tableDatabase.get_tables();
    auto it = find(tables.begin(), tables.end(), [scheduleID](const Table& table) { return table.get_id() == scheduleID; });
    if (it == tables.end()) {
        cout << "시간표를 찾을 수 없습니다." << endl;
        return;
    }
    Table& selectedSchedule = *it;
    displayScheduleInfo(selectedSchedule);

    vector<string> editOptions = { "1. 메인 시간표 지정", "2. 시간표 삭제", "3. 시간표 수정", "4. 뒤로가기" };
    int editChoice = navigateMenu(editOptions);

    switch (editChoice) {
    case 1:
        setMainSchedule(scheduleID);
        cout << "메인 시간표로 지정되었습니다." << endl;
        break;
    case 2:
        deleteSchedule(scheduleID);
        break;
    case 3:
        modifyExistingSchedule(selectedSchedule);
        break;
    case 4:
        return;
    default:
        cout << "잘못된 입력입니다." << endl;
        break;
    }
}

void deleteSchedule(int scheduleID) {
    char confirm;
    cout << "정말로 이 시간표를 삭제하시겠습니까? (Y/N): ";
    cin >> confirm;
    if (toupper(confirm) == 'Y') {
        tableDatabase.remove(scheduleID);
        cout << "시간표가 삭제되었습니다." << endl;
    } else {
        cout << "삭제가 취소되었습니다." << endl;
    }
}

void modifyExistingSchedule(Table& selectedSchedule) {
    system("cls");
    vector<string> modifyOptions = { "1. 강의 추가", "2. 강의 삭제", "3. 수정 종료" };
    while (true) {
        int modifyChoice = navigateMenu(modifyOptions);
        switch (modifyChoice) {
        case 1:
            addLectureToSchedule(selectedSchedule);
            break;
        case 2:
            deleteLectureFromSchedule(selectedSchedule);
            break;
        case 3:
            cout << "수정을 종료하고 메인 화면으로 돌아갑니다." << endl;
            return;
        default:
            cout << "잘못된 입력입니다." << endl;
            break;
        }
        waitForEnterOrEsc();
    }
}

void addLectureToSchedule(Table& schedule) {
    while (true) {
        system("cls");
        cout << "강의 검색을 위한 정보를 입력하세요." << endl;
        string dayInput = getInput("시간대(요일): (입력하지 않으면 전체 검색) ");
        string periodInput = getInput("시간대(교시): (입력하지 않으면 전체 검색) ");
        string courseName = getInput("강의명: (입력하지 않으면 전체 검색) ");
        string professorName = getInput("교수명: (입력하지 않으면 전체 검색) ");

        CourseQuery query = {schedule.get_semester(), schedule.get_year(), {}, {}, courseName, {}, {professorName}};
        vector<Course> foundLectures = courseDatabase.query(query);
        if (foundLectures.empty()) {
            cout << "조건에 맞는 강의가 없습니다. 검색 조건을 다시 확인해주세요." << endl;
            waitForEnterOrEsc();
            continue;
        }

        int selectedLectureIndex = navigateLectures(foundLectures);
        if (selectedLectureIndex == -1) {
            return; 
        }

        Course selectedLecture = foundLectures[selectedLectureIndex];
        if (isLectureAlreadyAdded(schedule, selectedLecture)) {
            cout << "선택한 강의가 이미 시간표에 포함되어 있습니다." << endl;
            waitForEnterOrEsc();
            continue;
        }
        if (isCreditLimitExceeded(schedule, selectedLecture)) {
            cout << "학점 초과로 강의를 추가할 수 없습니다. 현재 학점: " << getCurrentCredits(schedule) << ", 최대 학점: " << getMaxCredits(schedule) << endl;
            waitForEnterOrEsc();
            continue;
        }
        if (isTimeConflict(schedule, selectedLecture)) {
            cout << "선택한 강의가 시간표의 다른 강의와 시간이 겹칩니다. 확인 후 다시 시도해주세요." << endl;
            waitForEnterOrEsc();
            continue;
        }

        schedule.insert_course(selectedLecture);
        cout << "강의가 추가되었습니다." << endl;
        displayCurrentSchedule(schedule);
        waitForEnterOrEsc();
    }
}

void deleteLectureFromSchedule(Table& schedule) {
    system("cls");
    cout << "선택한 시간표(ID: " << schedule.get_id() << ")의 현재 강의 목록입니다." << endl;
    displayLectureList(schedule);
    cout << "삭제할 강의의 해당 번호를 입력해주세요. (중복가능)" << endl;

    vector<int> lectureNumbers;
    cout << "삭제할 강의 번호들을 공백으로 구분하여 입력하세요: ";
    string input;
    getline(cin, input);
    size_t pos = 0;
    while ((pos = input.find(' ')) != string::npos) {
        lectureNumbers.push_back(stoi(input.substr(0, pos)));
        input.erase(0, pos + 1);
    }
    if (!input.empty()) {
        lectureNumbers.push_back(stoi(input));
    }

    char confirm;
    cout << "정말로 선택한 강의들을 삭제하시겠습니까? (Y/N): ";
    cin >> confirm;
    if (toupper(confirm) == 'Y') {
        for (int lectureNumber : lectureNumbers) {
            if (lectureNumber >= 0 && lectureNumber < schedule.get_course().size()) {
                Course course = schedule.get_course().at(lectureNumber);
                schedule.remove_course(course);
            }
        }
        cout << "선택한 강의가 삭제되었습니다." << endl;
        displayCurrentSchedule(schedule);
    } else {
        cout << "강의 삭제가 취소되었습니다." << endl;
    }
    waitForEnterOrEsc();
}
