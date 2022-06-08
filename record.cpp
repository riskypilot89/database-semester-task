#include <algorithm>
#include "record.h"

#define LEN 1234
#define MAX_PRINT 20
#define SPECIAL_CONST 17

//=======================record==========================================//
//=======================================================================//
record:: record (char *name, int phone, int group)
{
    init(name, phone, group);
}

int record:: init (char *name, int phone, int group)
{
    if (name) {
        this -> name = std::unique_ptr<char[]>(new char[strlen(name)+1]);
        if (!this -> name)
            return -1;
        strcpy((this -> name).get(), name);
    }
    else
        this -> name = nullptr;

    this -> phone = phone;
    this -> group = group;

    return 0;
}

record:: record (const record &b)
{
    init((b.name).get(), b.phone, b.group);
}

record:: record (record && x)
{
    phone = x.phone;
    group = x.group;
    name = std::move(x.name);
    x.group = x.phone = 0;
    x.name = nullptr;
}

record:: ~record () {}
//=======================================================================//
record& record:: move (record &b)
{
    if (this == &b)
        return *this;
    
    name = std::move(b.name);
    phone = b.phone;
    group = b.group;
    b.group = b.phone = 0;
    b.name = nullptr;
    
    return *this;
}

void record:: swap (record &b)
{
    int tmp = phone;
    
    name.swap(b.name);
    phone = b.phone;
    b.phone = tmp;
    tmp = group;
    group = b.group;
    b.group = tmp;
}
//=======================================================================//
char* record:: get_name () const
{
    return name.get();
}

int record:: get_phone () const
{
    return phone;
}

int record:: get_group () const
{
    return group;
}
//=======================================================================//
void record:: set_name (char *name)
{
    init_name(name);
}

int record:: init_name (char *namee)
{
    if (namee) {
        name = std:: unique_ptr<char[]> (new char[strlen(namee)+1]);
        if (!name)
            return -1;
        strcpy(name.get(), namee);
    }
    else
        name = nullptr;
    
    return 0;
}

void record:: set_phone (int phone)
{
    this -> phone = phone;
}

void record:: set_group (int group)
{
    this -> group = group;
}
//=======================================================================//
record& record:: operator = (const record &rhs)
{
    init(rhs.name.get(), rhs.phone, rhs.group);

    return *this;
}
//=======================================================================//
int record:: cmp (const record &b) const
{
    if (name.get() && b.name.get()) {
        int res = strcmp(name.get(), b.name.get());
        if (res)
            return res;
        
        if (phone - b.phone)
            return phone - b.phone;
        
        return group - b.group;
    }
    if (!name.get() && b.name.get())
        return -1;
    if (name.get() && !b.name.get())
        return 1;
    
    if (phone - b.phone)
        return phone - b.phone;
    
    return group - b.group;
}

int record:: cmp_btree (const record &b) const
{
    if (group - b.group)
        return group - b.group;
    
    if (phone - b.phone)
        return phone - b.phone;
    
    if (name.get() && b.name.get())
        return strcmp(name.get(), b.name.get());
        
    if (!name.get() && b.name.get())
        return -1;
    
    if (name.get() && !b.name.get())
        return 1;
    
    return 0;
}

int record:: cmp_phone (const record &b) const
{
    if (phone - b.phone)
        return phone - b.phone;
    
    if (group - b.group)
        return group - b.group;
    
    if (name.get() && b.name.get())
        return strcmp(name.get(), b.name.get());
    
    if (!name.get() && b.name.get())
        return -1;
    
    if (name.get() && !b.name.get())
        return 1;
    
    return 0;
}

int record:: cmp_name (const record &b) const
{
    if (name.get() && b.name.get()) {
        int res = strcmp(name.get(), b.name.get());
        if (res)
            return res;
        
        if (phone - b.phone)
            return phone - b.phone;
        
        return group - b.group;
    }
    if (!name.get() && b.name.get())
        return -1;
    if (name.get() && !b.name.get())
        return 1;
    
    if (phone - b.phone)
        return phone - b.phone;
    
    return group - b.group;
}

bool record:: operator < (const record &b) const
{
    return (cmp(b) < 0) ? 1 : 0;
}

bool record:: operator > (const record &b) const
{
    return (cmp(b) > 0) ? 1 : 0;
}

bool record:: operator <= (const record &b) const
{
    return !(b < *this);
}

bool record:: operator >= (const record &b) const
{
    return !(*this < b);
}

bool record:: operator == (const record &b) const
{
    return (cmp(b) == 0) ? 1 : 0;
}

bool record:: operator != (const record &b) const
{
    return !(*this == b);
}
//=======================================================================//
int record:: read (FILE *fp)
{
    char buf[LEN];
    int p, g;
    
    if (fscanf(fp, "%s%d%d", buf, &p, &g) != 3)
        return 1;
    
    return init(buf, p, g);
}

