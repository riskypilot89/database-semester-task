#include <ctime>
#include <cstdio>
#include "parser.h"
#include "database.h"
#include "external_index.h"

#define BTREE_ORDER 5

// indexes:
// B-tree for groups
// RB-trees for phones or names (into B-tree nodes or just external)

int main (int argc, char *argv[])
{
    list a;
    external_index phones(a, true);
    external_index names(a, false);
    btree groups(BTREE_ORDER, a);
    database db(a, phones, names, groups);
    
    double t;
    if (argc >= 2) {
        const char *filename = argv[1];
        
        t = clock();
        db.read(filename);
        t = (clock() - t) / CLOCKS_PER_SEC;
        fprintf(stdout, "...Elapsed on filling database: %.2f\nStarted executing of queries...\n", t);
    }
    
    parser pars;
    command c;
    
    t = clock();
    while (c.type != cmd_type::QUIT) { // VOID? QUIT?
        c = pars.parse_query();
        
        if (c.type == cmd_type::PERR) {
            t = (clock() - t) / CLOCKS_PER_SEC;
            fprintf(stdout, "Error! This command doesn't exist or the sequence of commands doesn't ends on \"quit;\"\nElapsed = %.2f\n", t);
            return 2;
        }
        
        // c.print_cmd();
        // int res; res =
        db.action(c);
        
        // if (c.type == cmd_type::SELECT)
          // fprintf(stdout, " %d\n", res);
    }
    t = (clock() - t) / CLOCKS_PER_SEC;
    
    // groups.print_btree();
    
    fprintf(stderr, "\nAll queries are applied successfully\nElapsed = %.2f\n", t);
    
    return 0;
}
