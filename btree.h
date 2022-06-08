#ifndef BTREE_H
#define BTREE_H

#include "btree_node.h"
#include "server_string.h"

#define MAX_PRINT_NODE_BTREE 11

class rbtree;
class indexes;
class database;

struct pieces
{
    btree_node *right = nullptr;
    list_node *median_head = nullptr;
    list_node *median_tail = nullptr;
    internal_index *median_phones = nullptr;
    internal_index *median_names = nullptr;
    
    pieces () {}
    
    pieces (list_node *a)
    {
        median_head = median_tail = a;
    }
    
    pieces& operator = (const pieces &rhs)
    {
        median_head = rhs.median_head;
        median_tail = rhs.median_tail;
        median_phones = rhs.median_phones;
        median_names = rhs.median_names;
        
        return *this;
    }
};

struct full_set
{
    list_node *head = nullptr;
    list_node *tail = nullptr;
    internal_index *phones = nullptr;
    internal_index *names = nullptr;
    
    full_set (list_node *head, list_node *tail, internal_index *phones, internal_index *names)
    {
        this -> head = head;
        this -> tail = tail;
        this -> phones = phones;
        this -> names = names;
    }
    
    full_set& operator = (const full_set &rhs)
    {
        head = rhs.head;
        tail = rhs.tail;
        phones = rhs.phones;
        names = rhs.names;
        
        return *this;
    }
};

class btree
{
private:
    btree_node *root = nullptr;
    size_t m = 0;
    list &a;
    list_node *removing_elements = nullptr;
    list_node *tail_removing_elements = nullptr;
    friend class rbtree;
    friend class indexes;
    friend class database;
    
    size_t search_place (btree_node *x, list_node *a);
    
    pieces add (btree_node *x, list_node *a, btree_node **left, btree_node **right);
    pieces recursion_add (btree_node *x, pieces adding, btree_node **left, btree_node **right);
    void split (btree_node *x, pieces adding, size_t place, pieces *outp);
    
    void delete_key (list_node *a);
    void rec_delete_key (btree_node *x, list_node *a);
    void delete_into_array_element (btree_node *x, size_t place, list_node *a, bool *flag);
    size_t search_group (btree_node *x, int group);
    void rec_delete_group (btree_node *x, int group);
    void delete_into_not_leaf (btree_node *x, size_t place);
    full_set find_max_left_side (btree_node *x, size_t place);
    full_set find_min_right_side (btree_node *x, size_t place);
    void delete_into_leaf (btree_node *x, size_t place);
    void balance (btree_node *x, size_t place);
    void merge (btree_node *x, size_t place);
    
    void delete_tree (btree_node *root);
    void rec_print (int maxlevel, btree_node *root, int level);
    
    int select_eq (btree_node *x, int g, FILE *fp, bool flag, char *s, command &c, bool phone);
    int select_ne (btree_node *x, int g, FILE *fp, bool flag, char *s, command &c, bool phone);
    int select_lt (btree_node *x, int g, FILE *fp, bool flag, char *s, command &c, bool phone);
    int select_gt (btree_node *x, int g, FILE *fp, bool flag, char *s, command &c, bool phone);
    int select_le (btree_node *x, int g, FILE *fp, bool flag, char *s, command &c, bool phone);
    int select_ge (btree_node *x, int g, FILE *fp, bool flag, char *s, command &c, bool phone);
    
    int select_eq (btree_node *x, int g, server_string &serv_str, bool flag, char *s, command &c, bool phone);
    int select_ne (btree_node *x, int g, server_string &serv_str, bool flag, char *s, command &c, bool phone);
    int select_lt (btree_node *x, int g, server_string &serv_str, bool flag, char *s, command &c, bool phone);
    int select_gt (btree_node *x, int g, server_string &serv_str, bool flag, char *s, command &c, bool phone);
    int select_le (btree_node *x, int g, server_string &serv_str, bool flag, char *s, command &c, bool phone);
    int select_ge (btree_node *x, int g, server_string &serv_str, bool flag, char *s, command &c, bool phone);
    
    void delete_eq (btree_node *x, int g, command &c, bool phone);
    void delete_ne (btree_node *x, int g, command &c, bool phone);
    void delete_lt (btree_node *x, int g, command &c, bool phone);
    void delete_gt (btree_node *x, int g, command &c, bool phone);
    void delete_le (btree_node *x, int g, command &c, bool phone);
    void delete_ge (btree_node *x, int g, command &c, bool phone);
    void delete_it (btree_node *x, size_t place, list_node *prev, list_node *curr, list_node *next);
    
    void mark_list (command &c);
    void add_removing (internal_index *x);
    
public:
    btree (size_t m, list &a);
    ~btree ();
    
    btree_node* get_root ();
    
    int apply_cmd (command &c, FILE *fp = stdout);
    int apply_cmd (command &c, server_string &serv_str);
    
    int read_btree (const char *filename);
    void print_btree (int maxlevel = 10);
    void print_subtree (btree_node *root, int maxlevel = 10);
};

#endif
