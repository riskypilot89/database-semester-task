#include "command.h"

void command:: split_cmd_str (char *cmd_str)
{
    _clear_cmd_str_copy();
    input_cmd_str_copy(cmd_str);
    _words_cmd_str_copy();
}

void command:: _clear_cmd_str_copy ()
{
    for (size_t i = 0; cmd_str_copy[i]; ++i)
        cmd_str_copy[i] = '\0';
}

void command:: input_cmd_str_copy (char *cmd_str)
{
    if (cmd_str) {
        strcpy(cmd_str_copy, cmd_str);
        
        for (size_t i = 0; cmd_str_copy[i]; ++i) {
            if (cmd_str_copy[i] == '\n') {
                cmd_str_copy[i] = '\0';
                break;
            }
        }
    }
}

void command:: _words_cmd_str_copy ()
{
    size_t i;
    
    len_wcsc = 0;
    for (i = 0; words_cmd_str_copy[i]; ++i)
        words_cmd_str_copy[i] = 0;
    
    for (i = 0; cmd_str_copy[i] && cmd_str_copy[i] == '%'; ++i)
        ;
    
    if (!cmd_str_copy[i])
        return;
    
    words_cmd_str_copy[len_wcsc++] = cmd_str_copy + i;
    
    ++i;
    for ( ; cmd_str_copy[i]; ) {
        if (cmd_str_copy[i] == '%') {
            cmd_str_copy[i++] = '\0';
            
            for ( ; cmd_str_copy[i] && cmd_str_copy[i] == '%'; ++i)
                ;
            
            if (cmd_str_copy[i]) {
                words_cmd_str_copy[len_wcsc++] = cmd_str_copy + i;
                ++i;
            }
        }
        else
            ++i;
    }
}

// cmd_str: "where name like *cmd_str*"
// str: a name from our list of data
bool command:: strcmp_like (char *cmd_str, char *str)
{
    //split_str(str); // we split our string into words (now we have only "%" as delimiter and suppose cmd_str divided already)
    
    if (!words_cmd_str_copy[0]) { // if cmd_str is "%%%%%%%%%..."
        return true;
    }
    
    // these variables are the lengths of arrays of "words"
    size_t place = 0, saved = 0;
    bool flag = false;
    place = solve_like_strcmp(str, words_cmd_str_copy[0], 0, &flag); // where is the end of including words_cmd_str[0] into the str string
    // for example: cmd_str == "%a%b....", str == "*SOMETHING*a%b...."
    // this place is exactly the next symbol after the words_cmd_str[0] string
    // it is also a very good marker to begin finding next places (for words_cmd_str[1] ... and so on) – to not go (on the string) again
    
    if (cmd_str[0] != '%' && place && !flag) // example: cmd_str == "a%b....", str == "*SOMETHING*a..." – we found 'a'
        place = 0;                                                        // ("a" and "*SOMETING*a" are two words exactly
    
    if (place) { // then we need to find more :)
        size_t k;
        for (k = 1; words_cmd_str_copy[k] && words_cmd_str_copy[k+1]; ++k) { // when we have minimum two words at least
            place = solve_like_strcmp(str, words_cmd_str_copy[k], place, &flag);
            if (!place) // 404 – not found :))
                break;
        }
        
        if (words_cmd_str_copy[k] && !words_cmd_str_copy[k+1] && place) { // the last one
            saved = place = solve_like_strcmp(str, words_cmd_str_copy[k], place, &flag);
            while (place) {
                place = solve_like_strcmp(str, words_cmd_str_copy[k], place, &flag);
                if (place)
                    saved = place;
            } // this while cycle needed for example when cmd_str == "a%b%c" and str == "a*SMTH*b*SMTH*c*SMTH*d*SMTH*c"
            // just for knowledge that our str ends on "c" (in this one)
            place = saved;
            
            if (cmd_str[strlen(cmd_str)-1] != '%' && place != strlen(str))
                place = 0;
        }
        else if (k == 1 && !words_cmd_str_copy[k] && place) {
            saved = place;
            while (place) {
                place = solve_like_strcmp(str, words_cmd_str_copy[0], place, &flag);
                if (place)
                    saved = place;
            }
            place = saved;
            
            if (cmd_str[strlen(cmd_str)-1] != '%' && place != strlen(str))
                place = 0;
        }
    }
    
    if (place)
        return true;
    
    return false;
}

