#ifndef RECORD_H
#define RECORD_H

#include <cstring>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <memory>

#include "server_string.h"

class list_node;
class list;
class btree;
class rbtree_node;
class rbtree;
class indexes;
struct pieces;
class internal_index;
class external_index;
class database;

class record
{
private:
    std::unique_ptr<char[]> name = nullptr;
    int phone;
    int group;
    friend class list_node;
    friend class list;
    friend class btree;
    friend class rbtree_node;
    friend class rbtree;
    friend class indexes;
    friend struct pieces;
    friend class internal_index;
    friend class external_index;
    friend class database;
    
    int init_name (char *name);
    
public:
    record (char *name = nullptr, int phone = 0, int group = 0);
    record (const record &init);
    record (record &&x);
    ~record ();
    
    record& move (record &b);
    void swap (record &b);
    
    char* get_name () const;
    int get_phone () const;
    int get_group () const;
    
    void set_name (char *name = nullptr);
    void set_phone (int phone = 0);
    void set_group (int group = 0);
    
    record & operator = (const record &rhs);
    
    int cmp (const record &b) const;
    bool operator < (const record &b) const;
    bool operator > (const record &b) const;
    bool operator <= (const record &b) const;
    bool operator >= (const record &b) const;
    bool operator == (const record &b) const;
    bool operator != (const record &b) const;
    int cmp_btree (const record &b) const;
    int cmp_phone (const record &b) const;
    int cmp_name (const record &b) const;
    
    int read (FILE *fp);
    void print (FILE *fp = stdout, bool flag = true, char *s = nullptr) const;
    void print (server_string &serv_str, bool flag = true, char *s = nullptr) const;
    
    int init (char *name, int phone, int group);
    
};

#endif
