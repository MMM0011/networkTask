#include "clientServer.h"

int main(int argc, char **argv) {

    if(argc != 2) 
        err_n_die("Error: see README");
    
    if(!strcmp(SERVER, argv[1])) {
        int status = Server();
        exit(status);
    }
    else if(!strcmp(CLIENT, argv[1])) {
        int status = Client();
        exit(status);
    }
    else 
        err_n_die("Error: see README");
}

int Server() {

    int listenfd, connfd;
    char sendBuff[1025];
    char resvBuff[1024];
    struct sockaddr_in serv_addr;

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("Error : Could not create socket");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));
    memset(resvBuff, '0', sizeof(resvBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(MYPORT);
    
    if ( (bind(listenfd, (SA*)&serv_addr, sizeof(serv_addr))) != 0)
        err_n_die("Error : Could not bind");

    if ( (listen(listenfd, 10)) != 0)
        err_n_die("Error : Could not listen");

    while(1) {
        if ( (connfd = accept(listenfd, NULL, NULL)) == -1)
            err_n_die("Error : Could not accept");

        write(1, ">", 1);
        //two struct for read/write buffer
        funcArgc_t s_resv, s_send;

        s_resv.fd = connfd;
        s_resv.buff = resvBuff;
        s_send.fd = connfd;
        s_send.buff = sendBuff;
        
        //create two threads for Read() and Write() func
        pthread_t read_p, write_p;

        create_thread(&read_p, &Read, &s_resv);
        create_thread(&write_p, &Write, &s_send);

        join_thread(&read_p);
        join_thread(&write_p);

        close(connfd);
    }

    return 0;
}

int Client() {

    int sockfd = 0, n = 0;
    char sendBuff[1025];
    char resvBuff[1024];
    struct sockaddr_in serv_addr;

    memset(resvBuff, '0', sizeof(resvBuff));
    memset(sendBuff, '0', sizeof(sendBuff));
    memset(&serv_addr, '0', sizeof(serv_addr));

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("Error : Could not create socket");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(MYPORT);

    if(inet_pton(AF_INET, LOCALHOST, &serv_addr.sin_addr) <= 0)
        err_n_die("inet_pton error occured");

    if( connect(sockfd, (SA*)&serv_addr, sizeof(serv_addr)) < 0)
        err_n_die("Error : Connect Failed");

    //two struct for read/write buffer
    funcArgc_t s_resv, s_send;

    s_resv.fd = sockfd;
    s_resv.buff = resvBuff;
    s_send.fd = sockfd;
    s_send.buff = sendBuff;

    write(1, ">", 1);
    //create & join two threads for Read() and Write() func
    pthread_t read_p, write_p;
   
    create_thread(&read_p, &Read, &s_resv);
    create_thread(&write_p, &Write, &s_send);

    join_thread(&read_p);
    join_thread(&write_p);

    return 0;
}

void create_thread(pthread_t *thread, void *p_func, funcArgc_t *s_struct) {
    if(pthread_create(thread, NULL, p_func,(void *) s_struct) != 0)
    err_n_die("thread create fail"); 
}

void join_thread(pthread_t *thread) {
    if(pthread_join(*thread, NULL) != 0)
    err_n_die("thread join fail");
}

void* Read(void *args) {
    funcArgc_t *my_arg = (funcArgc_t*)args;

    int n;
    while( (n = read(my_arg->fd, my_arg->buff, sizeof(my_arg->buff) - 1)) > 0) {
        my_arg->buff[n] = 0;   
        fflush(stdout); 
    
        if(fputs(my_arg->buff, stdout) == EOF) {
            perror("\n Error : Fputs error\n");
        }
        fflush(stdout); 

        write(1, "\n\n>", 3);
        fflush(stdout); 
    }
    if(n < 0)
        perror("\n Read error \n");

    return 0;
}

void* Write(void *args) {
    funcArgc_t *my_arg = (funcArgc_t*)args;

    while(1) {
        scanf("%1024s", my_arg->buff);
        fpurge(stdin);
        write(1, ">", 1);

        write(my_arg->fd, my_arg->buff, strlen(my_arg->buff));
   }
}

void err_n_die(const char *format, ...){
    int errno_save = errno;
    va_list ap;

    //print format+args to stdout
    va_start(ap, format);
    vfprintf(stdout, format, ap);
    fprintf(stdout, "\n");
    fflush(stdout);

    //print error msg if errno != 0
    if(errno_save != 0) {
        fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
        fprintf(stdout, "\n");
        fflush(stdout);
    }
    va_end(ap);

    //die with error
    exit(1);
}   
