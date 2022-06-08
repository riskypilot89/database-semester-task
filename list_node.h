#ifndef LIST_NODE_H
#define LIST_NODE_H

#include "command.h"

class list;
class btree;
class rbtree;
class indexes;
struct pieces;
class internal_index;
class external_index;
class database;

class list_node: public record
{
private:
    list_node *prev = nullptr;
    list_node *next = nullptr;
    list_node *into_prev = nullptr;
    list_node *into_next = nullptr;
    list_node *r_prev = nullptr; // "r" - removing
    list_node *r_next = nullptr;
    friend class list;
    friend class btree;
    friend class rbtree;
    friend class indexes;
    friend struct pieces;
    friend class internal_index;
    friend class external_index;
    friend class database;
    
public:
    list_node ();
    list_node (char *name, int phone, int group);
    list_node (const list_node &i);
    list_node (const list_node &&x);
    ~list_node ();
    
    list_node& operator = (const list_node &rhs);
    
    list_node *get_prev () const;
    list_node* get_next () const;
    void set_prev (list_node *prev);
    void set_next (list_node *next);
    
    list_node *get_r_prev () const;
    list_node* get_r_next () const;
    void set_r_prev (list_node *prev);
    void set_r_next (list_node *next);
    
    int check_on_cmd (command &c);
    
    void print_internal_list (list_node *head);
    
};

#endif
