#ifndef SCHEDULE_MANAGEMENT_H
#define SCHEDULE_MANAGEMENT_H

#include "course.h"
#include "table.h"
#include <iostream>
#include <string>
#include <vector>

// Query schedules based on user input
std::vector<int> querySchedules(const std::string &userName, int year, Semester semester, int scheduleID);
Table getTableById(int scheduleID);
void setMainSchedule(int scheduleID);
void deleteTable(int scheduleID);
std::vector<Course> searchLectures(const std::string &dayInput, const std::string &periodInput,
                                   const std::string &courseName, const std::string &professorName);

// Lecture checks and operations
bool isLectureAlreadyAdded(const Table &schedule, const Course &selectedLecture);
bool isCreditLimitExceeded(const Table &schedule, const Course &selectedLecture);
int getCurrentCredits(const Table &schedule);
int getMaxCredits(const Table &schedule);
bool isTimeConflict(const Table &schedule, const Course &selectedLecture);

void addLecture(Table &schedule, const Course &selectedLecture);
void deleteLectures(Table &schedule, const std::vector<int> &lectureNumbers);

// Utility functions to handle user input and schedule operations
std::string getInput(const std::string &prompt);
int getOptionalIntInput(const std::string &prompt, int min, int max);
Semester getOptionalSemesterInput(const std::string &prompt);
int navigateMenu(const std::vector<std::string> &options, int selectedOption);
std::string encodeSemester(Semester semester);
std::string encodeWeekday(Weekday weekday);

#endif