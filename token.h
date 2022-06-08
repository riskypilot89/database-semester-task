#ifndef TOKEN_H
#define TOKEN_H

struct token
{
    enum token_type
    {
        // command types:
        QUIT, STOP, SELECT, INSERT, DELETE,
        // special words
        WHERE,
        // fields:
        AST, NAME, PHONE, GROUP,
        // (AST - asterisk)
        // condition types:
        EQ, NE, LT, GT, LE, GE, LIKE,
        // operation types (logic):
        AND, OR,
        // other:
        EF, COMMA, ENQ, LP, RP, FERR, LERR, OTHER, VOID,
        // (EF – End of File, LP - left parentheses, RP - right parentheses, ENQ - ENd of Query, FERR – error while reading File or other issue with input stream, LERR – error with Lexer while parsing string (this string is not good for a query), VOID – void space like a "\n")
    } type;
    char *cmp_with; // it is for condition types (when we need compare some strings)
};

#endif
