#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char const *argv[]){

    int sock = 0, confd = 0, b, tot = 0;
    struct sockaddr_in serv_addr;

    char buffer[1024];
    int num;

    if(argc != 2){
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(-1);
    }

    int port = atoi(argv[1]);

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "Error in socket\n");
        exit(-1);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buffer, '0', sizeof(buffer));

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if(bind(sock, (struct socaddr*) &serv_addr, sizeof(serv_addr))){
        fprintf(stderr, "Error in Bind\n");
        exit(-1);
    }
    fprintf(stderr, "Bind Done\n");

    if(listen(sock, 10) < 0){
        fprintf(stderr, "Error in Listen\n");
        exit(-1);
    }
    fprintf(stderr, "Listen Done");

    if((confd = accept(sock, (struct sockaddr*)NULL, NULL)) != -1){
        fprintf(stderr, "Not Accept\n");
        exit(-1);
    }
    fprintf(stderr, "Accept\n");

    FILE* fp = fopen("text.txt", "r");
    
    if(fp != NULL){
        while((b = fread(buffer, 1, sizeof(buffer), fp)) > 0){
            send(sock, buffer, b, 0);
        }
    }else{
        fprintf(stderr, "Error in Reading File");
    }

    close(confd);

    return(0);
}