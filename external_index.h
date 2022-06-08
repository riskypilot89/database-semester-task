#ifndef EXTERNAL_INDEX_H
#define EXTERNAL_INDEX_H

#include "rbtree.h"
#include "server_string.h"

class database;

class external_index: public rbtree
{
private:
    list &a;
    list_node *removing_elements = nullptr;
    list_node *tail_removing_elements = nullptr;
    friend class database;
    
    int check_and_select (rbtree_node *x, FILE *fp, bool flag, char *s, command &c);
    int select_eq (int p, FILE *fp, bool flag, char *s, command &c); // phones
    int select_eq_do (rbtree_node *curr, int p, FILE *fp, bool flag, char *s, command &c);
    int select_ne (int p, FILE *fp, bool flag, char *s, command &c);
    int select_ne_do (rbtree_node *curr, int p, FILE *fp, bool flag, char *s, command &c);
    int select_lt (int p, FILE *fp, bool flag, char *s, command &c);
    int select_lt_do (rbtree_node *curr, int p, FILE *fp, bool flag, char *s, command &c);
    int select_gt (int p, FILE *fp, bool flag, char *s, command &c);
    int select_gt_do (rbtree_node *curr, int p, FILE *fp, bool flag, char *s, command &c);
    int select_le (int p, FILE *fp, bool flag, char *s, command &c);
    int select_le_do (rbtree_node *curr, int p, FILE *fp, bool flag, char *s, command &c);
    int select_ge (int p, FILE *fp, bool flag, char *s, command &c);
    int select_ge_do (rbtree_node *curr, int p, FILE *fp, bool flag, char *s, command &c);
    int select_eq (char *str, FILE *fp, bool flag, char *s, command &c); // names
    int select_eq_do (rbtree_node *curr, char *str, FILE *fp, bool flag, char *s, command &c);
    int select_ne (char *str, FILE *fp, bool flag, char *s, command &c);
    int select_ne_do (rbtree_node *curr, char *str, FILE *fp, bool flag, char *s, command &c);
    int select_lt (char *str, FILE *fp, bool flag, char *s, command &c);
    int select_lt_do (rbtree_node *curr, char *str, FILE *fp, bool flag, char *s, command &c);
    int select_gt (char *str, FILE *fp, bool flag, char *s, command &c);
    int select_gt_do (rbtree_node *curr, char *str, FILE *fp, bool flag, char *s, command &c);
    int select_le (char *str, FILE *fp, bool flag, char *s, command &c);
    int select_le_do (rbtree_node *curr, char *str, FILE *fp, bool flag, char *s, command &c);
    int select_ge (char *str, FILE *fp, bool flag, char *s, command &c);
    int select_ge_do (rbtree_node *curr, char *str, FILE *fp, bool flag, char *s, command &c);
    
    int check_and_select (rbtree_node *x, server_string &serv_str, bool flag, char *s, command &c);
    int select_eq (int p, server_string &serv_str, bool flag, char *s, command &c); // phones
    int select_eq_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c);
    int select_ne (int p, server_string &serv_str, bool flag, char *s, command &c);
    int select_ne_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c);
    int select_lt (int p, server_string &serv_str, bool flag, char *s, command &c);
    int select_lt_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c);
    int select_gt (int p, server_string &serv_str, bool flag, char *s, command &c);
    int select_gt_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c);
    int select_le (int p, server_string &serv_str, bool flag, char *s, command &c);
    int select_le_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c);
    int select_ge (int p, server_string &serv_str, bool flag, char *s, command &c);
    int select_ge_do (rbtree_node *curr, int p, server_string &serv_str, bool flag, char *s, command &c);
    int select_eq (char *str, server_string &serv_str, bool flag, char *s, command &c); // names
    int select_eq_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c);
    int select_ne (char *str, server_string &serv_str, bool flag, char *s, command &c);
    int select_ne_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c);
    int select_lt (char *str, server_string &serv_str, bool flag, char *s, command &c);
    int select_lt_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c);
    int select_gt (char *str, server_string &serv_str, bool flag, char *s, command &c);
    int select_gt_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c);
    int select_le (char *str, server_string &serv_str, bool flag, char *s, command &c);
    int select_le_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c);
    int select_ge (char *str, server_string &serv_str, bool flag, char *s, command &c);
    int select_ge_do (rbtree_node *curr, char *str, server_string &serv_str, bool flag, char *s, command &c);
    
    int check_it_to_delete (rbtree_node *x, command &c);
    void mark_it_to_delete (rbtree_node *x);
    void delete_eq (int p, command &c); // phones
    void delete_eq_do (rbtree_node *curr, int p, command &c);
    void delete_ne (int p, command &c);
    void delete_ne_do (rbtree_node *curr, int p, command &c);
    void delete_lt (int p, command &c);
    void delete_lt_do (rbtree_node *curr, int p, command &c);
    void delete_gt (int p, command &c);
    void delete_gt_do (rbtree_node *curr, int p, command &c);
    void delete_le (int p, command &c);
    void delete_le_do (rbtree_node *curr, int p, command &c);
    void delete_ge (int p, command &c);
    void delete_ge_do (rbtree_node *curr, int p, command &c);
    void delete_eq (char *str, command &c); // names
    void delete_eq_do (rbtree_node *curr, char *str, command &c);
    void delete_ne (char *str, command &c);
    void delete_ne_do (rbtree_node *curr, char *str, command &c);
    void delete_lt (char *str, command &c);
    void delete_lt_do (rbtree_node *curr, char *str, command &c);
    void delete_gt (char *str, command &c);
    void delete_gt_do (rbtree_node *curr, char *str, command &c);
    void delete_le (char *str, command &c);
    void delete_le_do (rbtree_node *curr, char *str, command &c);
    void delete_ge (char *str, command &c);
    void delete_ge_do (rbtree_node *curr, char *str, command &c);
    
    // useless in the context:
    void do_delete ();
    void mark_list (command &c);
    void delete_into_dlist (list_node *node);
    
public:
    external_index (list &a, bool phone);
    ~external_index ();
    
    //int initialize_rbtree ();
    //int select_in_rbtree (command &c, FILE *fp = stdout);
    //void delete_in_rbtree (command &c);
    int apply_cmd (command &c, FILE *fp = stdout);
    int apply_cmd (command &c, server_string &str);
    
  //  rbtree_node outer = rbtree_node(BLACK, NIL, NIL);
    
};

#endif
