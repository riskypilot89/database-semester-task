#include "list.h"

int list:: apply_cmd (command &c, server_string &serv_str)
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
                    curr -> print(serv_str, c.ast, s);
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
                    curr -> print(serv_str, c.ast, s);
                   // printf("FSFSF\n");
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
