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
    struct sockaddr_in server_addr, client_addr; 
    int sock = 0; 
    
    char buffer[1024] = {0}; 
    
    int port;

    if (argc != 3) {
        fprintf(stderr, "Usage %s ip-address port\n", argv[0]);
        exit(-1);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        fprintf(stderr, "[-] Socket creation error \n"); 
        exit(-1); 
    } 
    printf("[+] Socket Created\n");

    memset(&server_addr, '0', sizeof(server_addr)); 
   
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(atoi(argv[2])); 
    
    if(inet_pton(AF_INET, argv[1], &server_addr.sin_addr)<=0)  { 
        printf("[-] Invalid address || Address not supported\n"); 
        exit(-1); 
    } 

    int b ;
    if (b = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) { 
        printf("[-] Connection Failed\n"); 
        exit(-1); 
    } 
    printf("[+] Socket Connected\n");
    
    char* message = "Message from Client\n";
    send(sock, message, strlen(message), 0);
    printf("Done\n");
    return 0;
}
