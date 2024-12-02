#include "course_db.h"
#include "parser.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>

#define TokenLength 2

bool DateIndexKey::operator==(const DateIndexKey &key) const
{
    return year == key.year && semester == key.semester && time == key.time && weekday == key.weekday;
}

std::size_t DateIndexKeyHash::operator()(const DateIndexKey &key) const
{
    return std::hash<int>()(key.year) ^ (std::hash<std::string>()(encode_semester(key.semester)) << 1) ^
           (std::hash<std::string>()(encode_weekday(key.weekday)) << 2) ^ (std::hash<int>()(key.time) << 3);
}

std::string convert_wstr_to_str(std::wstring wstr)
{
    const size_t size = wstr.size() * MB_CUR_MAX + 1;
    char *new_str = new char[size];
    size_t converted = std::wcstombs(new_str, wstr.c_str(), size);
    if (converted == -1)
    {
        delete[] new_str;
        return "";
    }
    std::string str = new_str;
    delete[] new_str;
    return str;
}

std::wstring convert_str_to_wstr(const std::string &str)
{
    size_t size = str.size() + 1;
    wchar_t *wchars = new wchar_t[size];

    std::mbstowcs(wchars, str.c_str(), size);

    std::wstring wstr = wchars;
    delete[] wchars;

    return wstr;
}

std::set<std::string> get_token(const std::string &str, int by)
{
    if (str.size() < by)
        return {};

    std::set<std::string> result;

    std::wstring wstr = convert_str_to_wstr(str);
    for (size_t i = 0; i < wstr.size() - by + 1; i++)
        result.insert(convert_wstr_to_str(wstr.substr(i, by)));
    return result;
}

CourseDatabase::CourseDatabase()
{
    std::setlocale(LC_ALL, "ko_KR.UTF-8");
};

void CourseDatabase::load()
{
    load_courses();
    load_date_index();
    load_name_index();
}

