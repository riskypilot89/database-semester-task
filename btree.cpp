#include "btree.h"

btree:: btree (size_t m, list &a)
: a(a)
{
    this -> root = nullptr;
    this -> m = m;
}

btree:: ~btree ()
{
    if (root)
        delete_tree(root);
    
    this -> root = nullptr;
    this -> m = 0;
}

btree_node* btree:: get_root ()
{
    return root;
}

void btree:: delete_tree (btree_node *root)
{
    for (size_t i = 0; i < ((root -> cd) + 1); ++i)
        if (root -> links[i])
            delete_tree(root -> links[i]);
    
    delete root;
}

size_t btree:: search_place (btree_node *x, list_node *a)
{
    size_t bottom = 0, top = (x -> cd), s;
    
    while (bottom != top) {
        s = (bottom + top) / 2;
        
        if (x -> head[s] -> group < a -> group)
            bottom = s + 1;
        else
            top = s;
    }
    
    return bottom;
}

int btree:: read_btree (const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return 1;
    
    list_node *tmp = new list_node;
    if (!tmp) {
        fclose(fp);
        return 2;
    }
    
    if (tmp -> read(fp)) {
        delete tmp;
        fclose(fp);
        return 3;
    }
    
    btree_node *left, *right;
    pieces res = add(nullptr, tmp, &left, &right);
    
    if (res.median_head)
        recursion_add(nullptr, res, &left, &right);
    
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
        
        res = add(nullptr, tmp, &left, &right);
        
        if (res.median_head)
            recursion_add(nullptr, res, &left, &right);
    }
}

void btree:: split (btree_node *x, pieces adding, size_t place, pieces *tmp) // without links
{
    if (!(tmp -> right = new btree_node(m)))
        return;
    
    if (place < m) {
        tmp -> median_head = x -> head[m-1];
        tmp -> median_tail = x -> tail[m-1];
        tmp -> median_phones = x -> phones[m-1];
        tmp -> median_names = x -> names[m-1];
    }
    else if (place == m) {
        *tmp = adding;
        if (!adding.median_phones) {
            tmp -> median_phones = new internal_index(adding.median_head, true);
            tmp -> median_names = new internal_index(adding.median_head, false);
            tmp -> median_phones -> add(adding.median_head);
            tmp -> median_names -> add(adding.median_head);
        }
    }
    else {
        tmp -> median_head = x -> head[m];
        tmp -> median_tail = x -> tail[m];
        tmp -> median_phones = x -> phones[m];
        tmp -> median_names = x -> names[m];
    }
    
    size_t i;
    
    if (place <= m)
        for (i = 0; i < m; ++i) {
            tmp -> right -> head[i] = x -> head[i + m];
            tmp -> right -> tail[i] = x -> tail[i + m];
            tmp -> right -> phones[i] = x -> phones[i + m];
            tmp -> right -> names[i] = x -> names[i + m];
            x -> tail[i + m] = x -> head[i + m] = nullptr;
            x -> phones[i + m] = x -> names[i + m] = nullptr;
        }
    else {
        for (i = 0; i < (place - m - 1); ++i) {
            tmp -> right -> head[i] = x -> head[i + m + 1];
            tmp -> right -> tail[i] = x -> tail[i + m + 1];
            tmp -> right -> names[i] = x -> names[i + m + 1];
            tmp -> right -> phones[i] = x -> phones[i + m + 1];
            x -> tail[i + m + 1] = x -> head[i + m + 1] = nullptr;
            x -> phones[i + m + 1] = x -> names[i + m + 1] = nullptr;
        }
        
        tmp -> right -> tail[i] = adding.median_tail;
        tmp -> right -> head[i] = adding.median_head;
        tmp -> right -> names[i] = adding.median_names;
        tmp -> right -> phones[i] = adding.median_phones;
        
        if (!tmp -> right -> names[i]) {
            tmp -> right -> phones[i] = new internal_index(adding.median_head, true);
            tmp -> right -> names[i] = new internal_index(adding.median_head, false);
            tmp -> right -> phones[i] -> add(adding.median_head);
            tmp -> right -> names[i] -> add(adding.median_head);
        }
        
        for (++i; i < m; ++i) {
            tmp -> right -> head[i] = x -> head[i + m];
            tmp -> right -> tail[i] = x -> tail[i + m];
            tmp -> right -> phones[i] = x -> phones[i + m];
            tmp -> right -> names[i] = x -> names[i + m];
            x -> tail[i + m] = x -> head[i + m] = nullptr;
            x -> phones[i + m] = x -> names[i + m] = nullptr;
        }
    }
}

