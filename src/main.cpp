#include "course_db.h"
#include "display.h"
#include "schedule_management.h"
#include <iostream>
#include <string>
#include "table_db.h"

using namespace std;

int main()
{
     std::cout << "Hello World!" << std::endl;
    TableDatabase db;
    Table table("<table><id>101</id><name>Park</name><user_id>20231234</user_id><year>2</year><Semester>Fall</Semester><course><id>10000</id><name>OOP</name><professor>Kim</professor><type>10</type><semester>2</semester><english>1</english><professor>string</professor><department>integer</department><department>integer</department><time><weekday>integer</weekday><t>3</t><room>727</room><building>310</building></time><time><weekday>4</weekday><t>4</t><room>728</room><building>310</building></time></course></table>");
    db.insert(table);
    db.save();
    db.load();
    vector<string> mainOptions = {"1. 사용자 설정", "2. 시간표", "3. 종료"};
    while (true) {
        try {
            int choice = navigateMenu(mainOptions);

            switch (choice) {
            case 1:
                userSettings();
                break;
            case 2:
                displayScheduleMenu();
                break;
            case 3:
                cout << "프로그램을 종료합니다." << endl;
                return 0;
            default:
                cout << "잘못된 입력입니다. 메뉴에서 제시된 번호를 선택해주세요." << endl;
                break;
            }
        }
        catch (runtime_error&) {
            cout << "메뉴로 돌아가습니다.\n" << endl;
        }
    }
    return 0;
}