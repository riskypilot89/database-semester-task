#ifndef DATABASE_H
#define DATABASE_H

#include "external_index.h"
#include "btree.h"
#include "server_string.h"

class database
{
private:
    list &a;
    external_index &phones; // main index
    external_index &names;
    btree &groups;
    
    list_node *removing_elements = nullptr;
    list_node *tail_removing_elements = nullptr;
    
    void do_delete ();
    
public:
    database (list &a, external_index &phones, external_index &names, btree &groups);
    
    int read (const char *filename);
    int action (command &c, FILE *fd = stdout);
    int action (command &c, server_string &str);
    
};

#endif
