#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define  SERVER_NAME "127.0.0.1"
#define  SERVER_PORT "5555"
#define  BUF_LEN_CLIENT 3001

int main (int argc, char *argv[])
{
    if (argc < 1) {
        fprintf(stderr, "usage: <[%s] [host's name/IP] [port]> OR <[%s] (for this machine %s and port = %s)>\n", argv[0], argv[0], SERVER_NAME, SERVER_PORT);
        return 1;
    }
    
    const char *host = SERVER_NAME, *port = SERVER_PORT;
    
    if (argc >= 3) {
        const char *run_check = argv[2];
        
        if (atoi(run_check) > 0) {
            FILE *fp = fopen(run_check, "r");
            
            if (fp)
                fclose(fp);
            else
                port = run_check;
        }
        
        host = argv[1];
    }
    
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if (getaddrinfo(NULL, SERVER_PORT, &hints, &res)) {
        fprintf(stderr, "Error! Client! Cannot get information about host %s and port = %s\n", host, port);
        return 3;
    }
    
    int sock;
    
    for (p = res; p; p = p -> ai_next) {
        if ((sock = socket(p -> ai_family, p -> ai_socktype, p -> ai_protocol)) < 0) {
            perror("socket");
            continue;
        }
        
        if (connect(sock, p -> ai_addr, p -> ai_addrlen) < 0) {
            perror("connect");
            close(sock);
            continue;
        }
        else
            break;
    }
    
    if (!p) {
        fprintf(stderr, "Error! Client! Cannot connect!\n");
        return 4;
    }
    
    freeaddrinfo(res);
    
    char buf[BUF_LEN_CLIENT];
    ssize_t check;
    size_t len, buflen, zu_size = sizeof(size_t);
    
    bool sent_exit = false;
    
    fprintf(stdout, "Connected. Input the queries you want to execute\n");
    
    while (fgets(buf, BUF_LEN_CLIENT, stdin)) {
        if (!strncmp(buf, "quit;", 5) || !strncmp(buf, "stop;", 5))
            sent_exit = true;
        
        len = strlen(buf) + 1;
        
        if (send(sock, buf, len, 0) != ((ssize_t) (len))) {
            perror("Error! Client! Cannot send"); // We need to send it separately in a good program, but my program isn't good too much
            close(sock);
            return 4;
        }
        
       // int i = 0;
        for (check = recv(sock, &buflen, zu_size, 0); buflen; check = recv(sock, &buflen, zu_size, 0)) {
            if (check == -1) {
                perror("Error! Client! Cannot read length");
                //fprintf(stderr, "I: %d\n", i);
                //fprintf(stderr, "BUFLEN: %zu\n", buflen);
                close(sock);
                return 5;
            }
            
           // fprintf(stderr, "BUFLEN: %zu\n", buflen);
            
            check = recv(sock, buf, buflen, 0);
            if (check == -1) {
                perror("Error! Client! Cannot read data");
                close(sock);
                return 6;
            }
            
           // size_t kk = buflen - check;
            
            //fprintf(stderr, "BUFLEN - CHECK: %zu\n", kk);
            
            buf[check] = buf[BUF_LEN_CLIENT-1] = '\0';
            
            fprintf(stdout, "%s", buf);
            //++i;
        }
        
        if (sent_exit) {
            close(sock);
            return 0;
        }
    }
    
    close(sock);
    
    return 0;
}
