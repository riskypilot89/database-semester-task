#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#define LEN_PARSER_BUF 512

class command;

class parser: public lexer
{
private:
    bool get_int (char *str, int *res);
    bool parse_where (command &c);
    char *cmp_copy (char *cmp_with);
public:
    char buf[LEN_PARSER_BUF] = {};
    size_t curr_pos = 0;
    
    parser (const char *input_file = nullptr, const char *output_file = nullptr)
    : lexer(input_file, output_file) {}
    ~parser () {}
    
    command parse_query ();
};

#endif

