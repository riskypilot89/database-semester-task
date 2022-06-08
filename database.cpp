#include "database.h"

database:: database (list &a, external_index &phones, external_index &names, btree &groups)
: a(a), phones(phones), names(names), groups(groups) {}

int database:: read (const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return 1;
    
    list_node *tmp = new list_node;
    if (!tmp) {
        fclose(fp);
        return 2;
    }
    
    if (tmp -> read(fp)) {
        delete tmp;
        fclose(fp);
        return 3;
    }
    
    if (phones.add(tmp)) {
        delete tmp;
        fclose(fp);
        return 4;
    }
    
    names.add(tmp);
    
    btree_node *left, *right;
    pieces res = groups.add(nullptr, tmp, &left, &right);
    
    if (res.median_head)
        groups.recursion_add(nullptr, res, &left, &right);
    
    a.add(tmp);
    
    while (true) {
        if (!(tmp = new list_node)) {
            fclose(fp);
            return 2;
        }
        
        if (tmp -> read(fp)) {
            delete tmp;
            fclose(fp);
            //a.print_list(a.head);
            return 0;
        }
        
        if (phones.add(tmp))
            delete tmp;
        else {
            names.add(tmp);
            
            res = groups.add(nullptr, tmp, &left, &right);
            
            if (res.median_head)
                groups.recursion_add(nullptr, res, &left, &right);
            
            a.add(tmp);
        }
    }
}

