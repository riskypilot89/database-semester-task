#ifndef BTREE_NODE_H
#define BTREE_NODE_H

#include "list.h"
#include "internal_index.h"

class btree;

class btree_node
{
private:
    //int *group = nullptr;
    list_node** head = nullptr;
    list_node** tail = nullptr;
    btree_node** links = nullptr; // We always have the same number of elements as in group[] but +1
    size_t cd = 0; // Count of Data – the number of elements in the group[] array
    internal_index **phones = nullptr;
    internal_index **names = nullptr;
    friend class btree;
    
    int create_node (size_t m);
    
public:
    btree_node (size_t m = 0); // we give m to the constructor but it is not the same as 'cd' variable, it's just maximum of cd.
    ~btree_node ();
    void remove ();
    
    void print_btree_node (FILE *fp = stdout);
    
};

#endif
