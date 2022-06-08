#ifndef LIST_H
#define LIST_H

#include "list_node.h"

#define MAX_PRINT_LIST 20

class btree;
class rbtree;
class indexes;
class internal_index;
class external_index;
class database;

class list
{
private:
    list_node *head;
    list_node *curr;
    list_node *tail;
    friend class btree;
    friend class rbtree;
    friend class indexes;
    friend class internal_index;
    friend class external_index;
    friend class database;
    
    void delete_list (list_node *head);
    
public:
    list ();
    ~list ();
    
    int apply_cmd (command &c, FILE *fp = stdout);
    int apply_cmd (command &c, server_string &serv_str);
    
    void goto_head ();
    void goto_prev ();
    void goto_next ();
    void goto_tail ();
    list_node* get_curr ();
    list_node* get_head ();
    list_node* get_tail ();
    void set_curr (list_node *a);
    void set_head (list_node *a);
    void set_tail (list_node *a);
    void delete_key (list_node *node);
    
    int read_list (const char *filename = "a.txt");
    void print_list (list_node *head, FILE *fp = stdout);
    void add (list_node *tmp);
    
};

#endif
