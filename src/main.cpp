#include "course_db.h"
#include "display.h"
#include "schedule_management.h"
#include "table_db.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    while (true)
    {
        system("cls");
        cout << "\n==================================================================================" << endl;
        cout << "[Chung-Ang University Schedule Management System]" << endl;
        cout << "==================================================================================" << endl;
        cout << " 1. User Settings \n 2. Schedule \n 3. Exit " << endl;
        cout << "==================================================================================" << endl;

        int choice;
        cout << " Please select a number from the menu: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please try again." << endl;
            continue;
        }

        switch (choice)
        {
        case 1:
            userSettings();
            break;
        case 2:
            displayScheduleMenu();
            break;
        case 3:
            cout << "Exiting the program." << endl;
            return 0;
        default:
            cout << "Invalid input. Please select a number from the menu." << endl;
            break;
        }
    }

    return 0;
}
