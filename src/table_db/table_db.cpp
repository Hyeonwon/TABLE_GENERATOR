#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "table_db.h"
#include "parser.h"

using namespace std;

vector<Table> TableDatabase::query(TableQuery tableQuery) {
    vector<Table> result;
    if (!tableQuery.id.empty()) {
        copy_if(tables.begin(), tables.end(), back_inserter(result), 
            [&](const Table& table) { return table.get_id() == stoi(tableQuery.id);});
    }
    if (!tableQuery.name.empty()) {
        copy_if(result.begin(), result.end(), back_inserter(result),
                [&](const Table &table) { return table.get_name() == tableQuery.name; });
    }
    try {
        encode_semester(tableQuery.semester);
        copy_if(result.begin(), result.end(), back_inserter(result),
                [&](const Table &table) { return table.get_semester() == tableQuery.semester; });
    } catch (exception &e) {
        
    }
    if (!tableQuery.year.empty()) {
        copy_if(result.begin(), result.end(), back_inserter(result),
                [&](const Table &table) { return table.get_year() == stoi(tableQuery.year); });
    }
    return result;
}

void TableDatabase::insert(Table &table) {	
	tables.push_back(table);
}

void TableDatabase::update(Table &table) {
    auto toUpdate = find_if(tables.begin(), tables.end(), [&table](const Table &t) { 
            return t.get_id() == table.get_id();
    });
  
    if (toUpdate != tables.end()) {
        tables.erase(toUpdate);
        tables.push_back(table);
    }
    else {
        cout << "There is no table." << endl;
    }
}

void TableDatabase::remove(int id) {
    auto toRemove = find_if(tables.begin(), tables.end(), [id](const Table &t) { 
        return t.get_id() == id; 
    });

    if (toRemove != tables.end()) {
        tables.erase(toRemove);
    }
}

void TableDatabase::load() {
    fstream file("tables.txt");
    string line;

    string::const_iterator it = line.begin();
    size_t current = 0;

    if (file.is_open()) {
        while (getline(file, line)) {
            try {
                ParseResult result = parse_tag(it, line.end());
                if (result.tag == "table") {
                    Table table(result.value);
                    tables.push_back(table);
                }
            }
            catch (const exception &e) {
                cout << "Failed to decode table (line: " << current << ")" << endl;
            }
        }
    }
    else {
        cout << "Failed to open tables.txt" << endl;
    }
}

void TableDatabase::save() {
    fstream file("tables.txt");

    if (file.is_open()) {
        for (Table table : tables) {
            file << table.encode();
        }
        file.close();
    } else {
        cout << "Failed to open tables.txt" << endl;
    }
}

// getter

vector<Table> TableDatabase::get_tables() const {
    return tables;
}
