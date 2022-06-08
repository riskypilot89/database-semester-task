#ifndef RBTREE_H
#define RBTREE_H

#include "rbtree_node.h"

#define MAX_PRINT_LEVEL_RBTREE 10

class btree;
class indexes;
class internal_index;
class external_index;

class rbtree
{
private:
    rbtree_node *root = NIL;
    rbtree_node *curr = NIL;
    //list &a;
    bool phone; //
    friend class btree;
    friend class indexes;
    friend class internal_index;
    friend class external_index;
    
    int apply_cmd (command &c, FILE *fp);
    void add_balance (rbtree_node *x);
    void after_deleting_key_balance (rbtree_node *child_of_removing_element);
    
    void rotate_left (rbtree_node *x);
    void rotate_right (rbtree_node *x);
    
    void delete_rbtree (rbtree_node *root);
    
public:
    rbtree (bool phone);
    ~rbtree ();
    
    void goto_root ();
    void goto_left ();
    void goto_right ();
    
    int add (list_node *a);
    void delete_key (list_node *a);
    
    int read_rbtree (const char *filename);
    void print_rbtree (FILE *fp = stdout);
    void print_subtree (rbtree_node *root);
    void rec_print (rbtree_node *root, int level, FILE *fp = stdout);
    
};

#endif
