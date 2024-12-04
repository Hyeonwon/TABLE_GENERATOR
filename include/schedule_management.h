#ifndef SCHEDULE_MANAGEMENT_H
#define SCHEDULE_MANAGEMENT_H

#include "course.h"
#include "table.h"
#include <string>

using namespace std;

// Function prototypes for helper functions used in modifySchedule.cpp
string getInput(const string &prompt);
int getOptionalIntInput(const string &prompt, int min, int max);
Semester getOptionalSemesterInput(const string &prompt);
void modifySelectedSchedule(int scheduleID);
void deleteSchedule(int scheduleID);
void modifyExistingSchedule(Table &selectedSchedule);
void addLectureToSchedule(Table &schedule);
void deleteLectureFromSchedule(Table &schedule);
void displayLectureList(const Table &table);
bool isLectureAlreadyAdded(const Table &schedule, const Course &newCourse);
bool isTimeConflict(const Table &schedule, const Course &newCourse);
void waitForEnterOrEsc();
Course createCourse(const string &name, const string &professor, Weekday day, int period, int credits);


#endif // MODIFY_SCHEDULE_HELPERS_H