pieces btree:: add (btree_node *x, list_node *a, btree_node **left, btree_node **right)
{
    if (!root)
        if (!(root = new btree_node(m))) {
            pieces res;
            return res;
        }
    
    if (!x)
        x = root;
    
    size_t place = search_place(x, a);
    
    if (place < x -> cd && x -> cd > 0 && a -> group == x -> head[place] -> group) {
        x -> tail[place] -> into_next = a;
        a -> into_prev = x -> tail[place];
        x -> tail[place] = a;
        x -> phones[place] -> add(a);
        x -> names[place] -> add(a);
        pieces res;
        
        return res;
    }
    
    
    if (x -> links[0]) {
        pieces res = add(x -> links[place], a, left, right);
        
        if (res.median_head)
            res = recursion_add(x, res, left, right);
        
        return res;
    }
    else {
        size_t i;
        
        if (x -> cd < 2*m) {
            if (x -> cd)
                for (i = ((x -> cd) - 1); i >= place; --i) {
                    x -> head[i + 1] = x -> head[i];
                    x -> tail[i + 1] = x -> tail[i];
                    x -> phones[i + 1] = x -> phones[i];
                    x -> names[i + 1] = x -> names[i];
                    
                    if (!place && !i)
                        break;
                }
            
            x -> tail[place] = x -> head[place] = a;
            x -> phones[place] = x -> names[place] = nullptr;
            x -> phones[place] = new internal_index(a, true);
            x -> names[place] = new internal_index(a, false);
            x -> phones[place] -> add(a);
            x -> names[place] -> add(a);
            ++(x -> cd);
            
            pieces res;
            return res;
        }
        else {
            pieces tmp, a_pieces(a); // constructor call
            split(x, a_pieces, place, &tmp);
            
            if (place < m) {
                for (i = m - 1; i >= place; --i) {
                    x -> head[i + 1] = x -> head[i];
                    x -> tail[i + 1] = x -> tail[i];
                    x -> phones[i + 1] = x -> phones[i];
                    x -> names[i + 1] = x -> names[i];
                    
                    if (!place && !i)
                        break;
                }
                x -> tail[place] = x -> head[place] = a;
                x -> phones[place] = x -> names[place] = nullptr;
                x -> phones[place] = new internal_index(a, true);
                x -> names[place] = new internal_index(a, false);
                x -> phones[place] -> add(a);
                x -> names[place] -> add(a);
            }
            
            tmp.right -> cd = x -> cd = m;
            *left = x;
            *right = tmp.right;
            
            return tmp;
        }
    }
}

pieces btree:: recursion_add (btree_node *x, pieces adding, btree_node **left, btree_node **right)
{
    if (!x) {
        if (!(root = new btree_node(m))) {
            pieces res;
            return res;
        }
        
        root -> head[0] = adding.median_head;
        root -> tail[0] = adding.median_tail;
        root -> phones[0] = adding.median_phones;
        root -> names[0] = adding.median_names;
        ++(root -> cd);
        
        root -> links[0] = *left;
        root -> links[1] = *right;
        
        pieces res;
        return res;
    }
    
    size_t place = search_place(x, adding.median_head), i;
    
    if (x -> cd < 2*m) {
        if (x -> cd)
            for (i = ((x -> cd) - 1); i >= place; --i) {
                x -> head[i + 1] = x -> head[i];
                x -> tail[i + 1] = x -> tail[i];
                x -> names[i + 1] = x -> names[i];
                x -> phones[i + 1] = x -> phones[i];
                x -> links[i + 2] = x -> links[i + 1];
                
                if (!place && !i)
                    break;
            }
        
        x -> tail[place] = adding.median_tail;
        x -> head[place] = adding.median_head;
        x -> names[place] = adding.median_names;
        x -> phones[place] = adding.median_phones;
        ++(x -> cd);
        
        x -> links[place] = *left;
        x -> links[place + 1] = *right;
        
        pieces res;
        return res;
    }
    else {
        pieces tmp;
        split(x, adding, place, &tmp);
        
        if (place <= m)
            for (i = 0; i < m + 1; ++i) {
                tmp.right -> links[i] = x -> links[i + m];
                x -> links[i + m] = nullptr;
            }
        else {
            for (i = 0; i < (place - m - 1); ++i) {
                tmp.right -> links[i] = x -> links[i + m + 1];
                x -> links[i + m + 1] = nullptr;
            }
            
            tmp.right -> links[i] = *left;
            x -> links[i + m + 1] = *right;
            
            for (++i; i < m + 1; ++i) {
                tmp.right -> links[i] = x -> links[i + m];
                x -> links[i + m] = nullptr;
            }
        }
        
        if (place < m) {
            for (i = m - 1; i >= place; --i) {
                x -> head[i + 1] = x -> head[i];
                x -> tail[i + 1] = x -> tail[i];
                x -> phones[i + 1] = x -> phones[i];
                x -> names[i + 1] = x -> names[i];
                x -> links[i + 2] = x -> links[i + 1];
                
                if (!place && !i)
                    break;
            }
            
            x -> head[place] = adding.median_head;
            x -> tail[place] = adding.median_tail;
            x -> names[place] = adding.median_names;
            x -> phones[place] = adding.median_phones;
            
            x -> links[place] = *left;
            x -> links[place + 1] = *right;
        }
        else if (place == m) {
            x -> links[m] = *left;
            tmp.right -> links[0] = *right;
        }
        
        x -> cd = tmp.right -> cd = m;
        
        *left = x;
        *right = tmp.right;
        
        return tmp;
    }
}

