#include "internal_index.h"

int internal_index:: check_and_select (rbtree_node *x, server_string &serv_str, bool flag, char *s, command &c)
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
        
        x -> data -> print(serv_str, flag, s);
        return 1;
    }
    else { // by name
        for (i = 0; i < 2; ++i)
            if (c.cond[i].cond_left_t == cond_type::_NAME)
                break;
        
        // another one check of our actions :)
        switch (c.cond[i].type_t) {
            case cond_type::EQ: {
                if (strcmp(c.cond[i].str, (x -> data -> name).get()))
                    return 0;
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
        
        x -> data -> print(serv_str, flag, s);
        return 1;
    }
}

int internal_index:: select_into_group (server_string &serv_str, bool flag, char *s, command &c)
{
    int i, count = 0, group = head -> group;
    
    
    
    for (i = 0; i < 2; ++i)
        if (c.cond[i].cond_left_t == cond_type::_GROUP)
            break;
    
    // printf("NUMBER: %d\nGROUP: %d\n", c.cond[i].number, group);
    //another one check of our actions :)
    switch (c.cond[i].type_t) {
        case cond_type::EQ: {
            if (c.cond[i].number != group)
                return 0;
            break;
        }
        case cond_type::NE: {
            if (c.cond[i].number == group)
                return 0;
            break;
        }
        case cond_type::LT: {
            if (group >= c.cond[i].number)
                return 0;
            break;
        }
        case cond_type::GT: {
            if (group <= c.cond[i].number)
                return 0;
            break;
        }
        case cond_type::LE: {
            if (group > c.cond[i].number)
                return 0;
            break;
        }
        case cond_type::GE: {
            if (group < c.cond[i].number)
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
    
    if (c.oper[0] == op_type_t::OP_NONE) {
        // printf("BLACK\n");
        for (list_node *curr = head; curr; curr = curr -> into_next) {
            ++count;
            curr -> print(serv_str, flag, s);
        }
        return count;
    }
    
    if (phone) {
        for (i = 0; i < 2; ++i)
            if (c.cond[i].cond_left_t == cond_type::_PHONE)
                break;
        
        switch (c.cond[i].type_t) {
            case cond_type::EQ: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (curr -> phone == c.cond[i].number) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::NE: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (curr -> phone != c.cond[i].number) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::LT: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (curr -> phone < c.cond[i].number) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::GT: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (curr -> phone > c.cond[i].number) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::LE: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (curr -> phone <= c.cond[i].number) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::GE: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (curr -> phone >= c.cond[i].number) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::LIKE: {
                return 0;
            }
            case cond_type::COND_NONE: {
                return 0;
            }
        }
        
        return count;
    }
    else { // by name
        for (i = 0; i < 2; ++i)
            if (c.cond[i].cond_left_t == cond_type::_NAME)
                break;
        
        switch (c.cond[i].type_t) {
            case cond_type::EQ: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (!strcmp(c.cond[i].str, (curr -> name).get())) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::NE: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (strcmp(c.cond[i].str, (curr -> name).get())) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::LT: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (strcmp(c.cond[i].str, (curr -> name).get()) > 0) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::GT: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (strcmp(c.cond[i].str, (curr -> name).get()) < 0) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::LE: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (strcmp(c.cond[i].str, (curr -> name).get()) >= 0) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::GE: {
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (strcmp(c.cond[i].str, (curr -> name).get()) <= 0) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::LIKE: {
                c.split_cmd_str(c.cond[i].str);
                for (list_node *curr = head; curr; curr = curr -> into_next) {
                    if (c.strcmp_like(c.cond[i].str, (curr -> name).get())) {
                        ++count;
                        curr -> print(serv_str, flag, s);
                    }
                }
                break;
            }
            case cond_type::COND_NONE: {
                return 0;
            }
        }
        
        return count;
    }
}

int internal_index:: select_eq (int p, server_string &serv_str, bool flag, char *s, command &c)
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

int internal_index:: select_eq_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_eq_do(curr -> left, p, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_eq_do(curr -> right, p, serv_str, flag, s, c);
    
    return count;
}

int internal_index:: select_lt (int p, server_string &serv_str, bool flag, char *s, command &c)
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

int internal_index:: select_lt_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_lt_do(curr -> left, p, serv_str, flag, s, c);
    
    if (curr -> right -> data && curr -> right -> data -> phone < p)
        count += select_lt_do(curr -> right, p, serv_str, flag, s, c);
    
    return count;
}

int internal_index:: select_gt (int p, server_string &serv_str, bool flag, char *s, command &c)
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

int internal_index:: select_gt_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data && curr -> left -> data -> phone > p)
        count += select_gt_do(curr -> left, p, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_gt_do(curr -> right, p, serv_str, flag, s, c);
    
    return count;
}

int internal_index:: select_le (int p, server_string &serv_str, bool flag, char *s, command &c)
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

int internal_index:: select_le_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_le_do(curr -> left, p, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_le_do(curr -> right, p, serv_str, flag, s, c);
    
    return count;
}

int internal_index:: select_ge (int p, server_string &serv_str, bool flag, char *s, command &c)
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

int internal_index:: select_ge_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_ge_do(curr -> left, p, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_ge_do(curr -> right, p, serv_str, flag, s, c);
    
    return count;
}

int internal_index:: select_eq (char *str, server_string &serv_str, bool flag, char *s, command &c)
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

int internal_index:: select_eq_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_eq_do(curr -> left, str, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_eq_do(curr -> right, str, serv_str, flag, s, c);
    
    return count;
}

int internal_index:: select_lt (char *str, server_string &serv_str, bool flag, char *s, command &c)
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

int internal_index:: select_lt_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_lt_do(curr -> left, str, serv_str, flag, s, c);
    
    if (curr -> right -> data && strcmp((curr -> right -> data -> name).get(), str) < 0)
        count += select_lt_do(curr -> right, str, serv_str, flag, s, c);
    
    return count;
}

int internal_index:: select_gt (char *str, server_string &serv_str, bool flag, char *s, command &c)
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

int internal_index:: select_gt_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data && strcmp((curr -> left -> data -> name).get(), str) > 0)
        count += select_gt_do(curr -> left, str, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_gt_do(curr -> right, str, serv_str, flag, s, c);
    
    return count;
}