// flag is for searching a special situation when we find s2 into s1 as the beginning of s1 (marker == 0)
size_t command:: solve_like_strcmp (char *s1, char *s2, size_t marker, bool *flag)
{
    size_t i, j, k, p;
    
    for (i = marker; s1[i]; ++i) { // (i = marker) <= we think our data is correct
        j = 0;
        if (s2[0] == s1[i] || s2[0] == '_') {
            if (!i)
                *flag = true;
            
            k = i + 1;
            p = j + 1;
            
            while (s2[p] && s1[k]) {
                if (s2[p] == s1[k] || s2[p] == '_') {
                    ++k;
                    ++p;
                }
                else {
                    *flag = false; // pokazalos' :-)
                    break;
                }
            }
            
            if (!s2[p])
                return k;
        }
    }
    
    return 0; // means "not found"
}

void command:: print_cmd (FILE *fp)
{
    switch (type) {
        case cmd_type::CMD_NONE: {
            break;
        }
        case cmd_type::QUIT: {
            fprintf(fp, "quit;\n");
            break;
        }
        case cmd_type::STOP: {
            fprintf(fp, "stop;\n");
            break;
        }
        case cmd_type::SELECT: {
            fprintf(fp, "select ");
            
            int i;
            bool where_check;
            if (!ast) {
                for (i = 0; i < 3; ++i) {
                    if (f[i] == field_type::NAME) {
                        fprintf(fp, "name");
                        if (i < 2 && f[i+1] != field_type::FIELD_NONE)
                            fprintf(fp, ", ");
                    }
                    else if (f[i] == field_type::PHONE) {
                        fprintf(fp, "phone");
                        if (i < 2 && f[i+1] != field_type::FIELD_NONE)
                            fprintf(fp, ", ");
                    }
                    else if (f[i] == field_type::GROUP) {
                        fprintf(fp, "group");
                        if (i < 2 && f[i+1] != field_type::FIELD_NONE)
                            fprintf(fp, ", ");
                    }
                    
                }
                
                where_check = false;
                for (i = 0; i < 3; ++i)
                    if (cond[i].cond_left_t != cond_type::_FIELD_NONE) {
                        where_check = true;
                        break;
                    }
                
                if (where_check)
                    fprintf(fp, " where ");
                else {
                    fprintf(fp, ";\n");
                    break;
                }
                
                for (i = 0; i < 3; ++i) {
                    if (cond[i].cond_left_t == cond_type::_NAME) {
                        fprintf(fp, "name ");
                        switch (cond[i].type_t) {
                            case cond_type::EQ: {
                                fprintf(fp, "= ");
                                break;
                            }
                            case cond_type::NE: {
                                fprintf(fp, "<> ");
                                break;
                            }
                            case cond_type::LT: {
                                fprintf(fp, "< ");
                                break;
                            }
                            case cond_type::GT: {
                                fprintf(fp, "> ");
                                break;
                            }
                            case cond_type::LE: {
                                fprintf(fp, "<= ");
                                break;
                            }
                            case cond_type::GE: {
                                fprintf(fp, ">= ");
                                break;
                            }
                            default: {
                                fprintf(fp, "like ");
                            }
                        }
                        
                        fprintf(fp, "%s", cond[i].str);
                        
                        if (i < 2 && oper[i] != op_type_t::OP_NONE) {
                            if (oper[i] == op_type_t::AND)
                                fprintf(fp, " and ");
                            else
                                fprintf(fp, " or ");
                        }
                    }
                    else if (cond[i].cond_left_t == cond_type::_PHONE) {
                        fprintf(fp, "phone ");
                        switch (cond[i].type_t) {
                            case cond_type::EQ: {
                                fprintf(fp, "= ");
                                break;
                            }
                            case cond_type::NE: {
                                fprintf(fp, "<> ");
                                break;
                            }
                            case cond_type::LT: {
                                fprintf(fp, "< ");
                                break;
                            }
                            case cond_type::GT: {
                                fprintf(fp, "> ");
                                break;
                            }
                            case cond_type::LE: {
                                fprintf(fp, "<= ");
                                break;
                            }
                            case cond_type::GE: {
                                fprintf(fp, ">= ");
                                break;
                            }
                            default: {
                                fprintf(fp, "like ");
                            }
                        }
                        
                        fprintf(fp, "%d", cond[i].number);
                        
                        if (i < 2 && oper[i] != op_type_t::OP_NONE) {
                            if (oper[i] == op_type_t::AND)
                                fprintf(fp, " and ");
                            else
                                fprintf(fp, " or ");
                        }
                    }
                    else if (cond[i].cond_left_t == cond_type::_GROUP) {
                        fprintf(fp, "group ");
                        switch (cond[i].type_t) {
                            case cond_type::EQ: {
                                fprintf(fp, "= ");
                                break;
                            }
                            case cond_type::NE: {
                                fprintf(fp, "<> ");
                                break;
                            }
                            case cond_type::LT: {
                                fprintf(fp, "< ");
                                break;
                            }
                            case cond_type::GT: {
                                fprintf(fp, "> ");
                                break;
                            }
                            case cond_type::LE: {
                                fprintf(fp, "<= ");
                                break;
                            }
                            case cond_type::GE: {
                                fprintf(fp, ">= ");
                                break;
                            }
                            default: {
                                fprintf(fp, "like ");
                            }
                        }
                        
                        fprintf(fp, "%d", cond[i].number);
                        
                        if (i < 2 && oper[i] != op_type_t::OP_NONE) {
                            if (oper[i] == op_type_t::AND)
                                fprintf(fp, " and ");
                            else
                                fprintf(fp, " or ");
                        }
                    }
                    
                }
            }
            else {
                fprintf(fp, "*");
                where_check = false;
                for (i = 0; i < 3; ++i)
                    if (cond[i].cond_left_t != cond_type::_FIELD_NONE) {
                        where_check = true;
                        break;
                    }
                
                if (where_check)
                    fprintf(fp, " where ");
                else {
                    fprintf(fp, ";\n");
                    break;
                }
                
                for (i = 0; i < 3; ++i) {
                    if (cond[i].cond_left_t == cond_type::_NAME) {
                        fprintf(fp, "name ");
                        switch (cond[i].type_t) {
                            case cond_type::EQ: {
                                fprintf(fp, "= ");
                                break;
                            }
                            case cond_type::NE: {
                                fprintf(fp, "<> ");
                                break;
                            }
                            case cond_type::LT: {
                                fprintf(fp, "< ");
                                break;
                            }
                            case cond_type::GT: {
                                fprintf(fp, "> ");
                                break;
                            }
                            case cond_type::LE: {
                                fprintf(fp, "<= ");
                                break;
                            }
                            case cond_type::GE: {
                                fprintf(fp, ">= ");
                                break;
                            }
                            default: {
                                fprintf(fp, "like ");
                            }
                        }
                        
                        fprintf(fp, "%s", cond[i].str);
                        
                        if (i < 2 && oper[i] != op_type_t::OP_NONE) {
                            if (oper[i] == op_type_t::AND)
                                fprintf(fp, " and ");
                            else
                                fprintf(fp, " or ");
                        }
                    }
                    else if (cond[i].cond_left_t == cond_type::_PHONE) {
                        fprintf(fp, "phone ");
                        switch (cond[i].type_t) {
                            case cond_type::EQ: {
                                fprintf(fp, "= ");
                                break;
                            }
                            case cond_type::NE: {
                                fprintf(fp, "<> ");
                                break;
                            }
                            case cond_type::LT: {
                                fprintf(fp, "< ");
                                break;
                            }
                            case cond_type::GT: {
                                fprintf(fp, "> ");
                                break;
                            }
                            case cond_type::LE: {
                                fprintf(fp, "<= ");
                                break;
                            }
                            case cond_type::GE: {
                                fprintf(fp, ">= ");
                                break;
                            }
                            default: {
                                fprintf(fp, "like ");
                            }
                        }
                        
                        fprintf(fp, "%d", cond[i].number);
                        
                        if (i < 2 && oper[i] != op_type_t::OP_NONE) {
                            if (oper[i] == op_type_t::AND)
                                fprintf(fp, " and ");
                            else
                                fprintf(fp, " or ");
                        }
                    }
                    else if (cond[i].cond_left_t == cond_type::_GROUP) {
                        fprintf(fp, "group ");
                        switch (cond[i].type_t) {
                            case cond_type::EQ: {
                                fprintf(fp, "= ");
                                break;
                            }
                            case cond_type::NE: {
                                fprintf(fp, "<> ");
                                break;
                            }
                            case cond_type::LT: {
                                fprintf(fp, "< ");
                                break;
                            }
                            case cond_type::GT: {
                                fprintf(fp, "> ");
                                break;
                            }
                            case cond_type::LE: {
                                fprintf(fp, "<= ");
                                break;
                            }
                            case cond_type::GE: {
                                fprintf(fp, ">= ");
                                break;
                            }
                            default: {
                                fprintf(fp, "like ");
                            }
                        }
                        
                        fprintf(fp, "%d", cond[i].number);
                        
                        if (i < 2 && oper[i] != op_type_t::OP_NONE) {
                            if (oper[i] == op_type_t::AND)
                                fprintf(fp, " and ");
                            else
                                fprintf(fp, " or ");
                        }
                    }
                    
                }
            }
            
            fprintf(fp, ";\n");
            break;
        }
        case cmd_type::INSERT: {
            fprintf(fp, "insert (%s, %d, %d);\n", get_name(), get_phone(), get_group());
            break;
        }
        case cmd_type::DELETE: {
            fprintf(fp, "delete");
            
            int i;
            bool where_check = false;
            
            for (i = 0; i < 3; ++i)
                if (cond[i].cond_left_t != cond_type::_FIELD_NONE) {
                    where_check = true;
                    break;
                }
            
            if (where_check) {
                fprintf(fp, " where ");
                
                for (i = 0; i < 3; ++i) {
                    if (cond[i].cond_left_t == cond_type::_NAME) {
                        fprintf(fp, "name ");
                        switch (cond[i].type_t) {
                            case cond_type::EQ: {
                                fprintf(fp, "= ");
                                break;
                            }
                            case cond_type::NE: {
                                fprintf(fp, "<> ");
                                break;
                            }
                            case cond_type::LT: {
                                fprintf(fp, "< ");
                                break;
                            }
                            case cond_type::GT: {
                                fprintf(fp, "> ");
                                break;
                            }
                            case cond_type::LE: {
                                fprintf(fp, "<= ");
                                break;
                            }
                            case cond_type::GE: {
                                fprintf(fp, ">= ");
                                break;
                            }
                            default: {
                                fprintf(fp, "like ");
                            }
                        }
                        
                        fprintf(fp, "%s", cond[i].str);
                        
                        if (i < 2 && oper[i] != op_type_t::OP_NONE) {
                            if (oper[i] == op_type_t::AND)
                                fprintf(fp, " and ");
                            else
                                fprintf(fp, " or ");
                        }
                    }
                    else if (cond[i].cond_left_t == cond_type::_PHONE) {
                        fprintf(fp, "phone ");
                        switch (cond[i].type_t) {
                            case cond_type::EQ: {
                                fprintf(fp, "= ");
                                break;
                            }
                            case cond_type::NE: {
                                fprintf(fp, "<> ");
                                break;
                            }
                            case cond_type::LT: {
                                fprintf(fp, "< ");
                                break;
                            }
                            case cond_type::GT: {
                                fprintf(fp, "> ");
                                break;
                            }
                            case cond_type::LE: {
                                fprintf(fp, "<= ");
                                break;
                            }
                            case cond_type::GE: {
                                fprintf(fp, ">= ");
                                break;
                            }
                            default: {
                                fprintf(fp, "like ");
                            }
                        }
                        
                        fprintf(fp, "%d", cond[i].number);
                        
                        if (i < 2 && oper[i] != op_type_t::OP_NONE) {
                            if (oper[i] == op_type_t::AND)
                                fprintf(fp, " and ");
                            else
                                fprintf(fp, " or ");
                        }
                    }
                    else if (cond[i].cond_left_t == cond_type::_GROUP) {
                        fprintf(fp, "group ");
                        switch (cond[i].type_t) {
                            case cond_type::EQ: {
                                fprintf(fp, "= ");
                                break;
                            }
                            case cond_type::NE: {
                                fprintf(fp, "<> ");
                                break;
                            }
                            case cond_type::LT: {
                                fprintf(fp, "< ");
                                break;
                            }
                            case cond_type::GT: {
                                fprintf(fp, "> ");
                                break;
                            }
                            case cond_type::LE: {
                                fprintf(fp, "<= ");
                                break;
                            }
                            case cond_type::GE: {
                                fprintf(fp, ">= ");
                                break;
                            }
                            default: {
                                fprintf(fp, "like ");
                            }
                        }
                        
                        fprintf(fp, "%d", cond[i].number);
                        
                        if (i < 2 && oper[i] != op_type_t::OP_NONE) {
                            if (oper[i] == op_type_t::AND)
                                fprintf(fp, " and ");
                            else
                                fprintf(fp, " or ");
                        }
                    }
                    
                }
            }
            
            fprintf(fp, ";\n");
            break;
        }
        case cmd_type::VOID: {
            fprintf(fp, "\n");
            break;
        }
        default:
            break;
    }
    
}
