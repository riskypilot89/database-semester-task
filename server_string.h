#ifndef SERVER_STRING_H
#define SERVER_STRING_H

#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <string>

#define PIECE_LEN 3000

struct server_string
{
    char *s;
    int count = 0;
    int left = -1;
    int size_array;
    
    int new_sock;
    
    size_t bytes_len;
    size_t len_to_send;
    size_t shift;
    size_t zu_size = sizeof(size_t);
    
    int init (int buf_len)
    {
        if (buf_len <= 0)
            return 1;
        
        s = new char[buf_len]{};
        
        if (!s)
            return 2;
        
        size_array = left = buf_len;
        
        return 0;
    }
    
    server_string (int buf_len)
    {
        init(buf_len);
        
       // if (res)
         //   fprintf(stderr, "WOW\nRES: %d\n", res);
       // this -> new_sock = new_sock;
    }
    
    ~server_string ()
    {
        delete [] s;
        size_array = 0;
        count = 0;
        left = -1;
    }
    
    void clear ()
    {
        for (size_t i = 0; s[i]; ++i)
            s[i] = '\0';
            
        count = 0;
        left = size_array;
    }
    
    void send_string ()
    {
        shift = 0;
        
        while (count) {
            if (count > PIECE_LEN)
                len_to_send = PIECE_LEN;
            else
                len_to_send = count;
            
            count -= len_to_send;
            bytes_len = len_to_send * sizeof(char);
            
           // fprintf(stderr, "LEN_TO_SEND: %zu\nBYTES_LEN:%zu\n", len_to_send, bytes_len);
            
            if (send(new_sock, (&bytes_len), zu_size, 0) != (ssize_t) (zu_size)) {
                perror("Sending length");
            }
            
            if (send(new_sock, s + ((ssize_t) (shift)), len_to_send, 0) != (ssize_t) (len_to_send))
                perror("Sending data");
            
            shift += len_to_send;
        }
        
       /* bytes_len = 0;
        if (send(new_sock, &bytes_len, zu_size, 0) != (ssize_t) (zu_size))
            perror("Sending length"); */
        
        clear();
    }
    
    void send_string_final ()
    {
        shift = 0;
        
        while (count) {
            if (count > PIECE_LEN)
                len_to_send = PIECE_LEN;
            else
                len_to_send = count;
            
            count -= len_to_send;
            bytes_len = len_to_send * sizeof(char);
            
           // fprintf(stderr, "LEN_TO_SEND: %zu\nBYTES_LEN:%zu\n", len_to_send, bytes_len);
            
            if (send(new_sock, (&bytes_len), zu_size, 0) != (ssize_t) (zu_size)) {
                perror("Sending length");
            }
            
            if (send(new_sock, s + ((ssize_t) (shift)), len_to_send, 0) != (ssize_t) (len_to_send))
                perror("Sending data");
            
            shift += len_to_send;
        }
        
        bytes_len = 0;
        if (send(new_sock, &bytes_len, zu_size, 0) != (ssize_t) (zu_size))
            perror("Sending length");
        
        clear();
    }
};

#endif
