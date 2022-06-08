#include "rbtree_node.h"

rbtree_node:: rbtree_node () {}

rbtree_node:: rbtree_node (color_type color, rbtree_node *left, rbtree_node *right)
{
    this -> color = color;
    this -> left = left;
    this -> right = right;
}

rbtree_node:: ~rbtree_node () {}

rbtree_node* rbtree_node:: get_grandparent ()
{
    if (this -> parent != nullptr)
        return this -> parent -> parent;
    
    return nullptr;
}

rbtree_node* rbtree_node:: get_uncle ()
{
    rbtree_node *grandparent = get_grandparent();
    
    if (!grandparent)
        return nullptr;
    
    if (this -> parent == grandparent -> left)
        return grandparent -> right;
    
    return grandparent -> left;
}

rbtree_node* rbtree_node:: get_left ()
{
    return left;
}

rbtree_node* rbtree_node:: get_right ()
{
    return right;
}

rbtree_node* rbtree_node:: get_parent ()
{
    return parent;
}

void rbtree_node:: set_color (color_type color)
{
    this -> color = color;
}

void rbtree_node:: set_left (rbtree_node *left)
{
    this -> left = left;
}

void rbtree_node:: set_right (rbtree_node *right)
{
    this -> right = right;
}

void rbtree_node:: print (FILE *fp)
{
    if (this -> color == BLACK)
        fprintf(fp, "B: ");
    else
        fprintf(fp, "R: ");
    
    this -> data -> print(fp);
}