void record:: print (FILE *fp, bool asterisk, char *s) const
{
    if (asterisk) {
        // << name.get() << " " << phone << " " << group << std::endl;
        //if (name.get())
            fprintf(fp, "%s %d %d\n", name.get(), phone, group);
        //else
          //  fprintf(fp, "(noname) %d %d\n", phone, group);
       /* if (name.get())
            fprintf(fp, "  %s", name.get());
        else
            fprintf(fp, "                 ");
        
        long int k = SPECIAL_CONST - strlen(name.get());
        // we need SPECIAL_CONST just because of the way of printing of the files in the classroom
        if (k < 0)
            fprintf(fp, " %d %d\n", phone, group);
        else {
            for (long int i = 0; i < k; ++i)
                fprintf(fp, " ");
            
            fprintf(fp, "%d  %d\n", phone, group);
        }
         */
        return;
    }
    
    if (!strncmp(s, "123", 3)) {
        if (name.get())
            fprintf(fp, "%s %d %d\n", name.get(), phone, group);
        else
            fprintf(fp, "(noname) %d %d\n", phone, group);
    }
    else if (!strncmp(s, "132", 3)) {
        if (name.get())
            fprintf(fp, "%s %d %d\n", name.get(), group, phone);
        else
            fprintf(fp, "(noname) %d %d\n", group, phone);
    }
    else if (!strncmp(s, "213", 3)) {
        if (name.get())
            fprintf(fp, "%d %s %d\n", phone, name.get(), group);
        else
            fprintf(fp, "%d (noname) %d\n", phone, group);
    }
    else if (!strncmp(s, "231", 3)) {
        if (name.get())
            fprintf(fp, "%d %d %s\n", phone, group, name.get());
        else
            fprintf(fp, "%d %d (noname)\n", phone, group);
    }
    else if (!strncmp(s, "312", 3)) {
        if (name.get())
            fprintf(fp, "%d %s %d\n", group, name.get(), phone);
        else
            fprintf(fp, "%d (noname) %d\n", group, phone);
    }
    else if (!strncmp(s, "321", 3)) {
        if (name.get())
            fprintf(fp, "%d %d %s\n", group, phone, name.get());
        else
            fprintf(fp, "%d %d (noname)\n", group, phone);
    }
    else if (!strncmp(s, "12", 2)) {
        if (name.get())
            fprintf(fp, "%s %d\n", name.get(), phone);
        else
            fprintf(fp, "(noname) %d\n", phone);
    }
    else if (!strncmp(s, "21", 2)) {
        if (name.get())
            fprintf(fp, "%d %s\n", phone, name.get());
        else
            fprintf(fp, "%d (noname)\n", phone);
    }
    else if (!strncmp(s, "13", 2)) {
        if (name.get())
            fprintf(fp, "%s %d\n", name.get(), group);
        else
            fprintf(fp, "(noname) %d\n", group);
    }
    else if (!strncmp(s, "31", 2)) {
        if (name.get())
            fprintf(fp, "%d %s\n", group, name.get());
        else
            fprintf(fp, "%d (noname)\n", group);
    }
    else if (!strncmp(s, "23", 2)) {
        fprintf(fp, "%d %d\n", phone, group);
    }
    else if (!strncmp(s, "32", 2)) {
        fprintf(fp, "%d %d\n", group, phone);
    }
    else if (!strncmp(s, "1", 1)) {
        if (name.get())
            fprintf(fp, "%s\n", name.get());
        else
            fprintf(fp, "(noname)\n");
    }
    else if (!strncmp(s, "2", 1)) {
        fprintf(fp, "%d\n", phone);
    }
    else if (!strncmp(s, "3", 1)) {
        fprintf(fp, "%d\n", group);
    }
}

void record:: print (server_string &serv_str, bool asterisk, char *s) const
{
    if (serv_str.left < 256)
        serv_str.send_string();
    
    if (asterisk) {
        serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%.*s %d %d\n", (int) (strlen(name.get())), name.get(), phone, group);
        //fprintf(stderr, "COUNT: %d\n", serv_str.count);
        serv_str.left = serv_str.size_array - serv_str.count;
        
        return;
    }
    
    if (!strncmp(s, "123", 3)) {
        if (name.get())
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%s %d %d\n", name.get(), phone, group);
        else
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "(noname) %d %d\n", phone, group);
    }
    else if (!strncmp(s, "132", 3)) {
        if (name.get())
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%s %d %d\n", name.get(), group, phone);
        else
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "(noname) %d %d\n", group, phone);
    }
    else if (!strncmp(s, "213", 3)) {
        if (name.get())
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d %s %d\n", phone, name.get(), group);
        else
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d (noname) %d\n", phone, group);
    }
    else if (!strncmp(s, "231", 3)) {
        if (name.get())
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d %d %s\n", phone, group, name.get());
        else
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d %d (noname)\n", phone, group);
    }
    else if (!strncmp(s, "312", 3)) {
        if (name.get())
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d %s %d\n", group, name.get(), phone);
        else
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d (noname) %d\n", group, phone);
    }
    else if (!strncmp(s, "321", 3)) {
        if (name.get())
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d %d %s\n", group, phone, name.get());
        else
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d %d (noname)\n", group, phone);
    }
    else if (!strncmp(s, "12", 2)) {
        if (name.get())
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%s %d\n", name.get(), phone);
        else
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "(noname) %d\n", phone);
    }
    else if (!strncmp(s, "21", 2)) {
        if (name.get())
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d %s\n", phone, name.get());
        else
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d (noname)\n", phone);
    }
    else if (!strncmp(s, "13", 2)) {
        if (name.get())
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%s %d\n", name.get(), group);
        else
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "(noname) %d\n", group);
    }
    else if (!strncmp(s, "31", 2)) {
        if (name.get())
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d %s\n", group, name.get());
        else
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d (noname)\n", group);
    }
    else if (!strncmp(s, "23", 2)) {
        serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d %d\n", phone, group);
    }
    else if (!strncmp(s, "32", 2)) {
        serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d %d\n", group, phone);
    }
    else if (!strncmp(s, "1", 1)) {
        if (name.get())
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%s\n", name.get());
        else
            serv_str.count += sprintf((serv_str.s) + (serv_str.count), "(noname)\n");
    }
    else if (!strncmp(s, "2", 1)) {
        serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d\n", phone);
    }
    else if (!strncmp(s, "3", 1)) {
        serv_str.count += sprintf((serv_str.s) + (serv_str.count), "%d\n", group);
    }
    
    serv_str.left = serv_str.size_array - serv_str.count;
}
//=======================================================================//
