#include "lexer.h"
#include <cstdio>

lexer:: lexer (const char *input_file, const char *output_file)
{
    init(input_file, output_file);
}

int lexer:: init (const char *input_file, const char *output_file)
{
    if (input_file) {
        if (!strcmp(input_file, "stdin") || !strcmp(input_file, "stdin\n"))
            input_stream = stdin;
        else {
            input_stream = fopen(input_file, "r");
            if (!input_stream) {
                printf("Error! Cannot open input.txt. Using stdin...\n");
                input_stream = stdin;
                return 1;
            }
            else
                this -> input_file = input_file;
        }
    }
    
    if (output_file) {
        if (!strcmp(output_file, "stdout") || !strcmp(output_file, "stdout\n"))
            output_stream = stdout;
        else {
            output_stream = fopen(output_file, "w");
            if (!output_stream) {
                printf("Error! Cannot open output.txt. Using stdout...\n");
                output_stream = stdout;
                return 2;
            }
        else
            this -> output_file = output_file;
        }
    }
    
    return 0;
}

lexer:: ~lexer ()
{
    if (input_file)
        fclose(input_stream);
    
    if (output_file)
        fclose(output_stream);
}

token lexer:: get_token ()
{
    token a;
    char *word;
    size_t len = 0;
    
    // preparatory check:
    int check = get_lexem(&word, &len);
    if (check == 1) {
        a.type = token::EF;
        a.cmp_with = nullptr;
        return a;
    }
    if (check == 2) {
        a.type = token::FERR;
        a.cmp_with = nullptr;
        return a;
    }
    
    if (!len) {
        a.type = token::VOID;
        a.cmp_with = nullptr;
    }
    // command types:
    else if (!p && len == 4 && !strncmp(word, "quit", 4)) {
        a.type = token::QUIT;
        a.cmp_with = nullptr;
    }
    else if (!p && len == 4 && !strncmp(word, "stop", 4)) {
        a.type = token::STOP;
        a.cmp_with = nullptr;
    }
    else if (!p && len == 6 && !strncmp(word, "select", 6)) {
        a.type = token::SELECT;
        a.cmp_with = nullptr;
    }
    else if (!p && len == 6 && !strncmp(word, "insert", 6)) {
        a.type = token::INSERT;
        a.cmp_with = nullptr;
    }
    else if (!p && len == 6 && !strncmp(word, "delete", 6)) {
        a.type = token::DELETE;
        a.cmp_with = nullptr;
    }
    // special words:
    else if (!p && len == 5 && !strncmp(word, "where", 5)) {
        a.type = token::WHERE;
        a.cmp_with = nullptr;
    }
    // fields:
    else if (len == 1 && !strncmp(word, "*", 1)) {
        a.type = token::AST;
        a.cmp_with = nullptr;
    }
    else if (!p && len == 4 && !strncmp(word, "name", 4)) {
        a.type = token::NAME;
        a.cmp_with = nullptr;
    }
    else if (!p && len == 5 && !strncmp(word, "phone", 5)) {
        a.type = token::PHONE;
        a.cmp_with = nullptr;
    }
    else if (!p && len == 5 && !strncmp(word, "group", 5)) {
        a.type = token::GROUP;
        a.cmp_with = nullptr;
    }
    // condition types:
    else if (len == 1 && !strncmp(word, "=", 1)) {
        a.type = token::EQ;
        if (!(a.cmp_with = get_cmp()))
            a.type = token::LERR;
    }
    else if (len == 2 && !strncmp(word, "<>", 2)) {
        a.type = token::NE;
        if (!(a.cmp_with = get_cmp()))
            a.type = token::LERR;
    }
    else if (len == 1 && !strncmp(word, "<", 1)) {
        a.type = token::LT;
        if (!(a.cmp_with = get_cmp()))
            a.type = token::LERR;
    }
    else if (len == 2 && !strncmp(word, "<=", 2)) {
        a.type = token::LE;
        if (!(a.cmp_with = get_cmp()))
            a.type = token::LERR;
    }
    else if (len == 1 && !strncmp(word, ">", 1)) {
        a.type = token::GT;
        if (!(a.cmp_with = get_cmp()))
            a.type = token::LERR;
    }
    else if (len == 2 && !strncmp(word, ">=", 2)) {
        a.type = token::GE;
        if (!(a.cmp_with = get_cmp()))
            a.type = token::LERR;
    }
    else if (!p && len == 4 && !strncmp(word, "like", 4)) {
        a.type = token::LIKE;
        if (!(a.cmp_with = get_cmp()))
            a.type = token::LERR;
    }
    // operation types:
    else if (!p && len == 3 && !strncmp(word, "and", 3)) {
        a.type = token::AND;
        a.cmp_with = nullptr;
    }
    else if (!p && len == 2 && !strncmp(word, "or", 2)) {
        a.type = token::OR;
        a.cmp_with = nullptr;
    }
    // other:
    else if (len == 1 && !strncmp(word, ",", 1)) {
        a.type = token::COMMA;
        a.cmp_with = nullptr;
    }
    else if (len == 1 && !strncmp(word, ";", 1)) {
        a.type = token::ENQ;
        a.cmp_with = nullptr;
    }
    else if (len == 1 && !strncmp(word, "(", 1)) {
        p = true;
        a.type = token::LP;
        a.cmp_with = nullptr;
    }
    else if (len == 1 && !strncmp(word, ")", 1)) {
        p = false;
        a.type = token::RP;
        a.cmp_with = nullptr;
    }
    else {
        a.type = token::OTHER;
        a.cmp_with = word;
        // we initialize this pointer for knowledge what word we got exactly
    }
    
    return a;
}

