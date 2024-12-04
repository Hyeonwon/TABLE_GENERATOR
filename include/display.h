#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "course.h"
#include "table.h"
#include "table_db.h"
#include "course_db.h"
#include "parser.h"

using namespace std;

extern TableDatabase tableDatabase;
extern CourseDatabase courseDatabase;

bool checkForEsc();
void waitForEnterOrEsc();
void userSettings();
void displayScheduleTable(const Table& table);
void displayScheduleMenu();
void mainMenu();
void generateSchedule();
void modifySchedule();
int navigateSchedules(const vector<int>& schedules);
void displayScheduleInfo(const Table& table);
vector<int> searchSchedules(const string& userName, int year, Semester semester, int scheduleID);
void displayLectureList(const Table& table);
int navigateLectures(const vector<Course>& lectures);
void displayCurrentSchedule(const Table& table);



#endif 
