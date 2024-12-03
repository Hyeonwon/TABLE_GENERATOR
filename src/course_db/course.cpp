#include <iostream>
#include <stack>

#include "course.h"
#include "parser.h"

Course::Course()
{
}

bool CourseTime::operator==(const CourseTime &other) const
{
    return weekday == other.weekday && time == other.time && room == other.room && building == other.building;
}

bool Course::operator==(const Course &other) const
{
    return id == other.id && name == other.name && type == other.type && year == other.year &&
           semester == other.semester && english_a == other.english_a && professor == other.professor &&
           departments == other.departments && times == other.times;
}

// - Getter

int Course::get_id() const
{
    return id;
}

int Course::get_grade() const
{
    return times.size();
}

std::vector<CourseTime> Course::get_times() const
{
    return times;
}

std::string Course::get_name() const
{
    return name;
}

std::string Course::get_professor() const
{
    return professor;
}

CourseType Course::get_type() const
{
    return type;
}

Semester Course::get_semester() const
{
    return semester;
}

int Course::get_year() const
{
    return year;
}

bool Course::is_english_a() const
{
    return english_a;
}

std::set<Department> Course::get_departments() const
{
    return departments;
}

// Decoder

/*
Course Format
<course>
    <id>integer</id>
    <name>string</name>
    <professor>string</professor>
    <type>integer</type>
    <year>integer</year>
    <semester>integer</semester>
    <english>integer</english>
    <professor>string</professor>
    <department>integer</department> // support multiple department
    <department>integer</department>
    <time> // support multiple department
        <weekday>integer</weekday>
        <t>integer</t>
        <room>string</room>
        <building>string</building>
    </time>
    <time>
        <weekday>integer</weekday>
        <t>integer</t>
        <room>string</room>
        <building>string</building>
    </time>
</course>
*/

Course::Course(const std::string &input)
{
    std::string::const_iterator pt;
    for (pt = input.begin(); pt != input.end();)
    {
        ParseResult result = parse_tag(pt, input.end());
        if (!result.is_success)
            throw std::runtime_error("Fail to decode course.");
        else if (result.tag == "time")
            this->times.push_back(decode_coursetime(result.value));
        else if (result.tag == "department")
            this->departments.insert(decode_department(result.value));
        else if (result.tag == "id")
            id = std::stoi(result.value);
        else if (result.tag == "name")
            name = result.value;
        else if (result.tag == "professor")
            professor = result.value;
        else if (result.tag == "type")
            type = decode_coursetype(result.value);
        else if (result.tag == "semester")
            semester = decode_semester(result.value);
        else if (result.tag == "year")
            year = std::stoi(result.value);
        else if (result.tag == "english")
            english_a = result.value != "0";
        else if (result.tag != "course")
            throw std::runtime_error("Invalid tag error");

        if (pt + 1 == input.end())
            break;
    }
}

CourseTime decode_coursetime(const std::string &input)
{
    CourseTime time;
    std::string::const_iterator pt;
    for (pt = input.begin(); pt != input.end();)
    {
        ParseResult result = parse_tag(pt, input.end());
        if (result.tag == "weekday")
            time.weekday = decode_weekday(result.value);
        else if (result.tag == "t")
            time.time = std::stoi(result.value);
        else if (result.tag == "room")
            time.room = result.value;
        else if (result.tag == "building")
            time.building = result.value;

        if (pt + 1 == input.end())
            break;
    }
    return time;
}

Semester decode_semester(const std::string &input)
{
    int value = std::stoi(input);

    switch (value)
    {
    case 0:
        return Semester::Spring;
    case 1:
        return Semester::Summer;
    case 2:
        return Semester::Fall;
    case 3:
        return Semester::Winter;
    default:
        throw std::exception(); // Invalid semester value.
    }
}

Weekday decode_weekday(const std::string &input)
{
    int value = std::stoi(input);

    switch (value)
    {
    case 0:
        return Weekday::Sun;
    case 1:
        return Weekday::Mon;
    case 2:
        return Weekday::Tue;
    case 3:
        return Weekday::Wed;
    case 4:
        return Weekday::Thu;
    case 5:
        return Weekday::Fri;
    case 6:
        return Weekday::Sat;
    default:
        throw std::exception(); // Invalid semester value.
    }
}

