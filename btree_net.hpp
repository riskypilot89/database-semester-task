#include "btree.h"

int btree:: select_eq (btree_node *x, int g, server_string &serv_str, bool flag, char *s, command &c, bool phone)
{
    if (!x)
        return 0;
    
    size_t place = search_group(x, g);
    int count = 0;
    
    if (place < x -> cd && (x -> head[place] -> group) == g) {
        if (phone)
            count += x -> phones[place] -> cmd_select(serv_str, flag, s, c);
        else
            count += x -> names[place] -> cmd_select(serv_str, flag, s, c);
    }
    else if (x -> links[place])
        count += select_eq(x -> links[place], g, serv_str, flag, s, c, phone);
    
    return count;
}

int btree:: select_lt (btree_node *x, int g, server_string &serv_str, bool flag, char *s, command &c, bool phone)
{
    if (!x)
        return 0;
    
    size_t place = search_group(x, g), i;
    int count = 0;
    
    for (i = 0; i < place; ++i) {
        if (phone)
            count += x -> phones[i] -> cmd_select(serv_str, flag, s, c);
        else
            count += x -> names[i] -> cmd_select(serv_str, flag, s, c);
    }
    
    if (x -> links[0]) {
        for (i = 0; i <= place; ++i) {
            count += select_lt(x -> links[i], g, serv_str, flag, s, c, phone);
        }
    }
    
    return count;
}

int btree:: select_gt (btree_node *x, int g, server_string &serv_str, bool flag, char *s, command &c, bool phone)
{
    if (!x)
        return 0;
    
    size_t place = search_group(x, g), i;
    int count = 0;
    
    if (place < x -> cd && (x -> head[place] -> group) == g) {
        for (i = place + 1; i < x -> cd; ++i) {
            if (phone)
                count += x -> phones[i] -> cmd_select(serv_str, flag, s, c);
            else
                count += x -> names[i] -> cmd_select(serv_str, flag, s, c);
        }
        
        if (x -> links[0]) {
            for (i = place + 1; i <= x -> cd; ++i)
                count += select_gt(x -> links[i], g, serv_str, flag, s, c, phone);
        }
    }
    else if (place == x -> cd) {
        if (x -> links[0]) {
            count += select_gt(x -> links[place], g, serv_str, flag, s, c, phone);
        }
    }
    else {
        for (i = place; i < x -> cd; ++i) {
            if (phone)
                count += x -> phones[i] -> cmd_select(serv_str, flag, s, c);
            else
                count += x -> names[i] -> cmd_select(serv_str, flag, s, c);
        }
        
        if (x -> links[0]) {
            for (i = place; i <= x -> cd; ++i) {
                count += select_gt(x -> links[i], g, serv_str, flag, s, c, phone);
            }
        }
    }
    
    return count;
}

int btree:: select_le (btree_node *x, int g, server_string &serv_str, bool flag, char *s, command &c, bool phone)
{
    if (!x)
        return 0;
    
    size_t place = search_group(x, g), i;
    int count = 0;
    
    if (place < x -> cd && (x -> head[place] -> group) == g) {
        for (i = 0; i <= place; ++i) {
            if (phone)
                count += x -> phones[i] -> cmd_select(serv_str, flag, s, c);
            else
                count += x -> names[i] -> cmd_select(serv_str, flag, s, c);
        }
        
        if (x -> links[0]) {
            for (i = 0; i <= place; ++i)
                count += select_le(x -> links[i], g, serv_str, flag, s, c, phone);
        }
    }
    else if (place == x -> cd) {
        if (x -> links[0]) {
            for (i = 0; i <= place; ++i)
                count += select_le(x -> links[i], g, serv_str, flag, s, c, phone);
        }
    }
    else  {
        for (i = 0; i < place; ++i) {
            if (phone)
                count += x -> phones[i] -> cmd_select(serv_str, flag, s, c);
            else
                count += x -> names[i] -> cmd_select(serv_str, flag, s, c);
        }
        
        if (x -> links[0]) {
            for (i = 0; i <= place; ++i) {
                count += select_le(x -> links[i], g, serv_str, flag, s, c, phone);
            }
        }
    }
    
    return count;
}

int btree:: select_ge (btree_node *x, int g, server_string &serv_str, bool flag, char *s, command &c, bool phone)
{
    if (!x)
        return 0;
    
    size_t place = search_group(x, g), i;
    int count = 0;
    
    if (place < x -> cd) {
        for (i = place; i < x -> cd; ++i) {
            if (phone)
                count += x -> phones[i] -> cmd_select(serv_str, flag, s, c);
            else
                count += x -> names[i] -> cmd_select(serv_str, flag, s, c);
        }
        
        if (x -> links[0]) {
            for (i = place; i <= x -> cd; ++i) {
                count += select_ge(x -> links[i], g, serv_str, flag, s, c, phone);
            }
        }
    }
    else {
        if (x -> links[0]) {
            count += select_ge(x -> links[place], g, serv_str, flag, s, c, phone);
        }
    }
    
    return count;
}

int btree:: apply_cmd (command &c, server_string &serv_str)
{
    int i, gr_cond = 0, number = 0;
    bool phone = false;
    
    for (i = 0; i < 2; ++i) {
        if (c.cond[i].cond_left_t == cond_type::_GROUP) {
            gr_cond = i;
            number = c.cond[i].number;
        }
        else if (c.cond[i].cond_left_t == cond_type::_PHONE)
            phone = true;
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
            
            switch (c.cond[gr_cond].type_t) {
                case cond_type::EQ: {
                    return select_eq(root, number, serv_str, c.ast, s, c, phone);
                }
                case cond_type::NE: {
                    // ????????????????????????
                    return a.apply_cmd(c, serv_str);
                }
                case cond_type::LT: {
                    return select_lt(root, number, serv_str, c.ast, s, c, phone);
                }
                case cond_type::GT: {
                    return select_gt(root, number, serv_str, c.ast, s, c, phone);
                }
                case cond_type::GE: {
                    return select_ge(root, number, serv_str, c.ast, s, c, phone);
                }
                case cond_type::LE: {
                    return select_le(root, number, serv_str, c.ast, s, c, phone);
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
        case cmd_type::DELETE: {
            switch (c.cond[gr_cond].type_t) {
                case cond_type::EQ: {
                    delete_eq(root, number, c, phone);
                    break;
                }
                case cond_type::NE: {
                    mark_list(c);
                    break;
                }
                case cond_type::LT: {
                    delete_lt(root, number, c, phone);
                    break;
                }
                case cond_type::GT: {
                    delete_gt(root, number, c, phone);
                    break;
                }
                case cond_type::GE: {
                    delete_ge(root, number, c, phone);
                    break;
                }
                case cond_type::LE: {
                    delete_le(root, number, c, phone);
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

