#ifndef SCHEDULE_MANAGEMENT_H
#define SCHEDULE_MANAGEMENT_H

#include <vector>
#include <string>
#include "table.h"
#include "course.h"

std::vector<int> querySchedules(const std::string& userName, int year, Semester semester, int scheduleID);
Table getTableById(int scheduleID);
void setMainSchedule(int scheduleID);
void deleteTable(int scheduleID);
std::vector<Course> searchLectures(const std::string& dayInput, const std::string& periodInput, const std::string& courseName, const std::string& professorName);

bool isLectureAlreadyAdded(const Table& schedule, const Course& selectedLecture);
bool isCreditLimitExceeded(const Table& schedule, const Course& selectedLecture);
int getCurrentCredits(const Table& schedule);
int getMaxCredits(const Table& schedule);
bool isTimeConflict(const Table& schedule, const Course& selectedLecture);

void addLecture(Table& schedule, const Course& selectedLecture);
void deleteLectures(Table& schedule, const std::vector<int>& lectureNumbers);

#endif 
