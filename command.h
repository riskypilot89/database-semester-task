#ifndef COMMAND_H
#define COMMAND_H

#include <cstdlib>
#include <iostream>
#include "record.h"

#define LEN_CHAR_ARRAY 256
#define LEN_WORDS_ARRAY 512

class btree;

enum cmd_type
{
    CMD_NONE, QUIT, STOP, SELECT, INSERT, DELETE, PERR, VOID,
    // (PERR - error of Parse)
};

enum op_type_t
{
  OP_NONE, AND, OR,
};

enum field_type
{
    FIELD_NONE, NAME, PHONE, GROUP,
};

struct cond_type
{
    enum cond_left_t {
        _FIELD_NONE, _NAME, _PHONE, _GROUP,
    } cond_left_t;
    
    enum type_t {
        COND_NONE, EQ, NE, LT, GT, LE, GE, LIKE,
    } type_t;
    
    char *str; // we compare strings
    int number; // or numbers (phones, groups)
};

class command: public record
{
public:
    cmd_type type = cmd_type::CMD_NONE;
    
    // select <f[0]>, <f[1]>, <f[2]>
    field_type f[3];
    // ... or just "*" (all)
    bool ast = false;
    
    // ... where <name> (or smth else) ... <like> (or smth else)
    cond_type cond[3];
    
    // ... AND / OR
    op_type_t oper[2];
    
    void _clear_cmd_str_copy ();
    void input_cmd_str_copy (char *cmd_str);
    char cmd_str_copy[LEN_CHAR_ARRAY] = {};
    
    void _clear_words_cmd_str ();
    char *words_cmd_str_copy[LEN_WORDS_ARRAY] = {};
    size_t len_wcsc = 0;
    void _words_cmd_str_copy ();
    
    void split_cmd_str (char *cmd_str);
    
    friend class btree;
    
    
    void print_cmd (FILE *stream = stdout);
    
public:
    command ()
    : record() {}
    ~command () {}
    
    bool strcmp_like (char *cmd_str, char *str);
    size_t solve_like_strcmp (char *str, char *words_cmd_str, size_t marker, bool *flag);
};

#endif
