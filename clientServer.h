#ifndef __CLIENTSERVER_H
#define __CLIENTSERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>
#include <errno.h>

//localhost
#define LOCALHOST "127.0.0.1"
#define MYPORT 8080

//strings
#define SERVER "Server"
#define CLIENT "Client"

//for beauty)
#define SA struct sockaddr

//struct for arguments in pthread_create
typedef struct funcArgc_tag {
    int fd;
    char *buff;
} funcArgc_t;

int Server();
int Client();

//thread func
void create_thread(pthread_t *thread, void *p_func, funcArgc_t *s_struct);
void join_thread(pthread_t *thread);

//read/write to socket
void* Read(void *args);
void* Write(void *args);

//error print and ixit(1)
void err_n_die(const char *format, ...);

#endif