void btree:: print_subtree (btree_node *root, int maxlevel)
{
    printf("MAX_PRINT_NODE_CONSTANT: %d\n", MAX_PRINT_NODE_BTREE);
    printf("==============SUB-B-TREE================\n");
    rec_print(maxlevel, root, 0);
    printf("========================================\n");
}

void btree:: print_btree (int maxlevel)
{
    printf("MAX_PRINT_NODE_CONSTANT: %d\n", MAX_PRINT_NODE_BTREE);
    printf("================B-TREE==================\n");
    rec_print(maxlevel, root, 0);
    printf("========================================\n");
}

void btree:: rec_print (int maxlevel, btree_node *root, int level)
{
    if (!root || level > maxlevel)
        return;
    
    for (size_t i = 0; i < (root -> cd) && i < MAX_PRINT_NODE_BTREE; ++i) {
        if (root -> links[0])
            rec_print(maxlevel, (root -> links)[i], level + 1);
        
        for (int j = 0; j < level; ++j)
            printf("  ");
        
        // printf("%s %d %d\n", ((root -> head)[i]) -> get_name(), ((root -> head)[i]) -> get_phone(), ((root -> head)[i]) -> get_group());
        
        // root -> head[i] -> print_internal_list(root -> head[i]);
        
        printf("%d\n", root -> head[i] -> group);
    }
    
    if(root -> links[0])
        rec_print(maxlevel, (root -> links)[root -> cd], level + 1);
}


void btree:: delete_key (list_node *a)
{
    if (!root)
        return;
    
    rec_delete_key(root, a);
    
    if (!root -> cd) {
        btree_node *tmp = root;
        
        if (root -> links[0]) {
            root = root -> links[0];
            delete tmp;
        }
        else {
            delete tmp;
            root = nullptr;
        }
    }
}

void btree:: delete_into_array_element (btree_node *x, size_t place, list_node *a, bool *flag)
{
    list_node *prev = a -> into_prev, *next = a -> into_next;
    bool del = false;
    
    for (list_node *curr = x -> head[place]; curr; curr = curr -> into_next) {
        if (curr == a) {
            del = true;
            x -> phones[place] -> delete_key(a);
            x -> names[place] -> delete_key(a);
            break;
        }
    }
    
    if (!del)
        return;
    
    if (prev)
        prev -> into_next = next;
    else {
        x -> head[place] = next;
        x -> names[place] -> head = x -> phones[place] -> head = next;
    }
    
    if (next)
        next -> into_prev = prev;
    else {
        x -> tail[place] = prev;
        
        if (prev)
            prev -> into_next = nullptr;
    }
    
    if (!prev && !next)
        *flag = true;
}

void btree:: rec_delete_key (btree_node *x, list_node* a)
{
    bool flag = false;
    
    size_t place = search_place(x, a);
    
    if (place < (x -> cd) && x -> head[place] -> group == a -> group) {
        delete_into_array_element(x, place, a, &flag);
        
        if (!flag)
            return;
        
        delete x -> phones[place];
        delete x -> names[place];
        x -> names[place] = x -> phones[place] = nullptr;
        
        if (x -> links[0])
            delete_into_not_leaf(x, place);
        else
            delete_into_leaf(x, place);
    }
    else if (x -> links[0]) {
        rec_delete_key(x -> links[place], a);
        if (x -> links[place] -> cd < m)
            balance(x, place);
    }
}

