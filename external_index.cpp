#include "external_index.h"
//#include "rbtree.h"
//#define NIL &outer

external_index:: external_index (list &a, bool phone)
: rbtree(phone), a(a) {}

external_index:: ~external_index () {}

/*int external_index:: initialize_rbtree ()
{
    a.goto_head();
    while (a.curr != nullptr) {
        if (add(a.curr)) {
            if (root != NIL)
                delete_rbtree(root);
            root = curr = NIL;
            return 1;
        }
        a.goto_next();
    }
    
    return 0;
}*/

int external_index:: check_and_select (rbtree_node *x, FILE *fp, bool flag, char *s, command &c)
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
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str)) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) > 0) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LT: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) < 0) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GE: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) >= 0) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LE: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) <= 0) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LIKE: {
                            c.split_cmd_str(c.cond[i].str);
                            if (c.strcmp_like(c.cond[i].str, x -> data -> get_name())) {
                                x -> data -> print(fp, flag, s);
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
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (x -> data -> group != c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (x -> data -> group > c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LT: {
                            if (x -> data -> group < c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::GE: {
                            if (x -> data -> group >= c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LE: {
                            if (x -> data -> group <= c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
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
            x -> data -> print(fp, flag, s);
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
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (x -> data -> phone != c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (x -> data -> phone > c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LT: {
                            if (x -> data -> phone < c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::GE: {
                            if (x -> data -> phone >= c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LE: {
                            if (x -> data -> phone <= c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
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
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (x -> data -> group != c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (x -> data -> group > c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LT: {
                            if (x -> data -> group < c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::GE: {
                            if (x -> data -> group >= c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
                                return 1;
                            }
                            break;
                            
                        }
                        case cond_type::LE: {
                            if (x -> data -> group <= c.cond[i].number) {
                                x -> data -> print(fp, flag, s);
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
            x -> data -> print(fp, flag, s);
            return 1;
        } // else not supported yet
    }
    
    return 0;
}


int external_index:: select_eq (int p, FILE *fp, bool flag, char *s, command &c)
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
        return select_eq_do (curr, p, fp, flag, s, c);
    
    return 0;
}

int external_index:: select_eq_do (rbtree_node *curr, int p, FILE *fp, bool flag, char *s, command &c)
{
    int count = 0;
    
    //print_subtree(curr);
    count += check_and_select(curr, fp, flag, s, c);
    //return 0;
    
    if (curr -> left -> data)
        count += select_eq_do(curr -> left, p, fp, flag, s, c);
    
    if (curr -> right -> data)
        count += select_eq_do(curr -> right, p, fp, flag, s, c);
    
    return count;
}

int external_index:: select_lt (int p, FILE *fp, bool flag, char *s, command &c)
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
        return select_lt_do (curr, p, fp, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_lt_do (rbtree_node *curr, int p, FILE *fp, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, fp, flag, s, c);
    
    if (curr -> left -> data)
        count += select_lt_do(curr -> left, p, fp, flag, s, c);
    
    if (curr -> right -> data && curr -> right -> data -> phone < p)
        count += select_lt_do(curr -> right, p, fp, flag, s, c);
    
    return count;
}

int external_index:: select_gt (int p, FILE *fp, bool flag, char *s, command &c)
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
        return select_gt_do (curr, p, fp, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_gt_do (rbtree_node *curr, int p, FILE *fp, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, fp, flag, s, c);
    
    if (curr -> left -> data && curr -> left -> data -> phone > p)
        count += select_gt_do(curr -> left, p, fp, flag, s, c);
    
    if (curr -> right -> data)
        count += select_gt_do(curr -> right, p, fp, flag, s, c);
    
    return count;
}

int external_index:: select_le (int p, FILE *fp, bool flag, char *s, command &c)
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
        return select_le_do (curr, p, fp, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_le_do (rbtree_node *curr, int p, FILE *fp, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, fp, flag, s, c);
    
    if (curr -> left -> data)
        count += select_le_do(curr -> left, p, fp, flag, s, c);
    
    if (curr -> right -> data)
        count += select_le_do(curr -> right, p, fp, flag, s, c);
    
    return count;
}

int external_index:: select_ge (int p, FILE *fp, bool flag, char *s, command &c)
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
        return select_ge_do (curr, p, fp, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_ge_do (rbtree_node *curr, int p, FILE *fp, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, fp, flag, s, c);
    
    if (curr -> left -> data)
        count += select_ge_do(curr -> left, p, fp, flag, s, c);
    
    if (curr -> right -> data)
        count += select_ge_do(curr -> right, p, fp, flag, s, c);
    
    return count;
}

int external_index:: select_eq (char *str, FILE *fp, bool flag, char *s, command &c)
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
        return select_eq_do (curr, str, fp, flag, s, c);
    
    return 0;
}

int external_index:: select_eq_do (rbtree_node *curr, char *str, FILE *fp, bool flag, char *s, command &c)
{
    int count = 0;
    
    //print_subtree(curr);
    //curr -> data -> print();
    count += check_and_select(curr, fp, flag, s, c);
    //return 0;
    //printf("outer:\n");
    //outer.print();
    
   // if (curr -> left == &outer) {
     //   printf("ALARM\n");
    //}
    
    if (curr -> left -> data) {
        //curr -> left -> print();
        count += select_eq_do(curr -> left, str, fp, flag, s, c);
    }
    
    if (curr -> right -> data)
        count += select_eq_do(curr -> right, str, fp, flag, s, c);
    
    return count;
}

int external_index:: select_lt (char *str, FILE *fp, bool flag, char *s, command &c)
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
        return select_lt_do (curr, str, fp, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_lt_do (rbtree_node *curr, char *str, FILE *fp, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, fp, flag, s, c);
    
    if (curr -> left -> data)
        count += select_lt_do(curr -> left, str, fp, flag, s, c);
    
    if (curr -> right -> data && strcmp((curr -> right -> data -> name).get(), str) < 0)
        count += select_lt_do(curr -> right, str, fp, flag, s, c);
    
    return count;
}

int external_index:: select_gt (char *str, FILE *fp, bool flag, char *s, command &c)
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
        return select_gt_do (curr, str, fp, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_gt_do (rbtree_node *curr, char *str, FILE *fp, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, fp, flag, s, c);
    
    if (curr -> left -> data && strcmp((curr -> left -> data -> name).get(), str) > 0)
        count += select_gt_do(curr -> left, str, fp, flag, s, c);
    
    if (curr -> right -> data)
        count += select_gt_do(curr -> right, str, fp, flag, s, c);
    
    return count;
}

int external_index:: select_le (char *str, FILE *fp, bool flag, char *s, command &c)
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
        return select_le_do (curr, str, fp, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_le_do (rbtree_node *curr, char *str, FILE *fp, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, fp, flag, s, c);
    
    if (curr -> left -> data)
        count += select_le_do(curr -> left, str, fp, flag, s, c);
    
    if (curr -> right -> data)
        count += select_le_do(curr -> right, str, fp, flag, s, c);
    
    return count;
}

int external_index:: select_ge (char *str, FILE *fp, bool flag, char *s, command &c)
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
        return select_ge_do (curr, str, fp, flag, s, c);
    }
    
    return 0;
}

int external_index:: select_ge_do (rbtree_node *curr, char *str, FILE *fp, bool flag, char *s, command &c)
{
    int count = 0;
    
    count += check_and_select(curr, fp, flag, s, c);
    
    if (curr -> left -> data)
        count += select_ge_do(curr -> left, str, fp, flag, s, c);
    
    if (curr -> right -> data)
        count += select_ge_do(curr -> right, str, fp, flag, s, c);
    
    return count;
}


void external_index:: mark_it_to_delete (rbtree_node *x)
{
    if (removing_elements) {
        tail_removing_elements -> set_r_next(x -> data);
        x -> data -> set_r_prev(tail_removing_elements);
        tail_removing_elements = tail_removing_elements -> r_next;
    }
    else
        tail_removing_elements = removing_elements = x -> data;
}

int external_index:: check_it_to_delete (rbtree_node *x, command &c)
{
    if (!x)
        return -1;
    
    int i, ph = x -> data -> phone;
    
    if (phone) {
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
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str)) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) > 0) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LT: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) < 0) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GE: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) >= 0) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LE: {
                            if (strcmp(x -> data -> get_name(), c.cond[i].str) <= 0) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LIKE: {
                            c.split_cmd_str(c.cond[i].str);
                            if (c.strcmp_like(c.cond[i].str, x -> data -> get_name())) {
                                mark_it_to_delete(x);
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
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (x -> data -> group != c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (x -> data -> group > c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LT: {
                            if (x -> data -> group < c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GE: {
                            if (x -> data -> group >= c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LE: {
                            if (x -> data -> group <= c.cond[i].number) {
                                mark_it_to_delete(x);
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
            mark_it_to_delete(x);
            return 1;
        } // else is not supported in whole purpose!
    }
    else {
        for (i = 0; i < 2; ++i)
            if (c.cond[i].cond_left_t == cond_type::_NAME)
                break;
        
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
        
        if (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::OP_NONE) {
            for (i = 0; i < 2; ++i) {
                if (c.cond[i].cond_left_t == cond_type::_PHONE) {
                    switch (c.cond[i].type_t) {
                        case cond_type::EQ: {
                            if (x -> data -> phone == c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (x -> data -> phone != c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (x -> data -> phone > c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LT: {
                            if (x -> data -> phone < c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GE: {
                            if (x -> data -> phone >= c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LE: {
                            if (x -> data -> phone <= c.cond[i].number) {
                                mark_it_to_delete(x);
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
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::NE: {
                            if (x -> data -> group != c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GT: {
                            if (x -> data -> group > c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LT: {
                            if (x -> data -> group < c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::GE: {
                            if (x -> data -> group >= c.cond[i].number) {
                                mark_it_to_delete(x);
                                return 1;
                            }
                            break;
                        }
                        case cond_type::LE: {
                            if (x -> data -> group <= c.cond[i].number) {
                                mark_it_to_delete(x);
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
            mark_it_to_delete(x);
            return 1;
        } // else is not supported in whole purpose!
    }
    
    return 0;
}

void external_index:: delete_eq (int p, command &c)
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
        delete_eq_do (curr, p, c);
}

void external_index:: delete_eq_do (rbtree_node *curr, int p, command &c)
{
    check_it_to_delete(curr, c);
    
    if (curr -> left -> data)
        delete_eq_do(curr -> left, p, c);
    
    if (curr -> right -> data)
        delete_eq_do(curr -> right, p, c);
}

void external_index:: delete_lt (int p, command &c)
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
        delete_lt_do (curr, p, c);
    }
}

void external_index:: delete_lt_do (rbtree_node *curr, int p, command &c)
{
    check_it_to_delete(curr, c);
    
    if (curr -> left -> data)
        delete_lt_do(curr -> left, p, c);
    
    if (curr -> right -> data && curr -> right -> data -> phone < p)
        delete_lt_do(curr -> right, p, c);
}

void external_index:: delete_gt (int p, command &c)
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
        delete_gt_do (curr, p, c);
    }
}

void external_index:: delete_gt_do (rbtree_node *curr, int p, command &c)
{
    check_it_to_delete(curr, c);
    
    if (curr -> left -> data && curr -> left -> data -> phone > p)
        delete_gt_do(curr -> left, p, c);
    
    if (curr -> right -> data)
        delete_gt_do(curr -> right, p, c);
}

void external_index:: delete_le (int p, command &c)
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
        delete_le_do (curr, p, c);
    }
}

void external_index:: delete_le_do (rbtree_node *curr, int p, command &c)
{
    check_it_to_delete(curr, c);
    
    if (curr -> left -> data)
        delete_le_do(curr -> left, p, c);
    
    if (curr -> right -> data)
        delete_le_do(curr -> right, p, c);
}

void external_index:: delete_ge (int p, command &c)
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
        delete_ge_do (curr, p, c);
    }
}

void external_index:: delete_ge_do (rbtree_node *curr, int p, command &c)
{
    check_it_to_delete(curr, c);
    
    if (curr -> left -> data)
        delete_ge_do(curr -> left, p, c);
    
    if (curr -> right -> data)
        delete_ge_do(curr -> right, p, c);
}

/* void external_index:: delete_into_dlist (list_node *node)
 {
 list_node *prev = node -> prev, *next = node -> next;
 
 if (prev)
 prev -> set_next(next);
 else
 a.set_head(next);
 
 if (next)
 next -> set_prev(prev);
 else {
 a.set_tail(prev);
 
 if (prev)
 prev -> set_next(nullptr);
 }
 
 delete node;
 } */

void external_index:: delete_eq (char *str, command &c)
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
        delete_eq_do (curr, str, c);
}

void external_index:: delete_eq_do (rbtree_node *curr, char *str, command &c)
{
    check_it_to_delete(curr, c);
    
    if (curr -> left -> data)
        delete_eq_do(curr -> left, str, c);
    
    if (curr -> right -> data)
        delete_eq_do(curr -> right, str, c);
}

void external_index:: delete_lt (char *str, command &c)
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
        delete_lt_do (curr, str, c);
    }
}

void external_index:: delete_lt_do (rbtree_node *curr, char *str, command &c)
{
    check_it_to_delete(curr, c);
    
    if (curr -> left -> data)
        delete_lt_do(curr -> left, str, c);
    
    if (curr -> right -> data && strcmp((curr -> right -> data -> name).get(), str) < 0)
        delete_lt_do(curr -> right, str, c);
}

void external_index:: delete_gt (char *str, command &c)
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
        delete_gt_do (curr, str, c);
    }
}

void external_index:: delete_gt_do (rbtree_node *curr, char *str, command &c)
{
    check_it_to_delete(curr, c);
    
    if (curr -> left -> data && strcmp((curr -> left -> data -> name).get(), str) > 0)
        delete_gt_do(curr -> left, str, c);
    
    if (curr -> right -> data)
        delete_gt_do(curr -> right, str, c);
}

void external_index:: delete_le (char *str, command &c)
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
        delete_le_do (curr, str, c);
    }
}

void external_index:: delete_le_do (rbtree_node *curr, char *str, command &c)
{
    check_it_to_delete(curr, c);
    
    if (curr -> left -> data)
        delete_le_do(curr -> left, str, c);
    
    if (curr -> right -> data)
        delete_le_do(curr -> right, str, c);
}

void external_index:: delete_ge (char *str, command &c)
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
        delete_ge_do (curr, str, c);
    }
}

void external_index:: delete_ge_do (rbtree_node *curr, char *str, command &c)
{
    check_it_to_delete(curr, c);
    
    if (curr -> left -> data)
        delete_ge_do(curr -> left, str, c);
    
    if (curr -> right -> data)
        delete_ge_do(curr -> right, str, c);
}

/* void external_index:: delete_into_dlist (list_node *node)
{
    list_node *prev = node -> prev, *next = node -> next;
    
    if (prev)
        prev -> set_next(next);
    else
        a.set_head(next);
    
    if (next)
        next -> set_prev(prev);
    else {
        a.set_tail(prev);
        
        if (prev)
            prev -> set_next(nullptr);
    }
    
    delete node;
} */

/* void external_index:: do_delete ()
{
    list_node *curr = removing_elements, *tmp = nullptr;
    
    while (curr) {
        tmp = curr -> r_next;
        delete_key(curr);
        delete_into_dlist(curr);
        curr = tmp;
    }
    
    tail_removing_elements = removing_elements = nullptr;
} */


// "group" / "group and smth" / "smth and group"
/* int external_index:: select_in_rbtree (command &c, FILE *fp)
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
            return select_eq(number, fp, c.ast, s, c);
        }
        case cond_type::NE: {
            return a.apply_cmd(c, fp);
        }
        case cond_type::LT: {
            return select_lt(number, fp, c.ast, s, c);
        }
        case cond_type::GT: {
            return select_gt(number, fp, c.ast, s, c);
        }
        case cond_type::GE: {
            return select_ge(number, fp, c.ast, s, c);
        }
        case cond_type::LE: {
            return select_le(number, fp, c.ast, s, c);
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

void external_index:: delete_in_rbtree (command &c)
{
    int i, ph_cond = 0, number = 0;
    
    for (i = 0; i < 3; ++i) {
        if (c.cond[i].cond_left_t == cond_type::_PHONE) {
            ph_cond = i;
            number = c.cond[i].number;
            break;
        }
    }
    
    switch (c.cond[ph_cond].type_t) {
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
} */

void external_index:: mark_list (command &c)
{
    list_node *curr = a.head;
    
    if (c.cond[0].cond_left_t == cond_type::_FIELD_NONE) {
        while (curr) {
            if (removing_elements) {
                tail_removing_elements -> r_next = curr;
                curr -> r_prev = tail_removing_elements;
                tail_removing_elements = curr;
            }
            else
                tail_removing_elements = removing_elements = curr;
            
            curr = curr -> next;
        }
        
        return;
    }
    
    while (curr) {
        if (curr -> check_on_cmd(c)) {
            if (removing_elements) {
                tail_removing_elements -> r_next = curr;
                curr -> r_prev = tail_removing_elements;
                tail_removing_elements = curr;
            }
            else
                tail_removing_elements = removing_elements = curr;
        }
        
        curr = curr -> next;
    }
}

int external_index:: apply_cmd (command &c, FILE *fp)
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
                        return select_eq(number, fp, c.ast, s, c);
                    }
                    case cond_type::NE: {
                        return a.apply_cmd(c, fp);
                    }
                    case cond_type::LT: {
                        return select_lt(number, fp, c.ast, s, c);
                    }
                    case cond_type::GT: {
                        return select_gt(number, fp, c.ast, s, c);
                    }
                    case cond_type::GE: {
                        return select_ge(number, fp, c.ast, s, c);
                    }
                    case cond_type::LE: {
                        return select_le(number, fp, c.ast, s, c);
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
                        return select_eq(str, fp, c.ast, s, c);
                    }
                    case cond_type::NE: {
                        return a.apply_cmd(c, fp);
                    }
                    case cond_type::LT: {
                        return select_lt(str, fp, c.ast, s, c);
                    }
                    case cond_type::GT: {
                        return select_gt(str, fp, c.ast, s, c);
                    }
                    case cond_type::GE: {
                        return select_ge(str, fp, c.ast, s, c);
                    }
                    case cond_type::LE: {
                        return select_le(str, fp, c.ast, s, c);
                    }
                    case cond_type::LIKE: {
                        return a.apply_cmd(c, fp);
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

#include "external_index_net.hpp"
