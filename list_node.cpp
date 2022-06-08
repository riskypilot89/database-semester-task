#include "list_node.h"

list_node:: list_node ()
: record() {}

list_node:: list_node (char *name, int phone, int group)
: record(name, phone, group) {}

list_node:: list_node (const list_node &i)
: record(i) {}

list_node:: list_node (const list_node &&x)
: record(x) {}

list_node:: ~list_node () {}

list_node& list_node:: operator = (const list_node &rhs)
{
    *((record *) (this)) = rhs;
    r_next = r_prev = next = prev = nullptr;
    return *this;
}

list_node* list_node:: get_prev () const
{
    return prev;
}

list_node* list_node:: get_next () const
{
    return next;
}

void list_node:: set_prev (list_node *prev)
{
    this -> prev = prev;
}

void list_node:: set_next (list_node *next)
{
    this -> next = next;
}

list_node* list_node:: get_r_prev () const
{
    return r_prev;
}

list_node* list_node:: get_r_next () const
{
    return r_next;
}

void list_node:: set_r_prev (list_node *prev)
{
    this -> r_prev = prev;
}

void list_node:: set_r_next (list_node *next)
{
    this -> r_next = next;
}

int list_node:: check_on_cmd (command &c)
{
    int check[3] = {};
    
    for (int i = 0; i < 3; ++i) {
        if (c.cond[i].cond_left_t == cond_type::_NAME) {
            switch (c.cond[i].type_t) {
                case cond_type::EQ: {
                    if (!strcmp(name.get(), c.cond[i].str))
                        check[i] = 1;
                    break;
                }
                case cond_type::NE: {
                    if (strcmp(name.get(), c.cond[i].str))
                        check[i] = 1;
                    break;
                }
                case cond_type::LT: {
                    if (strcmp(name.get(), c.cond[i].str) < 0)
                        check[i] = 1;
                }
                case cond_type::GT: {
                    if (strcmp(name.get(), c.cond[i].str) > 0)
                        check[i] = 1;
                    break;
                }
                case cond_type::LE: {
                    if (strcmp(name.get(), c.cond[i].str) <= 0)
                        check[i] = 1;
                    break;
                }
                case cond_type::GE: {
                    if (strcmp(name.get(), c.cond[i].str) >= 0)
                        check[i] = 1;
                    break;
                }
                case cond_type::LIKE: {
                    //printf("LIKE!\n");
                    c.split_cmd_str(c.cond[i].str);
                    if (c.strcmp_like(c.cond[i].str, name.get())) {
                        //printf("IS LIKE\n");
                        check[i] = 1;
                    }
                    break;
                }
                case cond_type::COND_NONE: {
                    break;
                }
            }
        }
        else if (c.cond[i].cond_left_t == cond_type::_PHONE) {
            switch (c.cond[i].type_t) {
                case cond_type::EQ: {
                    if (phone == c.cond[i].number)
                        check[i] = 1;
                    break;
                }
                case cond_type::NE: {
                    if (phone != c.cond[i].number)
                        check[i] = 1;
                    break;
                }
                case cond_type::LT: {
                    if (phone < c.cond[i].number)
                        check[i] = 1;
                }
                case cond_type::GT: {
                    if (phone > c.cond[i].number)
                        check[i] = 1;
                    break;
                }
                case cond_type::LE: {
                    if (phone <= c.cond[i].number)
                        check[i] = 1;
                    break;
                }
                case cond_type::GE: {
                    if (phone >= c.cond[i].number)
                        check[i] = 1;
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
                    if (group == c.cond[i].number)
                        check[i] = 1;
                    break;
                }
                case cond_type::NE: {
                    if (group != c.cond[i].number)
                        check[i] = 1;
                    break;
                }
                case cond_type::LT: {
                    if (group < c.cond[i].number)
                        check[i] = 1;
                }
                case cond_type::GT: {
                    if (group > c.cond[i].number)
                        check[i] = 1;
                    break;
                }
                case cond_type::LE: {
                    if (group <= c.cond[i].number)
                        check[i] = 1;
                    break;
                }
                case cond_type::GE: {
                    if (group >= c.cond[i].number)
                        check[i] = 1;
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
        else
            break;
    }
    
    if (c.oper[0] == op_type_t::OP_NONE)
        return check[0];
    
    if (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::OP_NONE)
        return check[0] * check[1];
    
    if (c.oper[0] == op_type_t::OR && c.oper[1] == op_type_t::OP_NONE)
        return check[0] + check[1];
    
    if (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::AND)
        return check[0] * check[1] * check[2];
    
    if (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::OR)
        return check[0] * check[1] + check[2];
    
    if (c.oper[0] == op_type_t::OR && c.oper[1] == op_type_t::AND)
        return check[0] + check[1] * check[2];
    
    if (c.oper[0] == op_type_t::OR && c.oper[1] == op_type_t::OR)
        return check[0] + check[1] + check[2];
    
    return 0;
}


void list_node:: print_internal_list (list_node *head)
{
    // int count = 0;
    
    for (list_node *curr = head; curr; curr = curr -> into_next) {
        curr -> print();
       // ++count;
    }
    
   // printf("COUNT: %d\n", count);
}