size_t btree:: search_group (btree_node *x, int group)
{
    size_t bottom = 0, top = (x -> cd), s;
    
    while (bottom != top) {
        s = (bottom + top) / 2;
        
        if (x -> head[s] -> group < group)
            bottom = s + 1;
        else
            top = s;
    }
    
    return bottom;
}

void btree:: rec_delete_group (btree_node *x, int group)
{
    size_t place = search_group(x, group);
    
    if (place < (x -> cd) && x -> head[place] -> group == group) {
        if (x -> links[0])
            delete_into_not_leaf(x, place);
        else
            delete_into_leaf(x, place);
    }
    else if (x -> links[0]) {
        rec_delete_group(x -> links[place], group);
        if (x -> links[place] -> cd < m)
            balance(x, place);
    }
}

void btree:: delete_into_not_leaf (btree_node *x, size_t place)
{
    if ((x -> links[place] -> cd) > m) {
        full_set max_left_subtree = find_max_left_side(x, place);
        x -> head[place] = max_left_subtree.head;
        x -> tail[place] = max_left_subtree.tail;
        x -> phones[place] = max_left_subtree.phones;
        x -> names[place] = max_left_subtree.names;
        rec_delete_group(x -> links[place], x -> head[place] -> group);
    }
    else if ((x -> links[place + 1] -> cd) >= m) {
        full_set min_right_subtree = find_min_right_side(x, place);
        x -> head[place] = min_right_subtree.head;
        x -> tail[place] = min_right_subtree.tail;
        x -> phones[place] = min_right_subtree.phones;
        x -> names[place] = min_right_subtree.names;
        rec_delete_group(x -> links[place + 1], x -> head[place] -> group);
        
        if (x -> links[place + 1] -> cd < m)
            balance(x, place + 1);
    }
}

full_set btree:: find_max_left_side (btree_node *x, size_t place)
{
    btree_node *curr;
    
    for (curr = x -> links[place]; curr -> links[0]; curr = curr -> links[curr -> cd])
        ;
    
    full_set res(curr -> head[curr -> cd - 1], curr -> tail[curr -> cd - 1], curr -> phones[curr -> cd - 1], curr -> names[curr -> cd - 1]);
    
    return res;
}

full_set btree:: find_min_right_side (btree_node *x, size_t place)
{
    btree_node *curr;
    
    for (curr = x -> links[place + 1]; curr -> links[0]; curr = curr -> links[0])
        ;
    
    full_set res(curr -> head[0], curr -> tail[0], curr -> phones[0], curr -> names[0]);
    
    return res;
}

void btree:: delete_into_leaf (btree_node *x, size_t place)
{
    for (size_t i = place; i < ((x -> cd) - 1); ++i) {
        x -> head[i] = x -> head[i + 1];
        x -> tail[i] = x -> tail[i + 1];
        x -> phones[i] = x -> phones[i + 1];
        x -> names[i] = x -> names[i + 1];
    }
    
    x -> tail[(x -> cd) - 1] = x -> head[(x -> cd) - 1] = nullptr;
    x -> names[(x -> cd) - 1] = x -> phones[(x -> cd) - 1] = nullptr;
    
    --(x -> cd);
}

