#include "table_generator.h"
#include "table_db.h"
#include <vector>
#include "course_db.h"

using namespace std;

TableGenerator::TableGenerator(CourseDatabase &courseDB, TableDatabase &tableDB)
    : courseDB(courseDB), tableDB(tableDB), totalGrade(0)
{
}


void TableGenerator::generateTable(TableDatabase &tableDB, Table table) //query�� ó���� courseDB�� �ִ� course�� greedyAloghrithm���� �ð�ǥ ����
{
    vector<Course> courses = this->courseDB.query(this->query);
    for (Course &course : courses) // course�� ����ŭ �ݺ�
    {
        if (totalGrade >= currentGrade + course.get_times().size()) // ������ �� ���� �� ���� ���� ���� + course�� ������ �� �۰ų� ���� ���
        {
            if (this->findTime(course)) // �ð��� ��ġ�� ���� ���
            {
                table.insert_course(course);                     // �ð�ǥ�� ���� �߰�
                this->currentGrade += course.get_times().size(); // ���� �� �������� ������ ������ŭ ����
            }
        }    
    }
    tableDB.insert(table);
}

void TableGenerator::setQuery(CourseQuery query) // table generator ����
{
    this->query = query;
}

void TableGenerator::setTotalGrade(int grade) // ������ �ð�ǥ�� �� ������ ����
{
    this->totalGrade = grade;
}

bool TableGenerator::findTime(Course &course) // ��ġ�� �ð� �ִ��� Ȯ��
{
    for (const auto availableTime : course.get_times())
    {
        if (this->time.find(availableTime) != this->time.end())
        {
            return false;
        }
    }

    for (const auto availableTime : course.get_times())
    {
        this->time.insert(availableTime);
    }
    return true;
}