int database:: action (command &c, FILE *fd)
{
    
    // it's for 2 fields. 3 fields not supported yet :(
    if (c.cond[2].cond_left_t != cond_type::_FIELD_NONE)
        return -10;
    
    if (c.type == cmd_type::INSERT) {
        list_node *tmp = new list_node;
        if (!tmp)
            return -11;
        
        tmp -> name = std::move(c.name);
        tmp -> phone = c.phone;
        tmp -> group = c.group;
        
        if (phones.add(tmp)) {
            delete tmp;
            //printf("ALARM!\n");
            return -1;
        }
        else {
            names.add(tmp);
            
            btree_node *left, *right;
            pieces res = groups.add(nullptr, tmp, &left, &right);
            
            if (res.median_head)
                groups.recursion_add(nullptr, res, &left, &right);
            
            a.add(tmp);
        }
        
        return 0;
    }
    
    int i;
    bool group_indicator = false, phone_indicator = false, name_indicator = false;
    
    for (i = 0; i < 2; ++i) {
        if (c.cond[i].cond_left_t == cond_type::_GROUP)
            group_indicator = true;
        else if (c.cond[i].cond_left_t == cond_type::_PHONE)
            phone_indicator = true;
        else if (c.cond[i].cond_left_t == cond_type::_NAME)
            name_indicator = true;
    }
    
    if (name_indicator && !group_indicator && !phone_indicator) {
        // external name
        if (c.type == cmd_type::SELECT)
            return names.apply_cmd(c, fd);
        if (c.type == cmd_type::DELETE) {
            names.apply_cmd(c, fd);
            removing_elements = names.removing_elements;
            tail_removing_elements = names.tail_removing_elements;
            do_delete(); ////////??????????
            return 0;
        }
    }
    else if (phone_indicator && !name_indicator && !group_indicator) {
        // external phone
        if (c.type == cmd_type::SELECT)
            return phones.apply_cmd(c, fd);
        if (c.type == cmd_type::DELETE) {
            phones.apply_cmd(c, fd);
            removing_elements = phones.removing_elements;
            tail_removing_elements = phones.tail_removing_elements;
            do_delete(); ////////??????????
            return 0;
        }
    }
    else if (group_indicator && !name_indicator && !phone_indicator) {
        // btree group
        if (c.type == cmd_type::SELECT)
            return groups.apply_cmd(c, fd);
        if (c.type == cmd_type::DELETE) {
            groups.apply_cmd(c, fd);
            removing_elements = groups.removing_elements;
            tail_removing_elements = groups.tail_removing_elements;
            do_delete(); ////////??????????
            return 0;
        }
    }
    else if (name_indicator && phone_indicator) {
        if (c.oper[0] == op_type_t::OP_NONE || (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::OP_NONE)) {
            // external name
            if (c.type == cmd_type::SELECT)
                return names.apply_cmd(c, fd);
            if (c.type == cmd_type::DELETE) {
                names.apply_cmd(c, fd);
                removing_elements = names.removing_elements;
                tail_removing_elements = names.tail_removing_elements;
                do_delete(); ////////??????????
                return 0;
            }
        }
        if (c.oper[0] == op_type_t::OR && c.oper[1] == op_type_t::OP_NONE) {
            // external name, then NOT name AND phone in external phone
            if (c.cond[0].cond_left_t == cond_type::_NAME && c.cond[1].cond_left_t == cond_type::_PHONE && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                c.oper[0] = op_type_t::OP_NONE;
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += names.apply_cmd(c, fd);
                }
                
                if (c.type == cmd_type::DELETE) {
                    names.apply_cmd(c, fd);
                    removing_elements = names.removing_elements;
                    tail_removing_elements = names.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += phones.apply_cmd(c, fd);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    phones.apply_cmd(c, fd);
                    tail_removing_elements -> r_next = phones.removing_elements;
                    tail_removing_elements = phones.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
            if (c.cond[0].cond_left_t == cond_type::_PHONE && c.cond[1].cond_left_t == cond_type::_NAME && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                // reverse it to "name or phone":
                c.cond[0].cond_left_t = cond_type::_NAME;
                auto tmp = c.cond[0].type_t;
                c.cond[0].type_t = c.cond[1].type_t;
                c.cond[1].type_t = tmp;
                c.cond[1].cond_left_t = cond_type::_PHONE;
                auto tmp2 = c.cond[0].number;
                c.cond[0].number = 0;
                c.cond[0].str = c.cond[1].str;
                c.cond[1].str = nullptr;
                c.cond[1].number = tmp2;
                
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += names.apply_cmd(c, fd);
                }
                
                if (c.type == cmd_type::DELETE) {
                    names.apply_cmd(c, fd);
                    removing_elements = names.removing_elements;
                    tail_removing_elements = names.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += phones.apply_cmd(c, fd);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    phones.apply_cmd(c, fd);
                    tail_removing_elements -> r_next = phones.removing_elements;
                    tail_removing_elements = phones.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
        }
    }
    else if (name_indicator && group_indicator) {
        if (c.oper[0] == op_type_t::OP_NONE || (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::OP_NONE)) {
            // btree group, then internal name
            if (c.type == cmd_type::SELECT)
                return groups.apply_cmd(c, fd);
            if (c.type == cmd_type::DELETE) {
                groups.apply_cmd(c, fd);
                removing_elements = groups.removing_elements;
                tail_removing_elements = groups.tail_removing_elements;
                do_delete(); ////////??????????
                return 0;
            }
        }
        if (c.oper[0] == op_type_t::OR && c.oper[1] == op_type_t::OP_NONE) {
            // btree group, then NOT group AND name in external name
            if (c.cond[0].cond_left_t == cond_type::_GROUP && c.cond[1].cond_left_t == cond_type::_NAME && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                c.oper[0] = op_type_t::OP_NONE;
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += groups.apply_cmd(c, fd);
                }
                
                if (c.type == cmd_type::DELETE) {
                    groups.apply_cmd(c, fd);
                    removing_elements = groups.removing_elements;
                    tail_removing_elements = groups.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += names.apply_cmd(c, fd);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    names.apply_cmd(c, fd);
                    tail_removing_elements -> r_next = names.removing_elements;
                    tail_removing_elements = names.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
            if (c.cond[0].cond_left_t == cond_type::_NAME && c.cond[1].cond_left_t == cond_type::_GROUP && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                // reverse it to "group or name":
                c.cond[0].cond_left_t = cond_type::_GROUP;
                auto tmp = c.cond[0].type_t;
                c.cond[0].type_t = c.cond[1].type_t;
                c.cond[1].type_t = tmp;
                c.cond[1].cond_left_t = cond_type::_NAME;
                auto tmp2 = c.cond[0].str;
                c.cond[0].str = nullptr;
                c.cond[1].str = tmp2;
                c.cond[0].number = c.cond[1].number;
                c.cond[1].number = 0;
                
                c.oper[0] = op_type_t::OP_NONE;
                
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += groups.apply_cmd(c, fd);
                }
                
                if (c.type == cmd_type::DELETE) {
                    groups.apply_cmd(c, fd);
                    removing_elements = groups.removing_elements;
                    tail_removing_elements = groups.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += names.apply_cmd(c, fd);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    names.apply_cmd(c, fd);
                    tail_removing_elements -> r_next = names.removing_elements;
                    tail_removing_elements = names.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
        }
    }
    else if (phone_indicator && group_indicator) {
        if (c.oper[0] == op_type_t::OP_NONE || (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::OP_NONE)) {
            // btree then internal phone
            if (c.type == cmd_type::SELECT)
                return groups.apply_cmd(c, fd);
            if (c.type == cmd_type::DELETE) {
                groups.apply_cmd(c, fd);
                removing_elements = groups.removing_elements;
                tail_removing_elements = groups.tail_removing_elements;
                do_delete(); ////////??????????
                return 0;
            }
        }
        if (c.oper[0] == op_type_t::OR && c.oper[1] == op_type_t::OP_NONE) {
            // btree group, then NOT group AND phone in external phone
            if (c.cond[0].cond_left_t == cond_type::_GROUP && c.cond[1].cond_left_t == cond_type::_PHONE && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                c.oper[0] = op_type_t::OP_NONE;
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += groups.apply_cmd(c, fd);
                }
                
                if (c.type == cmd_type::DELETE) {
                    groups.apply_cmd(c, fd);
                    removing_elements = groups.removing_elements;
                    tail_removing_elements = groups.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += phones.apply_cmd(c, fd);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    phones.apply_cmd(c, fd);
                    tail_removing_elements -> r_next = phones.removing_elements;
                    tail_removing_elements = phones.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
            if (c.cond[0].cond_left_t == cond_type::_PHONE && c.cond[1].cond_left_t == cond_type::_GROUP && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                // reverse it to "group or phone":
                c.cond[0].cond_left_t = cond_type::_GROUP;
                auto tmp = c.cond[0].type_t;
                c.cond[0].type_t = c.cond[1].type_t;
                c.cond[1].type_t = tmp;
                c.cond[1].cond_left_t = cond_type::_PHONE;
                auto tmp2 = c.cond[0].number;
                c.cond[0].number = c.cond[1].number;
                c.cond[1].number = tmp2;
                
               // c.print_cmd();
                
                c.oper[0] = op_type_t::OP_NONE;
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += groups.apply_cmd(c, fd);
                }
                
                if (c.type == cmd_type::DELETE) {
                    groups.apply_cmd(c, fd);
                    removing_elements = groups.removing_elements;
                    tail_removing_elements = groups.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += phones.apply_cmd(c, fd);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    phones.apply_cmd(c, fd);
                    tail_removing_elements -> r_next = phones.removing_elements;
                    tail_removing_elements = phones.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
        }
    }
    
    return 0;
}

int database:: action (command &c, server_string &str)
{
    
    // it's for 2 fields. 3 fields not supported yet :(
    if (c.cond[2].cond_left_t != cond_type::_FIELD_NONE)
        return -10;
    
    if (c.type == cmd_type::INSERT) {
        list_node *tmp = new list_node;
        if (!tmp)
            return -11;
        
        tmp -> name = std::move(c.name);
        tmp -> phone = c.phone;
        tmp -> group = c.group;
        
        if (phones.add(tmp)) {
            delete tmp;
            //printf("ALARM!\n");
            return -1;
        }
        else {
            names.add(tmp);
            
            btree_node *left, *right;
            pieces res = groups.add(nullptr, tmp, &left, &right);
            
            if (res.median_head)
                groups.recursion_add(nullptr, res, &left, &right);
            
            a.add(tmp);
        }
        
        return 0;
    }
    
    int i;
    bool group_indicator = false, phone_indicator = false, name_indicator = false;
    
    for (i = 0; i < 2; ++i) {
        if (c.cond[i].cond_left_t == cond_type::_GROUP)
            group_indicator = true;
        else if (c.cond[i].cond_left_t == cond_type::_PHONE)
            phone_indicator = true;
        else if (c.cond[i].cond_left_t == cond_type::_NAME)
            name_indicator = true;
    }
    
    if (name_indicator && !group_indicator && !phone_indicator) {
        // external name
        if (c.type == cmd_type::SELECT) {
           // printf("NAME!!!\n");
            return names.apply_cmd(c, str);
        }
        if (c.type == cmd_type::DELETE) {
            names.apply_cmd(c, str);
            removing_elements = names.removing_elements;
            tail_removing_elements = names.tail_removing_elements;
            do_delete(); ////////??????????
            return 0;
        }
    }
    else if (phone_indicator && !name_indicator && !group_indicator) {
        // external phone
        if (c.type == cmd_type::SELECT)
            return phones.apply_cmd(c, str);
        if (c.type == cmd_type::DELETE) {
            phones.apply_cmd(c, str);
            removing_elements = phones.removing_elements;
            tail_removing_elements = phones.tail_removing_elements;
            do_delete(); ////////??????????
            return 0;
        }
    }
    else if (group_indicator && !name_indicator && !phone_indicator) {
        // btree group
        if (c.type == cmd_type::SELECT)
            return groups.apply_cmd(c, str);
        if (c.type == cmd_type::DELETE) {
            groups.apply_cmd(c, str);
            removing_elements = groups.removing_elements;
            tail_removing_elements = groups.tail_removing_elements;
            do_delete(); ////////??????????
            return 0;
        }
    }
    else if (name_indicator && phone_indicator) {
        if (c.oper[0] == op_type_t::OP_NONE || (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::OP_NONE)) {
            // external name
            if (c.type == cmd_type::SELECT)
                return names.apply_cmd(c, str);
            if (c.type == cmd_type::DELETE) {
                names.apply_cmd(c, str);
                removing_elements = names.removing_elements;
                tail_removing_elements = names.tail_removing_elements;
                do_delete(); ////////??????????
                return 0;
            }
        }
        if (c.oper[0] == op_type_t::OR && c.oper[1] == op_type_t::OP_NONE) {
            // external name, then NOT name AND phone in external phone
            if (c.cond[0].cond_left_t == cond_type::_NAME && c.cond[1].cond_left_t == cond_type::_PHONE && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                c.oper[0] = op_type_t::OP_NONE;
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += names.apply_cmd(c, str);
                }
                
                if (c.type == cmd_type::DELETE) {
                    names.apply_cmd(c, str);
                    removing_elements = names.removing_elements;
                    tail_removing_elements = names.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += phones.apply_cmd(c, str);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    phones.apply_cmd(c, str);
                    tail_removing_elements -> r_next = phones.removing_elements;
                    tail_removing_elements = phones.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
            if (c.cond[0].cond_left_t == cond_type::_PHONE && c.cond[1].cond_left_t == cond_type::_NAME && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                // reverse it to "name or phone":
                c.cond[0].cond_left_t = cond_type::_NAME;
                auto tmp = c.cond[0].type_t;
                c.cond[0].type_t = c.cond[1].type_t;
                c.cond[1].type_t = tmp;
                c.cond[1].cond_left_t = cond_type::_PHONE;
                auto tmp2 = c.cond[0].number;
                c.cond[0].number = 0;
                c.cond[0].str = c.cond[1].str;
                c.cond[1].str = nullptr;
                c.cond[1].number = tmp2;
                
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += names.apply_cmd(c, str);
                }
                
                if (c.type == cmd_type::DELETE) {
                    names.apply_cmd(c, str);
                    removing_elements = names.removing_elements;
                    tail_removing_elements = names.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += phones.apply_cmd(c, str);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    phones.apply_cmd(c, str);
                    tail_removing_elements -> r_next = phones.removing_elements;
                    tail_removing_elements = phones.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
        }
    }
    else if (name_indicator && group_indicator) {
        if (c.oper[0] == op_type_t::OP_NONE || (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::OP_NONE)) {
            // btree group, then internal name
            if (c.type == cmd_type::SELECT)
                return groups.apply_cmd(c, str);
            if (c.type == cmd_type::DELETE) {
                groups.apply_cmd(c, str);
                removing_elements = groups.removing_elements;
                tail_removing_elements = groups.tail_removing_elements;
                do_delete(); ////////??????????
                return 0;
            }
        }
        if (c.oper[0] == op_type_t::OR && c.oper[1] == op_type_t::OP_NONE) {
            // btree group, then NOT group AND name in external name
            if (c.cond[0].cond_left_t == cond_type::_GROUP && c.cond[1].cond_left_t == cond_type::_NAME && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                c.oper[0] = op_type_t::OP_NONE;
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += groups.apply_cmd(c, str);
                }
                
                if (c.type == cmd_type::DELETE) {
                    groups.apply_cmd(c, str);
                    removing_elements = groups.removing_elements;
                    tail_removing_elements = groups.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += names.apply_cmd(c, str);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    names.apply_cmd(c, str);
                    tail_removing_elements -> r_next = names.removing_elements;
                    tail_removing_elements = names.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
            if (c.cond[0].cond_left_t == cond_type::_NAME && c.cond[1].cond_left_t == cond_type::_GROUP && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                // reverse it to "group or name":
                c.cond[0].cond_left_t = cond_type::_GROUP;
                auto tmp = c.cond[0].type_t;
                c.cond[0].type_t = c.cond[1].type_t;
                c.cond[1].type_t = tmp;
                c.cond[1].cond_left_t = cond_type::_NAME;
                auto tmp2 = c.cond[0].str;
                c.cond[0].str = nullptr;
                c.cond[1].str = tmp2;
                c.cond[0].number = c.cond[1].number;
                c.cond[1].number = 0;
                
                c.oper[0] = op_type_t::OP_NONE;
                
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += groups.apply_cmd(c, str);
                }
                
                if (c.type == cmd_type::DELETE) {
                    groups.apply_cmd(c, str);
                    removing_elements = groups.removing_elements;
                    tail_removing_elements = groups.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += names.apply_cmd(c, str);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    names.apply_cmd(c, str);
                    tail_removing_elements -> r_next = names.removing_elements;
                    tail_removing_elements = names.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
        }
    }
    else if (phone_indicator && group_indicator) {
        if (c.oper[0] == op_type_t::OP_NONE || (c.oper[0] == op_type_t::AND && c.oper[1] == op_type_t::OP_NONE)) {
            // btree then internal phone
            if (c.type == cmd_type::SELECT)
                return groups.apply_cmd(c, str);
            if (c.type == cmd_type::DELETE) {
                groups.apply_cmd(c, str);
                removing_elements = groups.removing_elements;
                tail_removing_elements = groups.tail_removing_elements;
                do_delete(); ////////??????????
                return 0;
            }
        }
        if (c.oper[0] == op_type_t::OR && c.oper[1] == op_type_t::OP_NONE) {
            // btree group, then NOT group AND phone in external phone
            if (c.cond[0].cond_left_t == cond_type::_GROUP && c.cond[1].cond_left_t == cond_type::_PHONE && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                c.oper[0] = op_type_t::OP_NONE;
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += groups.apply_cmd(c, str);
                }
                
                if (c.type == cmd_type::DELETE) {
                    groups.apply_cmd(c, str);
                    removing_elements = groups.removing_elements;
                    tail_removing_elements = groups.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += phones.apply_cmd(c, str);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    phones.apply_cmd(c, str);
                    tail_removing_elements -> r_next = phones.removing_elements;
                    tail_removing_elements = phones.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
            if (c.cond[0].cond_left_t == cond_type::_PHONE && c.cond[1].cond_left_t == cond_type::_GROUP && c.cond[2].cond_left_t == cond_type::_FIELD_NONE) {
                // reverse it to "group or phone":
                c.cond[0].cond_left_t = cond_type::_GROUP;
                auto tmp = c.cond[0].type_t;
                c.cond[0].type_t = c.cond[1].type_t;
                c.cond[1].type_t = tmp;
                c.cond[1].cond_left_t = cond_type::_PHONE;
                auto tmp2 = c.cond[0].number;
                c.cond[0].number = c.cond[1].number;
                c.cond[1].number = tmp2;
                
                // c.print_cmd();
                
                c.oper[0] = op_type_t::OP_NONE;
                int res = 0;
                if (c.type == cmd_type::SELECT) {
                    res += groups.apply_cmd(c, str);
                }
                
                if (c.type == cmd_type::DELETE) {
                    groups.apply_cmd(c, str);
                    removing_elements = groups.removing_elements;
                    tail_removing_elements = groups.tail_removing_elements;
                    //do_delete();
                }
                
                switch (c.cond[0].type_t) {
                    case cond_type::EQ: {
                        c.cond[0].type_t = cond_type::NE;
                        break;
                    }
                    case cond_type::NE: {
                        c.cond[0].type_t = cond_type::EQ;
                        break;
                    }
                    case cond_type::LT: {
                        c.cond[0].type_t = cond_type::GE;
                        break;
                    }
                    case cond_type::GT: {
                        c.cond[0].type_t = cond_type::LE;
                        break;
                    }
                    case cond_type::LE: {
                        c.cond[0].type_t = cond_type::GT;
                        break;
                    }
                    case cond_type::GE: {
                        c.cond[0].type_t = cond_type::LT;
                        break;
                    }
                    case cond_type::LIKE: {
                        return 0;
                    }
                    case cond_type::COND_NONE: {
                        return 0;
                    }
                }
                c.oper[0] = op_type_t::AND;
                if (c.type == cmd_type::SELECT) {
                    res += phones.apply_cmd(c, str);
                    return res;
                }
                
                if (c.type == cmd_type::DELETE) {
                    phones.apply_cmd(c, str);
                    tail_removing_elements -> r_next = phones.removing_elements;
                    tail_removing_elements = phones.tail_removing_elements;
                    do_delete(); ////////// ?????
                }
                
                return 0;
            }
        }
    }
    
    return 0;
}

void database:: do_delete ()
{
    list_node *curr = removing_elements, *tmp = nullptr;
    
    while (curr) {
        tmp = curr -> r_next;
        groups.delete_key(curr);
        phones.delete_key(curr);
        names.delete_key(curr);
        a.delete_key(curr);
        curr = tmp;
    }
    
    tail_removing_elements = removing_elements = groups.removing_elements = groups.tail_removing_elements = phones.removing_elements = phones.tail_removing_elements = names.removing_elements = names.tail_removing_elements = nullptr;
}