void btree:: balance (btree_node *x, size_t place) // we have that cd of x -> links[place] equals to m - 1
{
    if (place && (x -> links[place-1] -> cd) > m) {
        size_t i;
        btree_node *left = x -> links[place - 1], *right = x -> links[place];
        
        for (i = ((right -> cd) - 1); i > 0; --i) {
            right -> head[i + 1] = right -> head[i];
            right -> tail[i + 1] = right -> tail[i];
            right -> phones[i + 1] = right -> phones[i];
            right -> names[i + 1] = right -> names[i];
        }
        
        right -> head[i + 1] = right -> head[i];
        right -> tail[i + 1] = right -> tail[i];
        right -> phones[i + 1] = right -> phones[i];
        right -> names[i + 1] = right -> names[i];
        
        if (right -> links[0]) {
            for (i = right -> cd; i > 0; --i)
                right -> links[i + 1] = right -> links[i];
            right -> links[i + 1] = right -> links[i];
            
            right -> links[0] = left -> links[left -> cd];
            left -> links[left -> cd] = nullptr;
        }
        
        right -> head[0] = x -> head[place - 1];
        right -> tail[0] = x -> tail[place - 1];
        right -> phones[0] = x -> phones[place - 1];
        right -> names[0] = x -> names[place - 1];
        x -> head[place - 1] = left -> head[(left -> cd) - 1];
        x -> tail[place - 1] = left -> tail[(left -> cd) - 1];
        x -> phones[place - 1] = left -> phones[(left -> cd) - 1];
        x -> names[place - 1] = left -> names[(left -> cd) - 1];
        left -> tail[(left -> cd) - 1] = left -> head[(left -> cd) - 1] = nullptr;
        left -> names[(left -> cd) - 1] = left -> phones[(left -> cd) - 1] = nullptr;
        
        ++(right -> cd);
        --(left -> cd);
    }
    else if (place != (x -> cd) && (x -> links[place + 1] -> cd) > m) {
        size_t i;
        btree_node *left = x -> links[place], *right = x -> links[place + 1];
        
        left -> head[left -> cd] = x -> head[place];
        left -> tail[left -> cd] = x -> tail[place];
        left -> phones[left -> cd] = x -> phones[place];
        left -> names[left -> cd] = x -> names[place];
        
        if (left -> links[0]) {
            left -> links[(left -> cd) + 1] = right -> links[0];
            for (i = 1; i <= (right -> cd); ++i)
                right -> links[i - 1] = right -> links[i];
            right -> links[right -> cd] = nullptr;
        }
        
        x -> head[place] = right -> head[0];
        x -> tail[place] = right -> tail[0];
        x -> phones[place] = right -> phones[0];
        x -> names[place] = right -> names[0];
        
        for (i = 1; i < (right -> cd); ++i) {
            right -> head[i - 1] = right -> head[i];
            right -> tail[i - 1] = right -> tail[i];
            right -> phones[i - 1] = right -> phones[i];
            right -> names[i - 1] = right -> names[i];
        }
        
        right -> tail[(right -> cd) - 1] = right -> head[(right -> cd) - 1] = nullptr;
        right -> phones[(right -> cd) - 1] = right -> names[(right -> cd) - 1] = nullptr;
        
        ++(left -> cd);
        --(right -> cd);
    }
    else if (place && (x -> links[place-1] -> cd) >= m)
        merge(x, place - 1); // left merge
    else if (place != (x -> cd) && (x -> links[place + 1] -> cd) >= m)
        merge(x, place); // right merge
    else {
        // it cannot be, it's here just for history... I suppose
        printf("===========================ALARM!SMTH_WRONG==========================\n");
        if (place != (x -> cd))
            merge(x, place);
        else
            merge(x, place - 1);
    }
}

void btree:: merge (btree_node *x, size_t place)
{
    btree_node *left = x -> links[place], *right = x -> links[place + 1];
    size_t i, t = (left -> cd == m - 1) ? m - 1 : m;
    
    left -> head[t] = x -> head[place];
    left -> tail[t] = x -> tail[place];
    left -> phones[t] = x -> phones[place];
    left -> names[t] = x -> names[place];
    
    for (i = 0; i < (right -> cd); ++i) {
        left -> head[i + t + 1] = right -> head[i];
        left -> tail[i + t + 1] = right -> tail[i];
        left -> phones[i + t + 1] = right -> phones[i];
        left -> names[i + t + 1] = right -> names[i];
    }
    
    if (left -> links[0])
        for (i = 0; i <= (right -> cd); ++i)
            left -> links[i + t + 1] = right -> links[i];
    
    for (i = place + 1; i < (x -> cd); ++i) {
        x -> head[i - 1] = x -> head[i];
        x -> tail[i - 1] = x -> tail[i];
        x -> phones[i - 1] = x -> phones[i];
        x -> names[i - 1] = x -> names[i];
    }
    
    for (i = place + 2; i <= (x -> cd); ++i)
        x -> links[i - 1] = x -> links[i];
    
    left -> cd += ((right -> cd) + 1);
    --(x -> cd);

    right -> cd = 0;
    
     delete right;
    //right -> remove();
}

int btree:: select_eq (btree_node *x, int g, FILE *fp, bool flag, char *s, command &c, bool phone)
{
    if (!x)
        return 0;
    
    size_t place = search_group(x, g);
    int count = 0;
    
    if (place < x -> cd && (x -> head[place] -> group) == g) {
        if (phone)
            count += x -> phones[place] -> cmd_select(fp, flag, s, c);
        else
            count += x -> names[place] -> cmd_select(fp, flag, s, c);
    }
    else if (x -> links[place])
        count += select_eq(x -> links[place], g, fp, flag, s, c, phone);
    
    return count;
}

