#include "clientServer.h"


int main(int argc, char **argv) {

    if(argc != 2) {
        perror("Error: see README");
        exit(1);
    }
    
    if(!strcmp(SERVER, argv[1])) {
        int status = Server();
        exit(status);
    }
    else if(!strcmp(CLIENT, argv[1])) {
        int status = Client();
        exit(status);
    }
    else {
        perror("Error: see README");
        exit(1);
    }
}

int Server() {

    int listenfd, connfd;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    char resvBuff[1024];

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));
    memset(resvBuff, '0', sizeof(resvBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(MYPORT);
    
    if ( (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) != 0) {
        perror("\n Error : Could not bind \n");
        return 1;
    }

    if ( (listen(listenfd, 10)) != 0) {
        perror("\n Error : Could not listen \n");
        return 1;
    }
    
    while(1) {
        if ( (connfd = accept(listenfd, (struct sockadd *)NULL, NULL)) == -1) {
            perror("\n Error : Could not accept \n");
            return 1;
        }

        resvArgs_t s_resv;
        s_resv.connfd = connfd;
        s_resv.resvBuff = resvBuff;

        sendArgs_t s_send;
        s_send.connfd = connfd;
        s_send.sendBuff = sendBuff;
        

        pthread_t read_p, write_p;
        if(pthread_create(&read_p, NULL, &Read, (void*) &s_resv) != 0) {
            perror("\n pthread read_p create fail\n");
            return 1;
        }
        if(pthread_create(&write_p, NULL, &Write, (void*) &s_send) != 0) {
            perror("\n pthread write_p create fail\n");
            return 1;
        }
        if(pthread_join(read_p, NULL) != 0) {
            perror("\n pthread read_p join fail\n");
            return 1;
        }
        if(pthread_join(write_p, NULL) != 0) {
            perror("\n pthread read_p join fail\n");
            return 1;
        }



        //write(connfd, stdinBuff, strlen(stdinBuff)); 
        //write(connfd, sendBuff, strlen(sendBuff));

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

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("\n Error : Could not create socket \n");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(MYPORT);

    if(inet_pton(AF_INET, LOCALHOST, &serv_addr.sin_addr) <= 0) {
        perror("\n inet_pton error occured\n");
        return 1;
    }

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("\n Error : Connect Failed \n");
        return 1;
    }

    resvArgs_t s_resv;
    s_resv.connfd = sockfd;
    s_resv.resvBuff = resvBuff;

    sendArgs_t s_send;
    s_send.connfd = sockfd;
    s_send.sendBuff = sendBuff;

    pthread_t write_p, read_p;
    if(pthread_create(&write_p, NULL, &Write, (void*) &s_send) != 0) {
        perror("\n write_p create fail\n");
        return 1;
    }
    if(pthread_create(&read_p, NULL, &Read, (void*) &s_resv) != 0) {
        perror("\n read_p create fail\n");
        return 1;
    }
    if(pthread_join(write_p, NULL) != 0) {
        perror("\n write_p join fail\n");
        return 1;
    }
    if(pthread_join(read_p, NULL) != 0) {
        perror("\n read_p join fail\n");
        return 1;
    }
    return 0;
}

void* Read(void *args) {
    resvArgs_t *my_arg = (resvArgs_t*)args;

    int n;
    while( (n = read(my_arg->connfd, my_arg->resvBuff, sizeof(my_arg->resvBuff) - 1)) > 0) {
        my_arg->resvBuff[n] = 0;
        write(1, "> ", 2);
        fflush(stdout); 
        
        if(fputs(my_arg->resvBuff, stdout) == EOF) {
            perror("\n Error : Fputs error\n");
        }
        fflush(stdout); 

        write(1, "\n\n", 2);
        fflush(stdout); 
    }
    if(n < 0)
        perror("\n Read error \n");

    return 0;
}

void* Write(void *args) {
    sendArgs_t *my_arg = (sendArgs_t*)args;

    while(1) {
        write(1, ">", 1);
        scanf("%1024s", my_arg->sendBuff);
        fpurge(stdin);

        write(my_arg->connfd, my_arg->sendBuff, strlen(my_arg->sendBuff));
   }
}

