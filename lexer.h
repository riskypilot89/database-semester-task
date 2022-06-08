#ifndef LEXER_H
#define LEXER_H

#include <cstring>
#include <memory>
#include <cstdlib>
#include <iostream>
#include "token.h"

#define LEN_LEXER_BUF 256

class lexer
{
private:
    bool p = false; // are we into parantheses?
    
    char lexem[LEN_LEXER_BUF] = {};
    size_t lexem_pos = 0;
    
    int get_lexem (char **start, size_t *len);
    int get_string ();
    char * get_cmp ();
    
    int init(const char *input_file, const char *ouput_file);
public:
    char curr_string[LEN_LEXER_BUF] = {};
    size_t currstr_pos = 0;
    
    lexer (const char *input_file = nullptr, const char *output_file = nullptr);
    ~lexer ();
    
    const char *input_file = nullptr;
    FILE *input_stream = stdin;
    
    const char *output_file = nullptr;
    FILE *output_stream = stdout;
    
    token get_token ();
};

#endif