int btree:: select_lt (btree_node *x, int g, FILE *fp, bool flag, char *s, command &c, bool phone)
{
    if (!x)
        return 0;
    
    size_t place = search_group(x, g), i;
    int count = 0;
    
    for (i = 0; i < place; ++i) {
        if (phone)
            count += x -> phones[i] -> cmd_select(fp, flag, s, c);
        else
            count += x -> names[i] -> cmd_select(fp, flag, s, c);
    }
    
    if (x -> links[0]) {
        for (i = 0; i <= place; ++i) {
            count += select_lt(x -> links[i], g, fp, flag, s, c, phone);
        }
    }
    
    return count;
}

int btree:: select_gt (btree_node *x, int g, FILE *fp, bool flag, char *s, command &c, bool phone)
{
    if (!x)
        return 0;
    
    size_t place = search_group(x, g), i;
    int count = 0;
    
    if (place < x -> cd && (x -> head[place] -> group) == g) {
        for (i = place + 1; i < x -> cd; ++i) {
            if (phone)
                count += x -> phones[i] -> cmd_select(fp, flag, s, c);
            else
                count += x -> names[i] -> cmd_select(fp, flag, s, c);
        }
        
        if (x -> links[0]) {
            for (i = place + 1; i <= x -> cd; ++i)
                count += select_gt(x -> links[i], g, fp, flag, s, c, phone);
        }
    }
    else if (place == x -> cd) {
        if (x -> links[0]) {
            count += select_gt(x -> links[place], g, fp, flag, s, c, phone);
        }
    }
    else {
        for (i = place; i < x -> cd; ++i) {
            if (phone)
                count += x -> phones[i] -> cmd_select(fp, flag, s, c);
            else
                count += x -> names[i] -> cmd_select(fp, flag, s, c);
        }
        
        if (x -> links[0]) {
            for (i = place; i <= x -> cd; ++i) {
                count += select_gt(x -> links[i], g, fp, flag, s, c, phone);
            }
        }
    }
    
    return count;
}

int btree:: select_le (btree_node *x, int g, FILE *fp, bool flag, char *s, command &c, bool phone)
{
    if (!x)
        return 0;
    
    size_t place = search_group(x, g), i;
    int count = 0;
    
    if (place < x -> cd && (x -> head[place] -> group) == g) {
        for (i = 0; i <= place; ++i) {
            if (phone)
                count += x -> phones[i] -> cmd_select(fp, flag, s, c);
            else
                count += x -> names[i] -> cmd_select(fp, flag, s, c);
        }
        
        if (x -> links[0]) {
            for (i = 0; i <= place; ++i)
                count += select_le(x -> links[i], g, fp, flag, s, c, phone);
        }
    }
    else if (place == x -> cd) {
        if (x -> links[0]) {
            for (i = 0; i <= place; ++i)
                count += select_le(x -> links[i], g, fp, flag, s, c, phone);
        }
    }
    else  {
        for (i = 0; i < place; ++i) {
            if (phone)
                count += x -> phones[i] -> cmd_select(fp, flag, s, c);
            else
                count += x -> names[i] -> cmd_select(fp, flag, s, c);
        }
        
        if (x -> links[0]) {
            for (i = 0; i <= place; ++i) {
                count += select_le(x -> links[i], g, fp, flag, s, c, phone);
            }
        }
    }
    
    return count;
}

int btree:: select_ge (btree_node *x, int g, FILE *fp, bool flag, char *s, command &c, bool phone)
{
    if (!x)
        return 0;
    
    size_t place = search_group(x, g), i;
    int count = 0;
    
    if (place < x -> cd) {
        for (i = place; i < x -> cd; ++i) {
            if (phone)
                count += x -> phones[i] -> cmd_select(fp, flag, s, c);
            else
                count += x -> names[i] -> cmd_select(fp, flag, s, c);
        }
        
        if (x -> links[0]) {
            for (i = place; i <= x -> cd; ++i) {
                count += select_ge(x -> links[i], g, fp, flag, s, c, phone);
            }
        }
    }
    else {
        if (x -> links[0]) {
            count += select_ge(x -> links[place], g, fp, flag, s, c, phone);
        }
    }
    
    return count;
}