int lexer:: get_lexem (char **start, size_t *len)
{
    size_t skip, i;
    int check;
    
    if ((currstr_pos >= LEN_LEXER_BUF || !curr_string[currstr_pos])) {
        check = get_string();
        if (check == 1)
            return 1;
        if (check == 2)
            return 2;
    }
    
    for (skip = currstr_pos; curr_string[skip] == ' ' || curr_string[skip] == '\t' || curr_string[skip] == '\n' || !curr_string[skip]; ) {
        if (skip >= LEN_LEXER_BUF - 1 || !curr_string[skip]) {
            if (!currstr_pos) {
                *len = 0;
                return 0;
            }
            check = get_string();
            if (check == 1)
                return 1;
            if (check == 2)
                return 2;
            skip = currstr_pos;
        }
        else
            ++skip;
    }
    
    for (i = skip; !(curr_string[i] == ' ' || curr_string[i] == '\t' || curr_string[i] == '\n' || curr_string[i] == '(' || curr_string[i] == ')' || curr_string[i] == ',' || curr_string[i] == ';'); ) {
        if (lexem_pos >= LEN_LEXER_BUF - 1)
            return 0;
        
        if (i >= LEN_LEXER_BUF - 1 || !curr_string[i]) {
            check = get_string();
            i = currstr_pos;
            if (check == 1)
                break;
            if (check == 2)
                return 2;
        }
        else
            lexem[lexem_pos++] = curr_string[i++];
    }
    
    if (!lexem_pos && (curr_string[i] == '(' || curr_string[i] == ')' || curr_string[i] == ',' || curr_string[i] == ';'))
        lexem[lexem_pos++] = curr_string[i++];
    
    lexem[lexem_pos] = '\0';
    
    *start = lexem;
    *len = lexem_pos;
    
    currstr_pos = i;
    lexem_pos = 0;
    
    return 0;
}

int lexer:: get_string ()
{
    char *check = fgets(curr_string, LEN_LEXER_BUF - 1, input_stream);
    
    if (feof(input_stream))
        return 1;
    
    if (check) {
        curr_string[strlen(curr_string)-1] = '\n';
        curr_string[strlen(curr_string)] = '\0';
        currstr_pos = 0;
        return 0;
    }
    
    if (strlen(curr_string)) {
        curr_string[strlen(curr_string)] = '\n';
        curr_string[strlen(curr_string)+1] = '\0';
        currstr_pos = 0;
        return 0;
    }
    
    return 2;
}

char * lexer:: get_cmp ()
{
    char *word;
    size_t len;
    int check = get_lexem(&word, &len);
    
    if (check == 1 || check == 2)
        return nullptr;
    
    return word;
}

