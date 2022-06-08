#include "server_string.h"
#include "parser.h"
#include "database.h"
#include "external_index.h"

#define BTREE_ORDER 5

#define PORT "5555"
#define UNKNOWN_COMMAND_MESSAGE "This command doesn't exist\n"
#define BUF_LEN_SERVER 256
#define SERVER_STRING_LEN 10000
#define BACKLOG 20

int main (int argc, char *argv[])
{
    list a;
    external_index phones(a, true);
    external_index names(a, false);
    btree groups (BTREE_ORDER, a);
    database db(a, phones, names, groups);
    
    parser pars;
    command c;
    
    if (argc < 1 || argc > 3) {
        fprintf(stderr, "usage: <[%s] [a.txt] [port]> OR <[%s] [a.txt] (port = %s)> OR <[%s] [port]> OR <[%s] (port = %s)>\n", argv[0], argv[0], PORT, argv[0], argv[0], PORT);
        return 0;
    }
    
    const char *port = PORT, *filename = nullptr;
    
    if (argc == 2) {
        const char *run_check = argv[1];
        
        if (atoi(run_check) <= 0)
            filename = run_check;
        else {
            FILE *fp = fopen(run_check, "r");
            
            if (fp) {
                fclose(fp);
                filename = run_check;
            }
            else
                port = run_check;
        }
    }
    else if (argc == 3) {
        filename = argv[1];
        port = argv[2];
        
        if (atoi(port) <= 0) {
            perror("Eror! Server! Incorrect port");
            return 2;
        }
    }
    
    double t = clock();
    if (filename)
        db.read(filename);
    t = (clock() - t) / CLOCKS_PER_SEC;
    
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    int yes = 1, sock, new_sock, chr;
    
    if (getaddrinfo(NULL, port, &hints, &res)) {
        perror("Error! Server! getaddrinfo");
        return 3;
    }
    
    for (p = res; p; p = p -> ai_next) {
        sock = socket(p -> ai_family, p -> ai_socktype, p -> ai_protocol);
        
        if (sock < 0)
            continue;
        
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
            perror("Error! Server! setsockopt");
            close(sock);
            return 4;
        }
        
        if (!bind(sock, p -> ai_addr, p -> ai_addrlen))
            break;
        else {
            close(sock);
            perror("Error! Server! bind");
            continue;
        }
    }
    
    if (!p) {
        if (sock >= 0)
            close(sock);
        
        fprintf(stderr, "Error! Server! Cannot bind in whole list\n");
        return 5;
    }
    
    freeaddrinfo(res);
    
    if (listen(sock, BACKLOG) < 0) {
        perror("Error! Server! Cannot listen");
        close(sock);
        return 6;
    }
    
    size_t i;
    ssize_t check;
    
    char buf[BUF_LEN_SERVER];
    server_string str(SERVER_STRING_LEN);
    
    fprintf(stderr, "Database is filled for %.2f sec. Clients can connect.\n", t);
    
    for (;;) {
        sin_size = sizeof(their_addr);
        
        if ((new_sock = accept(sock, (struct sockaddr *) (&their_addr), &sin_size)) < 0) {
            perror("accept");
            continue;
        }
        
        str.new_sock = new_sock;
        //fprintf(stderr, "NEW CLIENT\nNEW_SOCK: %d\n", new_sock);
        
        for (;;) {
            //char buf[BUF_LEN_SERVER];
            //server_string str(SERVER_STRING_LEN);
            
            
            if ((check = recv(new_sock, buf, BUF_LEN_SERVER, 0)) == -1)
                continue;
            
            for (i = 0; buf[i] && i < BUF_LEN_SERVER; ++i)
                pars.curr_string[i] = buf[i];
            pars.currstr_pos = 0;
            
            c = pars.parse_query();
            //c.print_cmd();
            
            if (c.type == cmd_type::STOP) {
                str.send_string_final();
                //fprintf(stderr, "hm\n");
                close(new_sock);
                close(sock);
                return 0;
            }
            
            if (c.type == cmd_type::PERR) {
                strcpy(str.s, UNKNOWN_COMMAND_MESSAGE);
                str.count = strlen(str.s);
                str.left -= str.count;
                
                str.send_string();
                
                continue;
            }
            
            if (c.type == cmd_type::QUIT) {
                str.send_string();
                close(new_sock);
                break;
            }
            
            db.action(c, str);
            
            str.send_string_final();
        }
        
        close(new_sock);
    }
    
    //close(sock);
    
   // fprintf(stderr, "hm\n");
    
    return 0;
}