CourseType decode_coursetype(const std::string &input)
{
    int value = std::stoi(input);

    switch (value)
    {
    case 0:
        return CourseType::General_Education;
    case 1:
        return CourseType::CoreCommunication;
    case 2:
        return CourseType::CoreCreativity;
    case 3:
        return CourseType::CoreChallenge;
    case 4:
        return CourseType::CoreConvergence;
    case 5:
        return CourseType::CoreTrust;
    case 6:
        return CourseType::ElectiveCommunication;
    case 7:
        return CourseType::ElectiveCreativity;
    case 8:
        return CourseType::ElectiveChallenge;
    case 9:
        return CourseType::ElectiveConvergence;
    case 10:
        return CourseType::ElectiveTrust;
    case 11:
        return CourseType::Major;
    case 12:
        return CourseType::MajorRequired;
    case 13:
        return CourseType::MajorFundamental;
    default:
        throw std::exception(); // Invalid semester value.
    }
}

Department decode_department(const std::string &input)
{
    int value = std::stoi(input);

    switch (value)
    {
    case 0:
        return Department::ComputerScience;
    default:
        throw std::exception(); // Invalid semester value.
    }
}

// - Encoder

std::string Course::encode() const
{
    std::string raw_value;
    raw_value.append("<course>");
    raw_value.append("<id>" + std::to_string(id) + "</id>");
    raw_value.append("<type>" + encode_coursetype(type) + "</type>");
    raw_value.append("<year>" + std::to_string(year) + "</year>");
    raw_value.append("<semester>" + encode_semester(semester) + "</semester>");
    if (english_a)
        raw_value.append("<english>1</english>");
    else
        raw_value.append("<english>0</english>");
    if (professor.size() > 0)
        raw_value.append("<professor>" + professor + "</professor>");
    for (auto department : departments)
        raw_value.append("<department>" + encode_department(department) + "</department>");
    for (auto time : times)
        raw_value.append("<time>" + encode_coursetime(time) + "</time>");
    raw_value.append("</course>");
    return raw_value;
}

std::string encode_coursetime(const CourseTime &time)
{
    std::string raw_value;
    raw_value.append("<weekday>" + encode_weekday(time.weekday) + "</weekday>");
    raw_value.append("<t>" + std::to_string(time.time) + "</t>");
    raw_value.append("<room>" + time.room + "</room>");
    raw_value.append("<building>" + time.building + "</building>");
    return raw_value;
}

std::string encode_semester(Semester semester)
{
    switch (semester)
    {
    case Semester::Spring:
        return "0";
    case Semester::Summer:
        return "1";
    case Semester::Fall:
        return "2";
    case Semester::Winter:
        return "3";
    }
}

std::string encode_weekday(Weekday weekday)
{
    switch (weekday)
    {
    case Weekday::Sun:
        return "0";
    case Weekday::Mon:
        return "1";
    case Weekday::Tue:
        return "2";
    case Weekday::Wed:
        return "3";
    case Weekday::Thu:
        return "4";
    case Weekday::Fri:
        return "5";
    case Weekday::Sat:
        return "6";
    }
}

std::string encode_coursetype(CourseType type)
{
    switch (type)
    {
    case CourseType::General_Education:
        return "0";
    case CourseType::CoreCommunication:
        return "1";
    case CourseType::CoreCreativity:
        return "2";
    case CourseType::CoreChallenge:
        return "3";
    case CourseType::CoreConvergence:
        return "4";
    case CourseType::CoreTrust:
        return "5";
    case CourseType::ElectiveCommunication:
        return "6";
    case CourseType::ElectiveCreativity:
        return "7";
    case CourseType::ElectiveChallenge:
        return "8";
    case CourseType::ElectiveConvergence:
        return "9";
    case CourseType::ElectiveTrust:
        return "10";
    case CourseType::Major:
        return "11";
    case CourseType::MajorRequired:
        return "12";
    case CourseType::MajorFundamental:
        return "13";
    }
}

std::string encode_department(Department department)
{
    switch (department)
    {
    case Department::ComputerScience:
        return "0";
    }
}