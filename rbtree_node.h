#ifndef RBTREE_NODE_H
#define RBTREE_NODE_H

#include "list.h"

class rbtree;
class internal_index;
class external_index;
class btree;

enum color_type
{
    RED, BLACK,
};

class rbtree_node
{
private:
    color_type color = RED;
    list_node *data = nullptr;
    rbtree_node *left = nullptr; // NIL
    rbtree_node *right = nullptr; // NIL
    rbtree_node *parent = nullptr;
    friend class rbtree;
    friend class internal_index;
    friend class external_index;
    friend class btree;
    
public:
    rbtree_node ();
    rbtree_node (color_type color, rbtree_node *left, rbtree_node *right); // just for NIL
    ~rbtree_node ();
    
    rbtree_node* get_left ();
    rbtree_node* get_right ();
    rbtree_node* get_parent ();
    rbtree_node* get_uncle ();
    rbtree_node* get_grandparent ();
    
    void set_color (color_type color);
    void set_left (rbtree_node *left);
    void set_right (rbtree_node *right);
    
    void print (FILE *fp = stdout);
    
    rbtree_node& operator = (const rbtree_node &rhs)
    {
        color = rhs.color;
        data = rhs.data;
        left = rhs.left;
        right = rhs.right;
        parent = rhs.parent;
        
        return *this;
    }
    
};

#define NIL &outer
static rbtree_node outer(BLACK, NIL, NIL);

#endif
