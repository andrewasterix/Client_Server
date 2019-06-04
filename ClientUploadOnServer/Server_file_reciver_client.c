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

int main(int argc, char *argv[]){

    int sock =0, confd = 0,b,tot;
    struct sockaddr_in serv_addr;

    char buff[1025];
    int num;

    if (argc != 2) {
        fprintf(stderr,"Usage: %s port\n", argv[0]);
        exit(-1);
    }

    int port= atoi(argv[1]);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                fprintf(stderr, "Error in socket\n");
                exit(-1);
        }
    
    fprintf (stderr, "socket done\n");
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buff, '0', sizeof(buff));

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port= htons(port);

    if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        fprintf(stderr, "Error in bind\n");
        exit(-1);
    }
    fprintf(stderr, "Bind Done\n");

    if(listen(sock, 10) < 0){
        fprintf(stderr, "Error in listen\n");
        exit(-1);
    }
    fprintf(stderr, "Listen Done\n");

    confd = accept(sock, (struct sockaddr*)NULL, NULL);
    if (confd==-1) {
        perror("Accept");
    }
        
    FILE* fp = fopen( "text_received.jpg", "wb");
    tot=0;
    if(fp != NULL){
        while( (b = recv(confd, buff, 1024,0))> 0 ) {
                tot+=b;
                fwrite(buff, 1, b, fp);
        }

        printf("Received byte: %d\n",tot);
        
        if (b<0)
            perror("Receiving");

            fclose(fp);
        } else {
            perror("File");
        }
        close(confd);
    
    return 0;
}