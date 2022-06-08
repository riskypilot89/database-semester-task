#include "list.h"

list:: list ()
{
    head = curr = tail = nullptr;
    
}

list:: ~list ()
{
    if (head)
        delete_list(head);

    head = curr = tail = nullptr;
}

void list:: goto_head ()
{
    curr = head;
}

void list:: goto_prev ()
{
    curr = curr -> get_prev();
}

void list:: goto_next ()
{
    curr = curr -> get_next();
}

void list:: goto_tail ()
{
    curr = tail;
}

list_node* list:: get_curr ()
{
    return curr;
}

list_node* list:: get_head ()
{
    return head;
}

list_node* list:: get_tail ()
{
    return tail;
}

void list:: set_curr (list_node *a)
{
    curr = a;
}

void list:: set_head (list_node *a)
{
    head = a;
}

void list:: set_tail (list_node *a)
{
    tail = a;
}


int list:: read_list (const char *filename)
{
    FILE * fp;
    list_node *tmp = nullptr, *tail = nullptr;
    bool add;
    
    if (!(fp = fopen(filename, "r")))
        return 1;
    
    head = new list_node;
    if (!head) {
        head = curr = this -> tail = nullptr;
        fclose(fp);
        return 2;
    }
    
    if (head -> read(fp)) {
        delete head;
        head = curr = this -> tail = nullptr;
        fclose(fp);
        return 3;
    }
    this -> tail = curr = tail = head;
    
    while (true) {
        add = true;
        
        if ((tmp = new list_node) -> read(fp)) {
            delete tmp;
            this -> tail = tail;
            fclose(fp);
            return 0;
        }
        
        for (list_node *check = head; check; check = check -> get_next()) {
            if (*check == *tmp) {
                add = false;
                delete tmp;
                break;
            }
        }
        
        if (add) {
            tail -> set_next(tmp);
            tmp -> set_prev(tail);
            tail = tail -> get_next();
        }
    }
}

void list:: print_list (list_node *head, FILE *fp)
{
    int i = 0;
    list_node *curr = head;
    
    if (fp != stdout)
        while (curr) {
            curr -> print(fp);
            curr = curr -> get_next();
        }
    else {
        while (curr && i < MAX_PRINT_LIST) {
            curr -> print();
            curr = curr -> get_next();
            ++i;
        }
        
        printf("\n");
    }
}

void list:: delete_list (list_node *p_head)
{
    list_node *tmp = nullptr;
    
    while (p_head) {
        tmp = p_head;
        p_head = p_head -> next;
        delete tmp;
    }
}

int list:: apply_cmd (command &c, FILE *fp)
{
    switch (c.type) {
        case cmd_type::SELECT: {
            char s[4]; // container for variable number_str
            int i, count = 0;
            
            if (!c.ast) {
                long int number_str = 0; // for order to print
                
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
                
                snprintf(s, 4 * sizeof(char), "%ld", number_str);
            }
            goto_head();
            
            if (c.cond[0].cond_left_t == cond_type::_FIELD_NONE) {
                while (curr) {
                    curr -> print(fp, c.ast, s);
                    goto_next();
                    ++count;
                }
                return count;
            }
            
          /*  for (i = 0; i < 3; ++i) {
                if (c.cond[i].type_t == cond_type::LIKE) {
                    c.split_cmd_str(c.cond[i].str);
                    break;
                }
            } */
            
            while (curr) {
                if (curr -> check_on_cmd(c)) {
                    curr -> print(fp, c.ast, s);
                    //curr -> print(stdout, c.ast, s);
                    ++count;
                }
                goto_next();
            }
            return count;
        }
        case cmd_type::DELETE: {
            if (c.cond[0].cond_left_t == cond_type::_FIELD_NONE) {
                if (head)
                    delete_list(head);
                
                return 0;
            }
            
            for (int i = 0; i < 3; ++i) {
                if (c.cond[i].type_t == cond_type::LIKE) {
                    c.split_cmd_str(c.cond[i].str);
                    break;
                }
            }
            
            list_node *saved_head = nullptr, *saved_tail = nullptr, *del_head = nullptr, *del_tail = nullptr;
            
            goto_head();
            while (curr) {
                if (curr -> check_on_cmd(c)) {
                    if (del_head) {
                        del_tail -> next = curr;
                        curr -> prev = del_tail;
                        del_tail = curr;
                    }
                    else {
                        del_tail = del_head = curr;
                    }
                }
                else {
                    if (saved_head) {
                        saved_tail -> next = curr;
                        curr -> prev = saved_tail;
                        saved_tail = curr;
                    }
                    else {
                        saved_tail = saved_head = curr;
                    }
                }
                goto_next();
            }
            
            if (saved_head)
                saved_tail -> next = nullptr;
            if (del_head)
                del_tail -> next = nullptr;
            
            head = saved_head;
            delete_list(del_head);
            
            return 0;
        }
        case cmd_type::INSERT: {
            goto_head();
            if (curr) {
                for ( ; curr; goto_next())
                    if (*curr == c)
                        return -1;
                
                list_node *tmp = new list_node;
                if (!tmp)
                    return -2;
                
                tmp -> name = std::move(c.name);
                tmp -> phone = c.phone;
                tmp -> group = c.group;
                
                tail -> next = tmp;
                tmp -> prev = tail;
                tail = tmp;
                
                return 0;
            }
            else {
                list_node *tmp = new list_node;
                if (!tmp)
                    return -2;
                
                tmp -> name = std::move(c.name);
                tmp -> phone = c.phone;
                tmp -> group = c.group;
                
                head = curr = tail = tmp;
                
                return 0;
            }
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

void list:: delete_key (list_node *node)
{
    list_node *prev = node -> prev, *next = node -> next;
    
    if (prev)
        prev -> set_next(next);
    else
        set_head(next);
    
    if (next)
        next -> set_prev(prev);
    else {
        set_tail(prev);
        
        if (prev)
            prev -> set_next(nullptr);
    }
    
    delete node;
}

void list:: add (list_node *tmp)
{
    if (head) {
        tail -> next = tmp;
        tmp -> prev = tail;
        tail = tmp;
    }
    else
        head = tail = tmp;
}

#include "list_net.hpp"
