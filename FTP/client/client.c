#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <sys/stat.h>
#include <fcntl.h>

// Server Address
struct sockaddr_in server_addr;

// Socket
int sock;

// Buffer
char buffer[1024];

// Command
char cmd[10];

//
struct stat obj;

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage %s ip-address port\n", argv[0]);
        exit(-1);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        fprintf(stderr, "[-] Socket Creation Error\n"); 
        return -1; 
    }
    printf("[+] Socket Created\n"); 
   
    memset(&server_addr, '0', sizeof(server_addr)); 
   
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(atoi(argv[2])); 
       
    if(inet_pton(AF_INET, argv[1], &server_addr.sin_addr)<=0)  { 
        fprintf(stderr, "[-] Invalid address -> Address not supported\n"); 
        exit(-1); 
    } 
    printf("[+] Address Found\n");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) { 
        fprintf(stderr, "[-] Connection Failed\n"); 
        exit(-1); 
    } 
    printf("[+] Connected\n");
    
    memset(&buffer, 0, sizeof(buffer));
    
    read(sock, buffer, sizeof(buffer));
    fprintf(stdout, "%s", buffer);

    while (1){
        memset(&buffer, 0, sizeof(buffer));
        memset(&cmd, 0, sizeof(cmd));
    
        read(sock, buffer, sizeof(buffer));
        fprintf(stdout, "%s", buffer);
        memset(&buffer, 0, sizeof(buffer));
        
        scanf("%s", cmd);
        write(sock, cmd, strlen(cmd));

        if(strncmp(cmd, "PUT", 4) == 0){
        
        /* DEBUG MESSAGE
            read(sock, buffer, sizeof(buffer));
            fprintf(stdout, "%s", buffer);
        */
            read(sock, buffer, sizeof(buffer));
            fprintf(stdout, "%s", buffer);
            memset(&buffer, 0, sizeof(buffer));
        
            scanf("%s", buffer);
            write(sock, buffer, sizeof(buffer));
            
            int F = open(buffer, O_RDONLY);

            if(F != -1){
                
                fstat(F, &obj);
                long size = obj.st_size;
                fprintf(stdout, "%ld\n", size);

                write(sock, &size, sizeof(long));

                char transmission[size];
                memset(&transmission, 0, sizeof(transmission));
                fprintf(stdout, "Start Transmitting\n");
                read(F, transmission, sizeof(transmission));

                fprintf(stdout, "Transmitting ..\n");
                write(sock, transmission, sizeof(transmission));
                fprintf(stdout, "Done\n");

            }else{
                fprintf(stderr, "File Not found\n");
                break;
            }

        }else if(strncmp(cmd, "GET", 4) == 0){
        
        /* DEBUG MESSAGE
            read(sock, buffer, sizeof(buffer));
            fprintf(stdout, "%s", buffer);
        */   
            read(sock, buffer, sizeof(buffer));
            fprintf(stdout, "%s", buffer);
            memset(&buffer, 0, sizeof(buffer));
        
            scanf("%s", buffer);
            write(sock, buffer, sizeof(buffer));

            int F = open(buffer, O_WRONLY | O_CREAT);

            if(F != -1){
                
                long size;
                read(sock, &size, sizeof(long));

                char trasmission[size];
                memset(&trasmission, 0, sizeof(trasmission));
        
                fprintf(stdout, "Start Receiving\n");
                read(sock, trasmission, sizeof(trasmission));

                fprintf(stdout, "Start Writing\n");
                write(F, trasmission, sizeof(trasmission));
                
                fprintf(stdout, "Done\n");

            }else {
                fprintf(stderr, "File Not found\n");
                break;
            }

        }else if(strncmp(cmd, "LS", 3) == 0){
        
        /* DEBUG MESSAGE
            read(sock, buffer, sizeof(buffer));
            fprintf(stdout, "%s", buffer);
        */
            read(sock, buffer, sizeof(buffer));

            fprintf(stdout, "%s", buffer);

        }else if(strncmp(cmd, "QUIT", 5) == 0){
        
        /* DEBUG MESSAGE
            read(sock, buffer, sizeof(buffer));
            fprintf(stdout, "%s", buffer);
        */

            read(sock, buffer, sizeof(buffer));
            fprintf(stdout, "%s", buffer);
            break;

        }else{
            read(sock, buffer, sizeof(buffer));
            fprintf(stdout, "%s", buffer);
        }
    }
    
    close(sock);
    return 0;
}
