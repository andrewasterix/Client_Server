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

int main(int argc, char const *argv[])
{
    int Sock = 0, NSock = 0, lenght, Nlenght;
    struct sockaddr_in serverAddr, nServerAddr;

    char buffer[1024];
    

    if(argc != 3){
        fprintf(stderr, "Usage: %s port file\n", argv[0]);
        exit(-1);
    }

    int port = atoi(argv[1]);
    if((Sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "Error Creation Socket\n");
        exit(-1);
    }

    printf("[+] Server Socket Created Successfully\n");

    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if(bind(Sock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
        fprintf(stderr, "Error in Binding\n");
        exit(-1);
    }

    printf("[+] Bind to Port Number: %s\n", argv[1]);

    if(listen(Sock, 10) < 0){
        fprintf(stderr, "Error in Listening\n");
        exit(-1);
    }

    printf("[+] Listening...\n");

    Nlenght = sizeof(nServerAddr);
    if((NSock = accept(Sock, (struct sockaddr *) &nServerAddr, &Nlenght)) < 0){
        fprintf(stderr, "Error in Accepting\n");
        exit(-1);
    }

    printf("[+] Accepted\n");
    
    FILE *F;
    F = fopen(argv[2],"rb");
    
    int b = 0;
    
    if(F == NULL){
        fprintf(stderr, "[+] Error in Reading File\n");
        exit(-1);
    }

    while( (b = fread(buffer, 1, sizeof(buffer), F))>0 ){
        send(NSock, buffer, b, 0);
    }

    fclose(F);
    close(NSock);

    return 0;
}