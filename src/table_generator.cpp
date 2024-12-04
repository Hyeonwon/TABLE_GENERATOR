#include "table_generator.h"
#include "table_db.h"
#include <vector>
#include "course_db.h"

using namespace std;

TableGenerator::TableGenerator(CourseDatabase &courseDB, TableDatabase &tableDB)
    : courseDB(courseDB), tableDB(tableDB), totalGrade(0)
{
}


void TableGenerator::generateTable(Table &table) //query�� ó���� courseDB�� �ִ� course�� greedyAloghrithm���� �ð�ǥ ����
{
    vector<Course> courses = this->courseDB.query(this->query);
    cout << courses.size() << endl;
    for (Course &course : courses) // course�� ����ŭ �ݺ�
    {
        if (totalGrade >= currentGrade + course.get_grade()) // ������ �� ���� �� ���� ���� ���� + course�� ������ �� �۰ų� ���� ���
        {
            if (this->findTime(course) && isConflict(course)) // �ð��� ��ġ�� ���� ���
            {
                table.insert_course(course);// �ð�ǥ�� ���� �߰�
                this->existCourse.push_back(course.get_name());
                this->currentGrade += course.get_grade(); // ���� �� �������� ������ ������ŭ ����
                cout << "add : " << course.get_name() << endl;
                for (CourseTime ctime : course.get_times())
                {
                    cout << "Day : ";
                    switch (ctime.weekday)
                    {
                    case Weekday::Mon:
                        cout << "Mon " << ctime.time << endl;
                        break;
                    case Weekday::Tue:
                        cout << "Tue " << ctime.time << endl;
                        break;
                    case Weekday::Wed:
                        cout << "Wed " << ctime.time << endl;
                        break;
                    case Weekday::Thu:
                        cout << "Thu " << ctime.time << endl;
                        break;
                    case Weekday::Fri:
                        cout << "Fri " << ctime.time << endl;
                        break;
                    case Weekday::Sat:
                        cout << "Sat " << ctime.time << endl;
                        break;
                    case Weekday::Sun:
                        cout << "Sun " << ctime.time << endl;
                        break;
                    }
                }
            }
        }     
    }
    cout << "total : " << totalGrade << "current : " << currentGrade << endl;
    this->tableDB.insert(table);
}

void TableGenerator::setQuery(CourseQuery query) // table generator ����
{
    this->query = query;
}

void TableGenerator::setTotalGrade(int grade) // ������ �ð�ǥ�� �� ������ ����
{
    this->totalGrade = grade;
}

bool TableGenerator::findTime(Course course) // ��ġ�� �ð� �ִ��� Ȯ��
{
    for (const CourseTime availableTime : course.get_times())
    {
        for (const CourseTime existingTime : this->time)
        {
            if (availableTime.weekday == existingTime.weekday && availableTime.time == existingTime.time) // ��ġ�� �ð��� �ִ� ���
            {
                return false;
            }
        }
    }

    for (const CourseTime availableTime : course.get_times())
    {
        this->time.push_back(availableTime);
    }

    return true; // ���������� �߰���
}

bool TableGenerator::isConflict(Course course)
{
    for (string name : this->existCourse)
    {
        if (name == course.get_name())
        {
            return false;
        }
    }

    return true;
}