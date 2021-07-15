#ifndef __CLIENTSERVER_H
#define __CLIENTSERVER_H

//#include <sys/socket.h>





//d#include <netinet/in.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include <sys/types.h>


//localhost
#define LOCALHOST "127.0.0.1"
#define MYPORT 8080

#define SERVER "Server"
#define CLIENT "Client"

typedef struct sendArgs_tag {
    int connfd;
    char *sendBuff;
} sendArgs_t;

typedef struct resvArgs_tag {
    int connfd;
    char *resvBuff;
} resvArgs_t;

int Server();
int Client();

void* Read(void *args);
void* Write(void *args);

#endif
