#ifndef DISPLAY_H
#define DISPLAY_H

#include "course.h"          // A�� ��� ������ ����
#include "course_db.h"       // A�� ��� ������ ����
#include "table.h"           // A�� ��� ������ ����
#include "table_db.h"        // A�� ��� ������ ����
#include "table_generator.h" // A�� ��� ������ ����
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

// ����� ���� ����ü
struct User
{
    string name;
    int year;
    int student_id;
    string department;
};

// ���� ����� �� �ð�ǥ ������
extern User currentUser;
extern vector<class Schedule *> schedules;

// Schedule ����ü ����
struct Schedule
{
    int id;
    int year;
    string semester;
    string department;
    int totalCredits;
    vector<string> courses;
    vector<string> days;

    Schedule(int year, const string &semester, const string &department, int totalCredits);
    void addCourse(const string &course);
    void addDay(const string &day);
    void removeCourse(int index);
    void removeDay(int index);
    void display();
};

// �Լ� ����
void createSchedule(User &currentUser, vector<Schedule *> &schedules);
void searchAndModifySchedule();
void setupUser(User &user);
void mainMenu();

#endif // DISPLAY_H
