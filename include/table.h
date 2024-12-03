#ifndef _TABLE_H_
#define _TABLE_H_

#include "course.h"
#include "table_db.h"
struct TableQuery;
#include <string>
#include <vector>

class Table
{
  public:
    int get_total_grade();
    void remove_course(Course &course);
    bool insert_course(Course &course);
    std::vector<Course> get_course() const;
    bool is_satisfy(TableQuery tableQuery) const;
    std::string encode();
    Table(const std::string &str);
    Table(int iid, int iuser_id, std::string iname, std::vector<Course> icourses, int iyear, Semester isemester);
    ~Table();

    Semester get_semester() const;
    int get_year() const;
    //vector<Course>& get_courses() const;
    std::string get_name() const;
    int get_user_id() const;
    int get_id() const;

  private:
    Semester semester;
    int year;
    std::vector<Course> courses;
    std::string name;
    int user_id;
    int id;
};

#endif