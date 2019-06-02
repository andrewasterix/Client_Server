#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){

    int sock = 0, confd = 0, b, tot = 0;
    char rbuff[1024];
    char sendbuffer[100];

    struct sockaddr_in serv_addr;

    if (argc != 3) {
      fprintf(stderr, "Usage %s ip-address port\n", argv[0]);
      exit(-1);
    }
    
    memset(rbuff, '0', sizeof(rbuff));

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 


    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    FILE* fp = fopen("text_received.txt", "w");

    if(fp != NULL){
        while((b = recv(confd, sendbuffer, 1024, 0)) < 0){
            tot += b;
            fwrite(sendbuffer, 1, b, fp);
        }

        printf("Recived Byte: %d\n", tot);
    }else{
        fprintf(stderr, "Error Reading File\n");
        exit(-1);
    }

    fclose(fp);
    close(confd);
    return 0;
}