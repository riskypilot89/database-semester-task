#include "rbtree.h"

rbtree:: rbtree (bool phone)
{
    this -> phone = phone;
}

rbtree:: ~rbtree ()
{
    if (root != NIL) {
       // printf("NOT NIL?\n");
        //root -> data -> print();
       delete_rbtree(root);
    }
    
    root = curr = NIL;
}

void rbtree:: delete_rbtree (rbtree_node *root)
{
    rbtree_node *root_left = root -> left, *root_right = root -> right;
    
    delete root;
    
    if (root_left -> data)
        delete_rbtree(root_left);
    if (root_right -> data)
        delete_rbtree(root_right);
}

void rbtree:: goto_left ()
{
    curr = curr -> left;
}

void rbtree:: goto_right ()
{
    curr = curr -> right;
}

void rbtree:: goto_root ()
{
    curr = root;
}

int rbtree:: add (list_node *a) // phone == true => phone, phone == false => name
{
    rbtree_node *curr = nullptr, *parent = nullptr;
    int res_cmp;
    
    for (curr = root; curr -> data; ) {
        parent = curr;
        
        if (phone)
            res_cmp = a -> cmp_phone(*(curr -> data));
        else
            res_cmp = a -> cmp_name(*(curr -> data));
        
        if (res_cmp < 0)
            curr = curr -> left;
        else if (res_cmp > 0)
            curr = curr -> right;
        else {
            //delete a;
            return 1;
        }
    }
    
    rbtree_node *tmp = nullptr;
    if (!(tmp = new rbtree_node)) {
        return 2;
    }
    
    tmp -> data = a;
    tmp -> left = tmp -> right = NIL;
    tmp -> parent = parent;
    
    if (parent) {
        if (phone) {
            if (a -> cmp_phone(*(parent -> data)) < 0)
                parent -> left = tmp;
            else
                parent -> right = tmp;
        }
        else {
            if (a -> cmp_name(*(parent -> data)) < 0)
                parent -> left = tmp;
            else
                parent -> right = tmp;
        }
    }
    else
        root = tmp;
    
    add_balance(tmp);
    
    return 0;
}

void rbtree:: add_balance (rbtree_node *x)
{
    rbtree_node *grandparent = nullptr, *uncle = nullptr;
    
    while (x != root && x -> parent -> color == RED) {
        grandparent = x -> get_grandparent();
        uncle = x -> get_uncle();
        
        if (uncle -> color == RED) {
            uncle -> color = x -> parent -> color = BLACK;
            grandparent -> color = RED;
            x = grandparent;
        }
        else {
            if (x -> parent == grandparent -> left) {
                if (x == x -> parent -> right) {
                    x = x -> parent;
                    rotate_left(x);
                    grandparent = x -> get_grandparent();
                }
                x -> parent -> color = BLACK;
                grandparent -> color = RED;
                rotate_right(grandparent);
            }
            else {
                if (x == x -> parent -> left) {
                    x = x -> parent;
                    rotate_right(x);
                    grandparent = x -> get_grandparent();
                }
                x -> parent -> color = BLACK;
                grandparent -> color = RED;
                rotate_left(grandparent);
            }
        }
    }
    
    root -> color = BLACK;
}

void rbtree:: rotate_left (rbtree_node *x)
{
    rbtree_node *pivot = x -> right;
    
    x -> right = pivot -> left;
    
    if (pivot -> left -> data)
        pivot -> left -> parent = x;
    
    if (pivot -> data) {
        pivot -> parent = x -> parent;
        pivot -> left = x;
    }
    
    if (x -> parent) {
        if (x == x -> parent -> left)
            x -> parent -> left = pivot;
        else
            x -> parent -> right = pivot;
    }
    else
        root = pivot;
    
    if (x -> data)
        x -> parent = pivot;
}

void rbtree:: rotate_right (rbtree_node *x)
{
    rbtree_node *pivot = x -> left;
    
    x -> left = pivot -> right;
    
    if (pivot -> right -> data)
        pivot -> right -> parent = x;
    
    if (pivot -> data) {
        pivot -> parent = x -> parent;
        pivot -> right = x;
    }
    
    if (x -> parent) {
        if (x == x -> parent -> right)
            x -> parent -> right = pivot;
        else
            x -> parent -> left = pivot;
    }
    else
        root = pivot;
    
    if (x -> data)
        x -> parent = pivot;
}

