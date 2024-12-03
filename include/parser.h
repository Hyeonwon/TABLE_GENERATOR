#ifndef PARSER_H
#define PARSER_H

#include <string>

struct ParseResult
{
    bool is_success;
    std::string tag;
    std::string value;

    bool operator==(const std::string&) const;
};

// Parse tag and tag's contents
ParseResult parse_tag(std::string::const_iterator&, const std::string::const_iterator&);

#endif