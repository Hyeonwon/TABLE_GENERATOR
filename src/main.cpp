#include "course_db.h"
#include "display.h"
#include "schedule_management.h"
#include "table_db.h"
#include <conio.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
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
                displayScheduleMenu();
                break;
            case 3:
                cout << "Exiting the program." << endl;
                return 0;
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

    return 0;
}
