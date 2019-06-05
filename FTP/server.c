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
    int server, client, serverLenght, clientLenght;
    struct sockaddr_in serverAddr, clientAddr;
    
    char buffer[2048];

    if(argc != 2){
        fprintf(stderr, "Usage; %s port\n", argv[0]);
        exit(-1);
    }

    int port = atoi(argv[1]);

    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0){
        fprintf(stderr, "[-] Error Creation Socket\n");
        exit(-1);
    }
    printf("[+] Socket Created\n");

    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(server, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
        fprintf(stderr, "[-] Bind Error\n");
        exit(-1);
    }

    printf("[+] Bind Done\n");

    if(listen(server, 10) < 0){
        fprintf(stderr, "[-] Listen Error\n");
        exit(-1);
    }

    printf("[+] Listening\n");

    clientLenght = sizeof(clientAddr);
    if((client = accept(server, (struct sockaddr *) &clientAddr, &clientLenght)) < 0){
        fprintf(stderr, "[-] Error in Accepting\n");
        exit(-1);
    }

    printf("[+] Accepted\n"); 

    while ((serverLenght = recv(server, buffer, sizeof(buffer), 0))>0) {
        fprintf (stderr, "%d: %s\n", serverLenght, buffer);
    }
    close(server);
    return 0;
}
