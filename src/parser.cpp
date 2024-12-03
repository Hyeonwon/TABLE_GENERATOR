#include "parser.h"

bool ParseResult::operator==(const std::string &tag) const
{
    return this->tag == tag;
}

// Parse tag and tag's contents
ParseResult parse_tag(std::string::const_iterator &pt, const std::string::const_iterator &end)
{
    ParseResult result;

    std::string target_token;
    std::string temp_token;
    bool is_success = false;
    bool is_token_content = false;
    bool is_token = false;
    bool is_closed = false;
    std::string value_str;

    for (; pt != end; ++pt)
    {
        if (is_token)
        {
            if (*pt == '/')
                is_closed = true;
            else if (*pt == '>')
            {
                bool is_target_exit = target_token.size() > 0;
                if (is_closed)
                {
                    if (is_target_exit && target_token == temp_token)
                    {
                        is_success = true;
                        break;
                    }
                    else
                        value_str.append("</" + temp_token + ">");
                    is_closed = false;
                }
                else if (!is_target_exit)
                {
                    target_token = temp_token;
                    is_token_content = true;
                }
                else
                    value_str.append("<" + temp_token + ">");
                is_token = false;
                temp_token.clear();
            }
            else
                temp_token.push_back(*pt);
        }
        else if (*pt == '<')
            is_token = true;
        else if (*pt != '\n' && is_token_content)
            value_str.push_back(*pt);
    }

    return ParseResult{is_success, target_token, value_str};
}