void rbtree:: delete_key (list_node *a)
{
    int res_cmp;
    
    for (curr = root; curr -> data; ) {
        if (a == curr -> data)
            break;
        
        if (phone)
            res_cmp = a -> cmp_phone(*(curr -> data));
        else
            res_cmp = a -> cmp_name(*(curr -> data));
        
        if (res_cmp < 0) // by phone
            goto_left();
        else
            goto_right();
    }
    
    if (!curr -> data || !curr) // not found
        return;
    
    // ok, delete current element!
    rbtree_node *removing_element = nullptr, *child_of_removing_element = nullptr;
    
    if (!curr -> left -> data || !curr -> right -> data)
        removing_element = curr;
    else {
        removing_element = curr -> right;
        
        while (removing_element -> left -> data)
            removing_element = removing_element -> left;
    }
    
    if (removing_element -> left -> data)
        child_of_removing_element = removing_element -> left;
    else
        child_of_removing_element = removing_element -> right;
    
    // shift:
    child_of_removing_element -> parent = removing_element -> parent;
    
    if (removing_element -> parent) {
        if (removing_element == removing_element -> parent -> left)
            removing_element -> parent -> left = child_of_removing_element;
        else
            removing_element -> parent -> right = child_of_removing_element;
    }
    else
        root = child_of_removing_element;
    
    if (removing_element != curr)
        curr -> data = removing_element -> data;
    
    if (removing_element -> color == BLACK)
        after_deleting_key_balance(child_of_removing_element);
    
    delete removing_element;
}

void rbtree:: after_deleting_key_balance (rbtree_node *child_of_removing_element)
{
    rbtree_node *sibling = nullptr;
    
    while (child_of_removing_element != root && child_of_removing_element -> color == BLACK) {
        // we have another parent after deleting already!
        if (child_of_removing_element == child_of_removing_element -> parent -> left) {
            sibling = child_of_removing_element -> parent -> right;
            
            if (sibling -> color == RED) {
                sibling -> color = BLACK;
                child_of_removing_element -> parent -> color = RED;
                rotate_left(child_of_removing_element -> parent);
                sibling = child_of_removing_element -> parent -> right;
            }
            
            if (sibling -> left -> color == BLACK && sibling -> right -> color == BLACK) {
                sibling -> color = RED;
                child_of_removing_element = child_of_removing_element -> parent;
            }
            else {
                if (sibling -> right -> color == BLACK) {
                    sibling -> left -> color = BLACK;
                    sibling -> color = RED;
                    rotate_right(sibling);
                    sibling = child_of_removing_element -> parent -> right;
                }
                sibling -> color = child_of_removing_element -> parent -> color;
                sibling -> right -> color = child_of_removing_element -> parent -> color = BLACK;
                rotate_left(child_of_removing_element -> parent);
                child_of_removing_element = root;
            }
        }
        else {
            sibling = child_of_removing_element -> parent -> left;
            
            if (sibling -> color == RED) {
                sibling -> color = BLACK;
                child_of_removing_element -> parent -> color = RED;
                rotate_right(child_of_removing_element -> parent);
                sibling = child_of_removing_element -> parent -> left;
            }
            
            if (sibling -> right -> color == BLACK && sibling -> left -> color == BLACK) {
                sibling -> color = RED;
                child_of_removing_element = child_of_removing_element -> parent;
            }
            else {
                if (sibling -> left -> color == BLACK) {
                    sibling -> right -> color = BLACK;
                    sibling -> color = RED;
                    rotate_left(sibling);
                    sibling = child_of_removing_element -> parent -> left;
                }
                sibling -> color = child_of_removing_element -> parent -> color;
                sibling -> left -> color = child_of_removing_element -> parent -> color = BLACK;
                rotate_right(child_of_removing_element -> parent);
                child_of_removing_element = root;
            }
        }
    }
    
    child_of_removing_element -> color = BLACK;
}

int rbtree:: read_rbtree (const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return 1;
    
    list_node *tmp = nullptr;
    
    while (true) {
        if (!(tmp = new list_node)) {
            fclose(fp);
            return 2;
        }
        
        if (tmp -> read(fp)) {
            delete tmp;
            fclose(fp);
            return 0;
        }
        
        add(tmp);
    }
}

void rbtree:: print_rbtree (FILE *fp)
{
    fprintf(fp, "MAX_PRINT_LEVEL_RBTREE: %d\n", MAX_PRINT_LEVEL_RBTREE);
    fprintf(fp, "===============RB-TREE==================\n");
    if (root -> data)
        rec_print(root, 0, fp);
    else
        fprintf(fp, "B: NIL\n");
    fprintf(fp, "========================================\n");
}

void rbtree:: print_subtree (rbtree_node *root)
{
    printf("MAX_PRINT_LEVEL_RBTREE: %d\n", MAX_PRINT_LEVEL_RBTREE);
    printf("===============RB-SUB-TREE==============\n");
    if (root -> data)
        rec_print(root, 0);
    else
        printf("B: NIL\n");
    printf("========================================\n");
}

void rbtree:: rec_print (rbtree_node *root, int level, FILE *fp)
{
    if (level > MAX_PRINT_LEVEL_RBTREE && fp == stdout)
        return;
    
    int i;
    
    for (i = 0; i < level; ++i)
        fprintf(fp, "  ");
    
    root -> print(fp);
    
    if (root -> left -> data)
        rec_print(root -> left, level + 1, fp);
    else {
        for (i = 0; i < level + 1; ++i)
            fprintf(fp, "  ");
        
        fprintf(fp, "B: NIL\n");
    }
    
    if (root -> right -> data)
        rec_print(root -> right, level + 1, fp);
    else {
        for (i = 0; i < level + 1; ++i)
            fprintf(fp, "  ");
        
        fprintf(fp, "B: NIL\n");
    }
}

