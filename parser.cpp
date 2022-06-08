#include "parser.h"
#include "command.h"
#include <cstdio>

command parser:: parse_query ()
{
    curr_pos = 0;
    
    command c;
    c.type = cmd_type::CMD_NONE;
    int i;
    for (i = 0; i < 3; ++i)
        c.f[i] = field_type::FIELD_NONE;
    for (i = 0; i < 3; ++i) {
        c.cond[i].cond_left_t = cond_type::_FIELD_NONE;
        c.cond[i].type_t = cond_type::COND_NONE;
    }
    for (i = 0; i < 2; ++i)
        c.oper[i] = op_type_t::OP_NONE;
    
    token a = get_token();
    switch (a.type) {
        case token::SELECT: {
            c.type = cmd_type::SELECT;
            a = get_token();
            if (a.type == token::AST) {
                c.ast = true;
                a = get_token();
            }
            else
                for (int i = 0; i < 3; ++i) {
                    if (a.type == token::NAME)
                        c.f[i] = field_type::NAME;
                    else if (a.type == token::PHONE)
                        c.f[i] = field_type::PHONE;
                    else if (a.type == token::GROUP)
                        c.f[i] = field_type::GROUP;
                    else {
                        c.type = cmd_type::PERR;
                        return c;
                    }
                    
                    a = get_token();
                    if (a.type == token::COMMA)
                        a = get_token();
                    else
                        break;
                }
            
            if (a.type == token::ENQ)
                return c;
            
            if (a.type != token::WHERE) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            if (!parse_where(c)) {
                c.type = cmd_type::PERR;
                return c;
            }
            return c;
        }
        case token::INSERT: {
            c.type = cmd_type::INSERT;
            a = get_token();
            if (a.type != token::LP) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            a = get_token();
            if (a.type != token::OTHER) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            // here cmp_with is not a pointer for cmp but it is the pointer to 'OTHER' word
            c.set_name(cmp_copy(a.cmp_with));
            
            a = get_token();
            if (a.type != token::COMMA) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            a = get_token();
            if (a.type != token::OTHER) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            int number;
            bool int_check = get_int(a.cmp_with, &number);
            if (!int_check) {
                c.type = cmd_type::PERR;
                return c;
            }
            c.set_phone(number);
            
            a = get_token();
            if (a.type != token::COMMA) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            a = get_token();
            if (a.type != token::OTHER) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            int_check = get_int(a.cmp_with, &number);
            if (!int_check) {
                c.type = cmd_type::PERR;
                return c;
            }
            c.set_group(number);
            
            a = get_token();
            if (a.type != token::RP) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            a = get_token();
            if (a.type != token::ENQ) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            return c;
        }
        case token::DELETE: {
            c.type = cmd_type::DELETE;
            a = get_token();
            if (a.type == token::ENQ)
                return c;
            
            if (a.type != token::WHERE) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            if (!parse_where(c)) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            return c;
        }
        case token::STOP: {
            c.type = cmd_type::STOP;
            a = get_token();
            if (a.type != token::ENQ) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            return c;
        }
        case token::QUIT: {
            c.type = cmd_type::QUIT;
            a = get_token();
            if (a.type != token::ENQ) {
                c.type = cmd_type::PERR;
                return c;
            }
            
            return c;
        }
        case token::VOID: {
            c.type = cmd_type::VOID;
            break;
        }
        default: {
            c.type = cmd_type::PERR;
            break;
        }
    }
    
    return c;
}

bool parser:: get_int (char *str, int *res)
{
    char *next;
    long int number = strtol(str, &next, 10);
    
    if (next == str || *next)
        return false;
    
    *res = number;
    
    return true;
}

bool parser:: parse_where (command &c)
{
    for (int i = 0; i < 3; ++i) {
        token a = get_token();
        bool int_check = false;
        if (a.type == token::NAME)
            c.cond[i].cond_left_t = cond_type::_NAME;
        else if (a.type == token::PHONE) {
            c.cond[i].cond_left_t = cond_type::_PHONE;
            int_check = true;
        }
        else if (a.type == token::GROUP) {
            c.cond[i].cond_left_t = cond_type::_GROUP;
            int_check = true;
        }
        else
            return false;
        
        a = get_token();
        switch (a.type) {
            case token::EQ:
                c.cond[i].type_t = cond_type::EQ;
                break;
            case token::NE:
                c.cond[i].type_t = cond_type::NE;
                break;
            case token::LT:
                c.cond[i].type_t = cond_type::LT;
                break;
            case token::LE:
                c.cond[i].type_t = cond_type::LE;
                break;
            case token::GT:
                c.cond[i].type_t = cond_type::GT;
                break;
            case token::GE:
                c.cond[i].type_t = cond_type::GE;
                break;
            case token::LIKE:
                c.cond[i].type_t = cond_type::LIKE;
                break;
            default:
                return false;
        }
        
        if (int_check && !get_int(a.cmp_with, &(c.cond[i].number)))
            return false;
        else if (!int_check)
            c.cond[i].str = cmp_copy(a.cmp_with);
        
        a = get_token();
        if (a.type == token::AND)
            c.oper[i] = op_type_t::AND;
        else if (a.type == token::OR)
            c.oper[i] = op_type_t::OR;
        else if (a.type == token::ENQ)
            return true;
    }
    
    token a = get_token();
    if (a.type != token::ENQ)
        return false;
    
    return true;
}

char * parser:: cmp_copy (char *cmp_with)
{
    if (!cmp_with || curr_pos >= LEN_PARSER_BUF)
        return nullptr;
    
    size_t len = strlen(cmp_with);
    if (len >= LEN_PARSER_BUF - curr_pos)
        return nullptr;
    
    char *add = buf + curr_pos;
    strcpy(add, cmp_with);
    
    curr_pos += (len + 1);
    
    return add;
}