void btree:: add_removing (internal_index *x)
{
    if (removing_elements) {
        tail_removing_elements -> r_next = x -> removing_elements;
        x -> removing_elements -> r_prev = tail_removing_elements;
        tail_removing_elements = x -> tail_removing_elements;
    }
    else {
        removing_elements = x -> removing_elements;
        tail_removing_elements = x -> tail_removing_elements;
    }
}

void btree:: delete_eq (btree_node *x, int g, command &c, bool phone)
{
    if (!x)
        return;
    
    size_t place = search_group(x, g);
    
    if (place < x -> cd && x -> head[place] -> group == g) {
        if (phone) {
            x -> phones[place] -> cmd_delete(c);
            add_removing(x -> phones[place]);
        }
        else {
            x -> names[place] -> cmd_delete(c);
            add_removing(x -> names[place]);
        }
    }
    else if (x -> links[0])
        delete_eq(x -> links[place], g, c, phone);
}

void btree:: delete_lt (btree_node *x, int g, command &c, bool phone)
{
    if (!x)
        return;
    
    size_t place = search_group(x, g), i;
    
    for (i = 0; i < place; ++i) {
        if (phone) {
            x -> phones[i] -> cmd_delete(c);
            add_removing(x -> phones[i]);
        }
        else {
            x -> names[i] -> cmd_delete(c);
            add_removing(x -> names[i]);
        }
    }
    
    if (x -> links[0])
        for (i = 0; i <= place; ++i)
            delete_lt(x -> links[i], g, c, phone);
}

void btree:: delete_gt (btree_node *x, int g, command &c, bool phone)
{
    if (!x)
        return;
    
    size_t place = search_group(x, g), i;
    
    if (place < x -> cd && (x -> head[place] -> group) == g) {
        for (i = place + 1; i < x -> cd; ++i) {
            if (phone) {
                x -> phones[i] -> cmd_delete(c);
                add_removing(x -> phones[i]);
            }
            else {
                x -> names[i] -> cmd_delete(c);
                add_removing(x -> names[i]);
            }
        }
        
        if (x -> links[0])
            for (i = place + 1; i <= x -> cd; ++i)
                delete_gt(x -> links[i], g, c, phone);
    }
    else if (place == x -> cd && x -> links[0])
            delete_gt(x -> links[place], g, c, phone);
    else {
        for (i = place; i < x -> cd; ++i) {
            if (phone) {
                x -> phones[i] -> cmd_delete(c);
                add_removing(x -> phones[i]);
            }
            else {
                x -> names[i] -> cmd_delete(c);
                add_removing(x -> names[i]);
            }
        }
        
        if (x -> links[0])
            for (i = place; i <= x -> cd; ++i)
                delete_gt(x -> links[i], g, c, phone);
    }
}

void btree:: delete_le (btree_node *x, int g, command &c, bool phone)
{
    if (!x)
        return;
    
    size_t place = search_group(x, g), i;
    
    if (place < x -> cd && (x -> head[place] -> group) == g) {
        for (i = 0; i <= place; ++i) {
            if (phone) {
                x -> phones[i] -> cmd_delete(c);
                add_removing(x -> phones[i]);
            }
            else {
                x -> names[i] -> cmd_delete(c);
                add_removing(x -> names[i]);
            }
        }
        
        if (x -> links[0]) {
            for (i = 0; i <= place; ++i) {
                delete_le(x -> links[i], g, c, phone);
            }
        }
    }
    else if (place == x -> cd && x -> links[0])
            delete_le(x -> links[place], g, c, phone);
    else  {
        for (i = 0; i < place; ++i) {
            if (phone) {
                x -> phones[i] -> cmd_delete(c);
                add_removing(x -> phones[i]);
            }
            else {
                x -> names[i] -> cmd_delete(c);
                add_removing(x -> names[i]);
            }
        }
        
        if (x -> links[0])
            for (i = 0; i <= place; ++i)
                delete_le(x -> links[i], g, c, phone);
    }
}

void btree:: delete_ge (btree_node *x, int g, command &c, bool phone)
{
    if (!x)
        return;
    
    size_t place = search_group(x, g), i;
    
    if (place < x -> cd) {
        for (i = place; i < x -> cd; ++i) {
            if (phone) {
                x -> phones[i] -> cmd_delete(c);
                add_removing(x -> phones[i]);
            }
            else {
                x -> names[i] -> cmd_delete(c);
                add_removing(x -> names[i]);
            }
        }
        
        if (x -> links[0])
            for (i = place; i <= x -> cd; ++i)
                delete_ge(x -> links[i], g, c, phone);
    }
    else if (x -> links[0])
        delete_ge(x -> links[place], g, c, phone);
}