int internal_index:: select_le (char *str, server_string &serv_str, bool flag, char *s, command &c)
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

int internal_index:: select_le_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_le_do(curr -> left, str, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_le_do(curr -> right, str, serv_str, flag, s, c);
    
    return count;
}

int internal_index:: select_ge (char *str, server_string &serv_str, bool flag, char *s, command &c)
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

int internal_index:: select_ge_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, serv_str, flag, s, c);
    
    if (curr -> left -> data)
        count += select_ge_do(curr -> left, str, serv_str, flag, s, c);
    
    if (curr -> right -> data)
        count += select_ge_do(curr -> right, str, serv_str, flag, s, c);
    
    return count;
}

int internal_index:: cmd_select (server_string &serv_str, bool flag, char *s, command &c)
{
    if (c.oper[0] == op_type_t::OP_NONE)
        return select_into_group(serv_str, flag, s, c);
    
    int i, number = 0;
    char *str = nullptr;
    
    if (phone) {
        for (i = 0; i < 2; ++i) {
            if (c.cond[i].cond_left_t == cond_type::_PHONE) {
                number = c.cond[i].number;
                break;
            }
        }
        
        switch (c.cond[i].type_t) {
            case cond_type::EQ: {
                return select_eq(number, serv_str, flag, s, c);
            }
            case cond_type::NE: {
                return select_into_group(serv_str, flag, s, c);
            }
            case cond_type::LT: {
                return select_lt(number, serv_str, flag, s, c);
            }
            case cond_type::GT: {
                return select_gt(number, serv_str, flag, s, c);
            }
            case cond_type::GE: {
                return select_ge(number, serv_str, flag, s, c);
            }
            case cond_type::LE: {
                return select_le(number, serv_str, flag, s, c);
            }
            case cond_type::LIKE: {
                return 0;
            }
            case cond_type::COND_NONE: {
                return 0;
            }
        }
    }
    else {
        for (i = 0; i < 2; ++i) {
            if (c.cond[i].cond_left_t == cond_type::_NAME) {
                str = c.cond[i].str;
                break;
            }
        }
        
        switch (c.cond[i].type_t) {
            case cond_type::EQ: {
                return select_eq(str, serv_str, flag, s, c);
            }
            case cond_type::NE: {
                return select_into_group(serv_str, flag, s, c);
            }
            case cond_type::LT: {
                return select_lt(str, serv_str, flag, s, c);
            }
            case cond_type::GT: {
                return select_gt(str, serv_str, flag, s, c);
            }
            case cond_type::GE: {
                return select_ge(str, serv_str, flag, s, c);
            }
            case cond_type::LE: {
                return select_le(str, serv_str, flag, s, c);
            }
            case cond_type::LIKE: {
                return select_into_group(serv_str, flag, s, c);
            }
            case cond_type::COND_NONE: {
                return 0;
            }
        }
    }
    
    return 0;
}