void CourseDatabase::load_courses()
{
    std::ifstream file("courses.txt");
    if (!file.is_open())
    {
        std::cout << "Failed to open courses.txt" << std::endl;
        return;
    }

    size_t current_line = 0;
    std::string line_str;
    while (std::getline(file, line_str))
    {
        try
        {
            std::string::const_iterator pt = line_str.begin();
            ParseResult result = parse_tag(pt, line_str.end());
            if (result.tag == "course")
            {
                Course course(result.value);
                int id = course.get_id();
                course_by_id[id] = course;
                course_ptrs.insert(&course_by_id[id]);
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Fail to decode course (line:" << current_line + 1 << ")" << std::endl;
        }
        current_line++;
    }
    file.close();
}

/*
indexing_by_date format
<index>
    <year>integer</year>
    <semester>integer</semester>
    <time>integer</time>
    <courses>
        <id>integer</id>
        <id>integer</id> // course id
    </courses>
</index>
*/

void CourseDatabase::load_date_index()
{
    std::ifstream file("date_index.txt");

    if (!file.is_open())
    {
        file.close();
        indexing_by_date();
        return;
    }

    std::string line_str;
    while (std::getline(file, line_str))
    {
        std::string::const_iterator point_of_input = line_str.begin();
        ParseResult index_tag = parse_tag(point_of_input, line_str.end());

        if (index_tag.is_success && index_tag == "index")
        {
            point_of_input = index_tag.value.begin();
            DateIndexKey key;
            CoursePtrSet course_ptr_set;
            for (; point_of_input != index_tag.value.end(); point_of_input++)
            {
                ParseResult property_tag = parse_tag(point_of_input, index_tag.value.end());
                if (property_tag == "year")
                    key.year = std::stoi(property_tag.value);
                else if (property_tag == "semester")
                    key.semester = decode_semester(property_tag.value);
                else if (property_tag == "time")
                    key.time = std::stoi(property_tag.value);
                else if (property_tag == "weekday")
                    key.weekday = decode_weekday(property_tag.value);
                else if (property_tag == "courses")
                {
                    std::string id_tags = property_tag.value;
                    std::string::const_iterator point_of_id_tags = id_tags.begin();
                    for (; point_of_id_tags != id_tags.end(); point_of_id_tags++)
                    {
                        ParseResult id_tag = parse_tag(point_of_id_tags, id_tags.end());
                        if (id_tag == "id")
                            course_ptr_set.insert(&course_by_id[std::stoi(id_tag.value)]);
                    }
                }
            }
            date_index[key].insert(course_ptr_set.begin(), course_ptr_set.end());
        }
    }

    file.close();
}

void CourseDatabase::indexing_by_date()
{
    for (auto &element : course_by_id)
    {
        Course &course = element.second;
        for (const auto time : course.get_times())
        {
            DateIndexKey key = {course.get_year(), course.get_semester(), time.weekday, time.time};
            date_index[key].insert(&course);
        }
    }

    std::ofstream file("date_index.txt");

    std::string raw_value;
    for (auto index : date_index)
    {
        const DateIndexKey key = index.first;
        raw_value.append("<index>");
        raw_value.append("<year>" + std::to_string(key.year) + "</year>");
        raw_value.append("<semester>" + encode_semester(key.semester) + "</semester>");
        raw_value.append("<time>" + std::to_string(key.time) + "</time>");
        raw_value.append("<weekday>" + encode_weekday(key.weekday) + "</weekday>");
        raw_value.append("<courses>");
        for (auto course : index.second)
            raw_value.append("<id>" + std::to_string(course->get_id()) + "</id>");
        raw_value.append("</courses>");
        raw_value.append("</index>");
        file << raw_value << std::endl;
        raw_value.clear();
    }

    file.close();
}

/*
indexing by name format
<index>
    <name>string</name>
    <courses>
        <id>integer</id>
    </courses>
</index>
*/

void CourseDatabase::indexing_by_name()
{
    for (const auto &element : course_by_id)
    {
        const Course &course = element.second;
        std::set<std::string> tokens = get_token(course.get_name(), TokenLength);
        for (const auto &token : tokens)
            name_index[token].insert(&course);
    }

    std::ofstream file("name_index.txt", std::ios::binary);
    std::string raw_value;
    for (const auto &element : name_index)
    {
        raw_value.append("<index>");
        raw_value.append("<name>" + element.first + "</name>");
        raw_value.append("<courses>");
        for (const auto &course : element.second)
        {
            raw_value.append("<id>" + std::to_string(course->get_id()) + "</id>");
        }
        raw_value.append("</courses>");
        raw_value.append("</index>");

        file << raw_value << std::endl;
        raw_value.clear();
    }
    file.close();
}

void CourseDatabase::load_name_index()
{
    std::ifstream file("name_index.txt");

    if (!file.is_open())
    {
        file.close();
        indexing_by_name();
        return;
    }

    std::string line_str;
    while (std::getline(file, line_str))
    {
        std::string::const_iterator point_of_input = line_str.begin();
        ParseResult index_tag = parse_tag(point_of_input, line_str.end());

        if (index_tag.is_success && index_tag == "index")
        {
            point_of_input = index_tag.value.begin();
            std::string key;
            CoursePtrSet course_ptrs;
            for (; point_of_input != index_tag.value.end(); point_of_input++)
            {
                ParseResult property_tag = parse_tag(point_of_input, index_tag.value.end());
                if (property_tag == "name")
                    key = property_tag.value;
                else if (property_tag == "courses")
                {
                    std::string id_tags = property_tag.value;
                    std::string::const_iterator point_of_id_tags = id_tags.begin();
                    for (; point_of_id_tags != id_tags.end(); point_of_id_tags++)
                    {
                        ParseResult id_tag = parse_tag(point_of_id_tags, id_tags.end());
                        if (id_tag.is_success && id_tag == "id")
                        {
                            int id = std::stoi(id_tag.value);
                            course_ptrs.insert(&course_by_id[id]);
                        }
                    }
                }
            }
            name_index[key] = course_ptrs;
        }
    }

    file.close();
}

std::vector<Course> CourseDatabase::query(CourseQuery condition) const
{

    CoursePtrSet matched_courses;

    // query by using date index.
    if (condition.weekdays.empty() && condition.times.empty())
    {
        matched_courses.insert(course_ptrs.begin(), course_ptrs.end());
    }
    else
    {
        std::set<Weekday> week_query;
        std::set<Time> time_query;
        if (condition.weekdays.empty())
            week_query = {Weekday::Sun, Weekday::Mon, Weekday::Tue, Weekday::Wed,
                          Weekday::Thu, Weekday::Fri, Weekday::Sat};
        else
            week_query = condition.weekdays;

        if (condition.times.empty())
            time_query = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        else
            time_query = condition.times;

        for (auto time : time_query)
        {
            for (auto week : week_query)
            {
                const DateIndexKey &key = {condition.year, condition.semester, week, time};
                if (date_index.count(key) > 0)
                    matched_courses.insert(date_index.at(key).begin(), date_index.at(key).end());
            }
        }
    }

    // query by name index
    if (!condition.name.empty())
    {
        std::set<std::string> search_tokens = get_token(condition.name, TokenLength);
        CoursePtrSet name_matched_courses;
        for (const auto &token : search_tokens)
        {
            if (name_index.count(token))
                name_matched_courses.insert(name_index.at(token).begin(), name_index.at(token).end());
        }
        CoursePtrSet intersection;
        std::set_intersection(matched_courses.begin(), matched_courses.end(), name_matched_courses.begin(),
                              name_matched_courses.end(), std::inserter(intersection, intersection.begin()));
        matched_courses = std::move(intersection); // remove courses not in name_matched_courses.
    }

    // query by departments, professors
    std::vector<Course> result;
    for (const auto course_ptr : matched_courses)
    {
        bool is_matched = true;

        if (!condition.departments.empty())
        {
            std::set<Department> departments = course_ptr->get_departments();
            if (!std::includes(condition.departments.begin(), condition.departments.end(), departments.begin(),
                               departments.end()))
                is_matched = false;
        }

        if (is_matched && !condition.professors.empty())
        {
            if (std::find(condition.professors.begin(), condition.professors.end(), course_ptr->get_professor())
                    ->empty())
                is_matched = false;
        }

        if (is_matched)
            result.push_back(*course_ptr);
    }

    return result;
}
