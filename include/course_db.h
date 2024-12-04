#ifndef COURSE_DB_H
#define COURSE_DB_H

#include "course.h"
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

typedef std::set<const Course *> CoursePtrSet;

std::set<std::string> get_token(const std::string &, int);

struct CourseQuery
{
    // Semester is required.
    Semester semester;
    // Year is required
    int year;
    // Set weekdays that you want to query. if empty, all weekday's courses will be quried.
    std::set<Weekday> weekdays;
    // Set times you want to query. if empty, all times will be quried.
    std::set<Time> times;
    // Query by course name. Query support n-gram query. if empty, all courses will be quried.
    std::string name;
    // Query by departments. if empty, all department's courses will be quried.
    std::set<Department> departments;
    // Query by professor. if empty, all courses will be quried.
    std::set<std::string> professors;
};

struct DateIndexKey
{
    int year;
    Semester semester;
    Weekday weekday;
    Time time;

    bool operator==(const DateIndexKey &) const;
};

struct DateIndexKeyHash
{ // for using index key in unordered map's key.
    std::size_t operator()(const DateIndexKey &) const;
};

class CourseDatabase
{
    std::unordered_map<int, Course> course_by_id;
    std::set<Course *> course_ptrs;
    std::unordered_map<DateIndexKey, CoursePtrSet, DateIndexKeyHash> date_index;
    std::unordered_map<std::string, CoursePtrSet> name_index;
    int version;

  public:
    CourseDatabase();
    std::vector<Course> query(CourseQuery) const;
    void load();

  private:
    void load_db_version();
    void update_db_version(int);
    // return course data version.
    int load_courses();
    void load_date_index(bool);
    void indexing_by_date();
    void load_name_index(bool is_update);
    void indexing_by_name();
    std::fstream open_cache(std::string);
    std::fstream open_file(std::string);
    std::ofstream write_cache(std::string);
};
#endif