int btree:: apply_cmd (command &c, FILE *fp)
{
    int i, gr_cond = 0, number = 0;
    bool phone = false;
    
    for (i = 0; i < 2; ++i) {
        if (c.cond[i].cond_left_t == cond_type::_GROUP) {
            gr_cond = i;
            number = c.cond[i].number;
        }
        else if (c.cond[i].cond_left_t == cond_type::_PHONE)
            phone = true;
    }
    
    switch (c.type) {
        case cmd_type::SELECT: {
            long int number_str = 0;
            char s[4];
            
            if (!c.ast) {
                for (i = 0; i < 3; ++i) {
                    if (c.f[i] == field_type::NAME) {
                        if (!i)
                            number_str += 100;
                        else if (i == 1)
                            number_str += 10;
                        else
                            ++number_str;
                    }
                    else if (c.f[i] == field_type::PHONE) {
                        if (!i)
                            number_str += 200;
                        else if (i == 1)
                            number_str += 20;
                        else
                            number_str += 2;
                    }
                    else if (c.f[i] == field_type::GROUP) {
                        if (!i)
                            number_str += 300;
                        else if (i == 1)
                            number_str += 30;
                        else
                            number_str += 3;
                    }
                }
                
                snprintf(s, 4, "%ld", number_str);
            }
            
            switch (c.cond[gr_cond].type_t) {
                case cond_type::EQ: {
                    return select_eq(root, number, fp, c.ast, s, c, phone);
                }
                case cond_type::NE: {
                    // ????????????????????????
                   return a.apply_cmd(c, fp);
                }
                case cond_type::LT: {
                    return select_lt(root, number, fp, c.ast, s, c, phone);
                }
                case cond_type::GT: {
                    return select_gt(root, number, fp, c.ast, s, c, phone);
                }
                case cond_type::GE: {
                    return select_ge(root, number, fp, c.ast, s, c, phone);
                }
                case cond_type::LE: {
                    return select_le(root, number, fp, c.ast, s, c, phone);
                }
                case cond_type::LIKE: {
                    return 0;
                }
                case cond_type::COND_NONE: {
                    return 0;
                }
            }
            return 0;
        }
        case cmd_type::DELETE: {
            switch (c.cond[gr_cond].type_t) {
                case cond_type::EQ: {
                    delete_eq(root, number, c, phone);
                    break;
                }
                case cond_type::NE: {
                    mark_list(c);
                    break;
                }
                case cond_type::LT: {
                    delete_lt(root, number, c, phone);
                    break;
                }
                case cond_type::GT: {
                    delete_gt(root, number, c, phone);
                    break;
                }
                case cond_type::GE: {
                    delete_ge(root, number, c, phone);
                    break;
                }
                case cond_type::LE: {
                    delete_le(root, number, c, phone);
                    break;
                }
                case cond_type::LIKE: {
                    break;
                }
                case cond_type::COND_NONE: {
                    break;
                }
            }
            
            return 0;
        }
        case cmd_type::INSERT: {
            return -4;
        }
        case cmd_type::PERR: {
            return -5;
        }
            
        case cmd_type::VOID: {
            return -6;
        }
            
        case cmd_type::STOP: {
            return -7;
        }
            
        case cmd_type::QUIT: {
            return -8;
        }
            
        case cmd_type::CMD_NONE: {
            return -9;
        }
    }
    
    return 0;
}


void btree:: mark_list (command &c)
{
    a.goto_head();
    
    if (c.cond[0].cond_left_t == cond_type::_FIELD_NONE) {
        while (a.curr) {
            if (removing_elements) {
                tail_removing_elements -> r_next = a.curr;
                a.curr -> r_prev = tail_removing_elements;
                tail_removing_elements = a.curr;
            }
            else
                tail_removing_elements = removing_elements = a.curr;
            
            a.goto_next();
        }
        
        return;
    }
    
    while (a.curr) {
        if (a.curr -> check_on_cmd(c)) {
            if (removing_elements) {
                tail_removing_elements -> r_next = a.curr;
                a.curr -> r_prev = tail_removing_elements;
                tail_removing_elements = a.curr;
            }
            else
                tail_removing_elements = removing_elements = a.curr;
        }
        
        a.goto_next();
    }
}

#include "btree_net.hpp"
