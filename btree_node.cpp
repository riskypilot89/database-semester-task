#include "btree_node.h"

btree_node:: btree_node (size_t m)
{
    create_node(m);
}

int btree_node:: create_node (size_t m)
{
    if (m) {
        //if (!(group = new int[2*m]))
          //  return 1;
        
        if (!(head = new list_node*[2*m])) {
            //delete [] group;
            return 1;
        }
        
        if (!(tail = new list_node*[2*m])) {
            //delete [] group;
            delete [] head;
            return 2;
        }
        
        if (!(links = new btree_node*[2*m+1])) {
           // delete [] group;
            delete [] head;
            delete [] tail;
            return 3;
        }
        
        if (!(phones = new internal_index*[2*m])) {
            //delete [] group;
            delete [] head;
            delete [] tail;
            delete [] links;
            return 4;
        }
        
        if (!(names = new internal_index*[2*m])) {
           // delete [] group;
            delete [] head;
            delete [] tail;
            delete [] links;
            delete [] phones;
            return 5;
        }
        
        size_t i;
        for (i = 0; i < 2*m; ++i) {
            tail[i] = head[i] = nullptr;
            phones[i] = names[i] = nullptr;
        }
        
        for (i = 0; i < 2*m+1; ++i)
            links[i] = nullptr;
    }
    else {
        tail = head = nullptr;
        phones = names = nullptr;
        links = nullptr;
    }
    
    cd = 0;
    
    return 0;
}

btree_node:: ~btree_node ()
{
    delete [] head;
    delete [] tail;
    delete [] links;
   // delete [] group;

   for (size_t i = 0; i < cd; ++i) {
        delete names[i];
        delete phones[i];
    }

    delete [] names;
    delete [] phones;
    
    tail = head = nullptr;
    links = nullptr;
    names = phones = nullptr;
    //group = nullptr;
    
    cd = 0;
}

void btree_node:: remove ()
{
    delete [] head;
    delete [] tail;
    delete [] links;
    delete [] names;
    delete [] phones;

    tail = head = nullptr;
    links = nullptr;
    names = phones = nullptr;
    //group = nullptr;

    cd = 0;
}

void btree_node:: print_btree_node (FILE *fp)
{
    for (size_t i = 0; i < cd; ++i)
        head[i] -> print(fp);
    
    fprintf(fp, "\n");
}

