#include "external_index.h"

int external_index:: check_and_select (rbtree_node *x, server_string &serv_str, bool flag, char *s, command &c)
{
    int i, ph = x -> data -> phone;
    
    if (phone) { // by phone
        for (i = 0; i < 2; ++i)
            if (c.cond[i].cond_left_t == cond_type::_PHONE)
                break;
        
        // another one check of our actions :)
        switch (c.cond[i].type_t) {
            case cond_type::EQ: {
                if (c.cond[i].number != ph)
                    return 0;
                break;
            }
            case cond_type::NE: {
                if (c.cond[i].number == ph)
                    return 0;
                break;
            }
            case cond_type::LT: {
                if (ph >= c.cond[i].number)
                    return 0;
                break;
            }
            case cond_type::GT: {
                if (ph <= c.cond[i].number)
                    return 0;
                break;
            }
            case cond_type::LE: {
                if (ph > c.cond[i].number)
                    return 0;
                break;
            }
            case cond_type::GE: {
                if (ph < c.cond[i].number)
                    return 0;
                break;
            }
            case cond_type::LIKE: {
                return 0;
            }
            case cond_type::COND_NONE: {
                return 0;
            }
        }
        
        if (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::OP_NONE) {
            for (i = 0; i < 2; ++i) {
                if (c.cond[i].cond_left_t == cond_type::_NAME) {
                    switch (c.cond[i].type_t) {
                        case cond_type::EQ: {
                            if (!strcmp(x -> data -> get_name(), c.cond[i].str)) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str)) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) > 0) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LT: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) < 0) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GE: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) >= 0) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LE: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) <= 0) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LIKE: {
                            c.split_cmd_str(c.cond[i].str);
                            if (c.strcmp_like(c.cond[i].str, x -> data -> get_name())) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::COND_NONE: {
                            break;
                        }
                    }
                }
                else if (c.cond[i].cond_left_t == cond_type::_GROUP) {
                    switch (c.cond[i].type_t) {
                        case cond_type::EQ: {
                            if (x -> data -> group == c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (x -> data -> group != c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (x -> data -> group > c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LT: {
                            if (x -> data -> group < c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::GE: {
                            if (x -> data -> group >= c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LE: {
                            if (x -> data -> group <= c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LIKE: {
                            break;
                        }
                        case cond_type::COND_NONE: {
                            break;
                        }
                    }
                }
            }
        }
        else if (c.oper[0] == op_type_t::OP_NONE) {
            x -> data -> print(serv_str, flag, s);
            return 1;
        } // else not supported yet
    }
    else { // by name
        // printf("CHECKING\n");
        
        for (i = 0; i < 2; ++i)
            if (c.cond[i].cond_left_t == cond_type::_NAME)
                break;
        
        // printf("%d\n", i);
        
        // another one check of our actions :)
        switch (c.cond[i].type_t) {
            case cond_type::EQ: {
                //   printf("BEFORE\n");
                if (strcmp(c.cond[i].str, (x -> data -> name).get())) {
                    //     printf("AFTER\n");
                    return 0;
                }
                break;
            }
            case cond_type::NE: {
                if (!strcmp(c.cond[i].str, (x -> data -> name).get()))
                    return 0;
                break;
            }
            case cond_type::LT: {
                if (strcmp(c.cond[i].str, (x -> data -> name).get()) >= 0)
                    return 0;
                break;
            }
            case cond_type::GT: {
                if (strcmp(c.cond[i].str, (x -> data -> name).get()) <= 0)
                    return 0;
                break;
            }
            case cond_type::LE: {
                if (strcmp(c.cond[i].str, (x -> data -> name).get()) > 0)
                    return 0;
                break;
            }
            case cond_type::GE: {
                if (strcmp(c.cond[i].str, (x -> data -> name).get()) < 0)
                    return 0;
                break;
            }
            case cond_type::LIKE: {
                return 0;
            }
            case cond_type::COND_NONE: {
                return 0;
            }
        }
        
        // printf("AFTER\n");
        
        if (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::OP_NONE) {
            for (i = 0; i < 2; ++i) {
                if (c.cond[i].cond_left_t == cond_type::_PHONE) {
                    switch (c.cond[i].type_t) {
                        case cond_type::EQ: {
                            if (x -> data -> phone == c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (x -> data -> phone != c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (x -> data -> phone > c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LT: {
                            if (x -> data -> phone < c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::GE: {
                            if (x -> data -> phone >= c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LE: {
                            if (x -> data -> phone <= c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LIKE: {
                            break;
                        }
                        case cond_type::COND_NONE: {
                            break;
                        }
                    }
                }
                else if (c.cond[i].cond_left_t == cond_type::_GROUP) {
                    switch (c.cond[i].type_t) {
                        case cond_type::EQ: {
                            if (x -> data -> group == c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (x -> data -> group != c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (x -> data -> group > c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LT: {
                            if (x -> data -> group < c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::GE: {
                            if (x -> data -> group >= c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LE: {
                            if (x -> data -> group <= c.cond[i].number) {
                                x -> data -> print(serv_str, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LIKE: {
                            break;
                        }
                        case cond_type::COND_NONE: {
                            break;
                        }
                    }
                }
            }
        }
        else if (c.oper[0] == op_type_t::OP_NONE) {
            // printf("2 CHECK\n");
            x -> data -> print(serv_str, flag, s);
            return 1;
        } // else not supported yet
    }
    
    return 0;
}


int external_index:: select_eq (int p, server_string &serv_str, bool flag, char *s, command &c)
{
    bool check = false;
    
    for (curr = root; curr -> data; ) {
        if (curr -> data -> phone == p) {
            check = true;
            break;
        }
        
        if (p < curr -> data -> phone) // by phone
            goto_left();
        else
            goto_right();
    }
    
    if (check)
        return select_eq_do (curr, p, serv_str, flag, s, c);
    
    return 0;
}

int external_index:: select_eq_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    //print_subtree(curr);
    count += check_and_select(curr, serv_str, flag, s, c);
    //return 0;
    
    if (curr -> left -> data)
        count += select_eq_do(curr -> left, p, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_eq_do(curr -> right, p, serv_str, flag, s, c);
    
    return count;
}

int external_index:: select_lt (int p, server_string &serv_str, bool flag, char *s, command &c)
{
    bool check = false;
    
    for (curr = root; curr -> data; ) {
        if (curr -> data -> phone < p) {
            check = true;
            break;
        }
        
        goto_left();
    }
    
    if (check) {
        return select_lt_do (curr, p, serv_str, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_lt_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_lt_do(curr -> left, p, serv_str, flag, s, c);
    
    if (curr -> right -> data && curr -> right -> data -> phone < p)
        count += select_lt_do(curr -> right, p, serv_str, flag, s, c);
    
    return count;
}

int external_index:: select_gt (int p, server_string &serv_str, bool flag, char *s, command &c)
{
    bool check = false;
    
    for (curr = root; curr -> data; ) {
        if (curr -> data -> phone > p) {
            check = true;
            break;
        }
        
        goto_right();
    }
    
    if (check) {
        return select_gt_do (curr, p, serv_str, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_gt_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data && curr -> left -> data -> phone > p)
        count += select_gt_do(curr -> left, p, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_gt_do(curr -> right, p, serv_str, flag, s, c);
    
    return count;
}

int external_index:: select_le (int p, server_string &serv_str, bool flag, char *s, command &c)
{
    bool check = false;
    
    for (curr = root; curr -> data; ) {
        if (curr -> data -> phone <= p) {
            check = true;
            break;
        }
        
        goto_left();
    }
    
    if (check) {
        return select_le_do (curr, p, serv_str, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_le_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_le_do(curr -> left, p, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_le_do(curr -> right, p, serv_str, flag, s, c);
    
    return count;
}

int external_index:: select_ge (int p, server_string &serv_str, bool flag, char *s, command &c)
{
    bool check = false;
    
    for (curr = root; curr -> data; ) {
        if (curr -> data -> phone >= p) {
            check = true;
            break;
        }
        
        goto_right();
    }
    
    if (check) {
        return select_ge_do (curr, p, serv_str, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_ge_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_ge_do(curr -> left, p, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_ge_do(curr -> right, p, serv_str, flag, s, c);
    
    return count;
}

int external_index:: select_eq (char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    bool check = false;
    int res;
    
    for (curr = root; curr -> data; ) {
        res = strcmp(str, (curr -> data -> name).get());
        if (!res) {
            check = true;
            break;
        }
        
        if (res < 0) // by name
            goto_left();
        else
            goto_right();
    }
    
    if (check)
        return select_eq_do (curr, str, serv_str, flag, s, c);
    
    return 0;
}

int external_index:: select_eq_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    //print_subtree(curr);
    //curr -> data -> print();
    count += check_and_select(curr, serv_str, flag, s, c);
    //return 0;
    //printf("outer:\n");
    //outer.print();
    
    // if (curr -> left == &outer) {
    //   printf("ALARM\n");
    //}
    
    if (curr -> left -> data) {
        //curr -> left -> print();
        count += select_eq_do(curr -> left, str, serv_str, flag, s, c);
    }
    
    if (curr -> right -> data)
        count += select_eq_do(curr -> right, str, serv_str, flag, s, c);
    
    return count;
}

int external_index:: select_lt (char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    bool check = false;
    
    for (curr = root; curr -> data; ) {
        if (strcmp((curr -> data -> name).get(), str) < 0) {
            check = true;
            break;
        }
        
        goto_left();
    }
    
    if (check) {
        return select_lt_do (curr, str, serv_str, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_lt_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_lt_do(curr -> left, str, serv_str, flag, s, c);
    
    if (curr -> right -> data && strcmp((curr -> right -> data -> name).get(), str) < 0)
        count += select_lt_do(curr -> right, str, serv_str, flag, s, c);
    
    return count;
}

int external_index:: select_gt (char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    bool check = false;
    
    for (curr = root; curr -> data; ) {
        if (strcmp((curr -> data -> name).get(), str) > 0) {
            check = true;
            break;
        }
        
        goto_right();
    }
    
    if (check) {
        return select_gt_do (curr, str, serv_str, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_gt_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data && strcmp((curr -> left -> data -> name).get(), str) > 0)
        count += select_gt_do(curr -> left, str, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_gt_do(curr -> right, str, serv_str, flag, s, c);
    
    return count;
}

int external_index:: select_le (char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    bool check = false;
    
    for (curr = root; curr -> data; ) {
        if (strcmp((curr -> data -> name).get(), str) <= 0) {
            check = true;
            break;
        }
        
        goto_left();
    }
    
    if (check) {
        return select_le_do (curr, str, serv_str, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_le_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_le_do(curr -> left, str, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_le_do(curr -> right, str, serv_str, flag, s, c);
    
    return count;
}

int external_index:: select_ge (char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    bool check = false;
    
    for (curr = root; curr -> data; ) {
        if (strcmp((curr -> data -> name).get(), str) >= 0) {
            check = true;
            break;
        }
        
        goto_right();
    }
    
    if (check) {
        return select_ge_do (curr, str, serv_str, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_ge_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_ge_do(curr -> left, str, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_ge_do(curr -> right, str, serv_str, flag, s, c);
    
    return count;
}

// "group" / "group and smth" / "smth and group"
/* int external_index:: select_in_rbtree (command &c, server_string &serv_str)
{
    int i, ph_cond = 0, number = 0;
    
    for (i = 0; i < 3; ++i) {
        if (c.cond[i].cond_left_t == cond_type::_PHONE) {
            ph_cond = i;
            number = c.cond[i].number;
            break;
        }
    }
    
    long int number_str = 0;
    char s[4];
    
    if (!c.ast) {
        for (i = 0; i < 3; ++i) {
            if (c.f[i] == field_type::NAME) {
                if (!i)
                    number_str += 100;
                else if (i == 1)
                    number_str += 10;
                else
                    ++number_str;
            }
            else if (c.f[i] == field_type::PHONE) {
                if (!i)
                    number_str += 200;
                else if (i == 1)
                    number_str += 20;
                else
                    number_str += 2;
            }
            else if (c.f[i] == field_type::GROUP) {
                if (!i)
                    number_str += 300;
                else if (i == 1)
                    number_str += 30;
                else
                    number_str += 3;
            }
        }
        
        snprintf(s, 4, "%ld", number_str);
    }
    
    switch (c.cond[ph_cond].type_t) {
        case cond_type::EQ: {
            return select_eq(number, serv_str, c.ast, s, c);
        }
        case cond_type::NE: {
            return a.apply_cmd(c, serv_str);
        }
        case cond_type::LT: {
            return select_lt(number, serv_str, c.ast, s, c);
        }
        case cond_type::GT: {
            return select_gt(number, serv_str, c.ast, s, c);
        }
        case cond_type::GE: {
            return select_ge(number, serv_str, c.ast, s, c);
        }
        case cond_type::LE: {
            return select_le(number, serv_str, c.ast, s, c);
        }
        case cond_type::LIKE: {
            return 0;
        }
        case cond_type::COND_NONE: {
            return 0;
        }
    }
    
    return 0;
} */

int external_index:: apply_cmd (command &c, server_string &serv_str)
{
    int i, cond = 0, number = 0;
    char *str = nullptr;
    
    
    if (phone) {
        for (i = 0; i < 2; ++i) {
            if (c.cond[i].cond_left_t == cond_type::_PHONE) {
                cond = i;
                number = c.cond[i].number;
                break;
            }
        }
    }
    else {
        for (i = 0; i < 2; ++i) {
            if (c.cond[i].cond_left_t == cond_type::_NAME) {
                cond = i;
                str = c.cond[i].str;
                break;
            }
        }
    }
    
    switch (c.type) {
        case cmd_type::SELECT: {
            long int number_str = 0;
            char s[4];
            
            if (!c.ast) {
                for (i = 0; i < 3; ++i) {
                    if (c.f[i] == field_type::NAME) {
                        if (!i)
                            number_str += 100;
                        else if (i == 1)
                            number_str += 10;
                        else
                            ++number_str;
                    }
                    else if (c.f[i] == field_type::PHONE) {
                        if (!i)
                            number_str += 200;
                        else if (i == 1)
                            number_str += 20;
                        else
                            number_str += 2;
                    }
                    else if (c.f[i] == field_type::GROUP) {
                        if (!i)
                            number_str += 300;
                        else if (i == 1)
                            number_str += 30;
                        else
                            number_str += 3;
                    }
                }
                
                snprintf(s, 4, "%ld", number_str);
            }
            
            if (phone) {
                switch (c.cond[cond].type_t) {
                    case cond_type::EQ: {
                        return select_eq(number, serv_str, c.ast, s, c);
                    }
                    case cond_type::NE: {
                        return a.apply_cmd(c, serv_str);
                    }
                    case cond_type::LT: {
                        return select_lt(number, serv_str, c.ast, s, c);
                    }
                    case cond_type::GT: {
                        return select_gt(number, serv_str, c.ast, s, c);
                    }
                    case cond_type::GE: {
                        return select_ge(number, serv_str, c.ast, s, c);
                    }
                    case cond_type::LE: {
                        return select_le(number, serv_str, c.ast, s, c);
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                return 0;
            }
            else {
                switch (c.cond[cond].type_t) {
                    case cond_type::EQ: {
                        return select_eq(str, serv_str, c.ast, s, c);
                    }
                    case cond_type::NE: {
                        return a.apply_cmd(c, serv_str);
                    }
                    case cond_type::LT: {
                        return select_lt(str, serv_str, c.ast, s, c);
                    }
                    case cond_type::GT: {
                        return select_gt(str, serv_str, c.ast, s, c);
                    }
                    case cond_type::GE: {
                        return select_ge(str, serv_str, c.ast, s, c);
                    }
                    case cond_type::LE: {
                        return select_le(str, serv_str, c.ast, s, c);
                    }
                    case cond_type::LIKE: {
                        return a.apply_cmd(c, serv_str);
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                return 0;
            }
        }
        case cmd_type::DELETE: {
            if (phone) {
                switch (c.cond[cond].type_t) {
                    case cond_type::EQ: {
                        delete_eq(number, c);
                        break;
                    }
                    case cond_type::NE: {
                        mark_list(c);
                        break;
                    }
                    case cond_type::LT: {
                        delete_lt(number, c);
                        break;
                    }
                    case cond_type::GT: {
                        delete_gt(number, c);
                        break;
                    }
                    case cond_type::GE: {
                        delete_ge(number, c);
                        break;
                    }
                    case cond_type::LE: {
                        delete_le(number, c);
                        break;
                    }
                    case cond_type::LIKE: {
                        break;
                    }
                    case cond_type::COND_NONE: {
                        break;
                    }
                }
                
                return 0;
            }
            else {
                switch (c.cond[cond].type_t) {
                    case cond_type::EQ: {
                        delete_eq(str, c);
                        break;
                    }
                    case cond_type::NE: {
                        mark_list(c);
                        break;
                    }
                    case cond_type::LT: {
                        delete_lt(str, c);
                        break;
                    }
                    case cond_type::GT: {
                        delete_gt(str, c);
                        break;
                    }
                    case cond_type::GE: {
                        delete_ge(str, c);
                        break;
                    }
                    case cond_type::LE: {
                        delete_le(str, c);
                        break;
                    }
                    case cond_type::LIKE: {
                        mark_list(c);
                        break;
                    }
                    case cond_type::COND_NONE: {
                        break;
                    }
                }
                
                return 0;
            }
        }
        case cmd_type::INSERT: {
            return -4;
        }
        case cmd_type::PERR: {
            return -5;
        }
            
        case cmd_type::VOID: {
            return -6;
        }
            
        case cmd_type::STOP: {
            return -7;
        }
            
        case cmd_type::QUIT: {
            return -8;
        }
            
        case cmd_type::CMD_NONE: {
            return -9;
        }
    }
    
    return 0;
}


