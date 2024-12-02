#ifndef _TABLE_DB_H_
#define _TABLE_DB_H_

#include "table.h"
class Table;
#include <string>
#include <vector>
#include "course.h"

struct TableQuery
{
    std::string id;
    Semester semester;
    std::string year;
    std::string name;
    std::string user_id;
};

class TableDatabase
{
  public:
    std::vector<Table> query(TableQuery tableQuery);
    void insert(Table &table);
    void update(Table &table);
    void remove(int id);
    void load();
    void save();

    std::vector<Table> get_tables() const;

  private:
    std::vector<Table> tables;
};

#endif
