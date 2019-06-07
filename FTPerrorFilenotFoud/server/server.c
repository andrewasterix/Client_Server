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

// Server varibili
int server, client, slen, clen, len;
 
// Porta
int port;

// Buffer in lettura
char buffer[1024];

// Comandi
char cmd[10];

// SockAddress
struct sockaddr_in server_addr, client_addr;

//
struct stat obj;

int main(int argc, char const *argv[])
{   
    if (argc != 2) {
        fprintf(stderr,"Usage: %s port\n", argv[0]);
        exit(-1);
    }

    port= atoi(argv[1]);
        
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "[-] Socket Error\n");
        exit(-1);
    }
    printf("[+] Socket Done\n");

    bzero((char *) &server_addr, sizeof(server_addr));
        
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    if (bind(server, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "[-] Bind Error\n");
        exit(-1);
    }
    
    printf("[+] Bind Done\n");

    if (listen(server, 5) < 0) {
        fprintf(stderr, "[-] Listen Error\n");
        exit(-1);
	}
    
    printf("[+] Listen Done\n");

    clen = sizeof(client_addr);
        
    if ((client = accept(server, (struct sockaddr *) &client_addr, &clen)) < 0) {
        fprintf(stderr, "[-] Accept Error\n");
        exit(-1);
    }
    
    printf("[+] Accepted\n");

    char *welcome = "Welcome In FTP Server\nUsage:\nPUT - to upload\nGET - to download\nLS - list of file\nQUIT - bye\n";
    write(client, welcome, strlen(welcome));

    fprintf(stdout, "%s\n", welcome);

    while (1){
        memset(&buffer, 0, sizeof(buffer));
        memset(&cmd, 0, sizeof(cmd));
    
        char *message = "What want you to do?\n";
        write(client, message, strlen(message));
        fprintf(stdout, "%s\n", message);

        read(client, cmd, sizeof(cmd));
        fprintf(stdout, "%s\n", cmd);

        if(strncmp(cmd, "PUT", 4) == 0){
        
        /* DEBUG MESSAGE
            char *testP = "Find PUT\n";
            write(client, testP, strlen(testP));
            fprintf(stdout, "%s\n", testP);
        */
        
            char *put = "Insert Filename: ";
            write(client, put, strlen(put));
            fprintf(stdout, "%s", put);

            read(client, buffer, sizeof(buffer));
            fprintf(stdout, "%s\n", buffer);

            int F = open(buffer, O_WRONLY | O_CREAT);

            if(F != -1){

                long size;
                read(client, &size, sizeof(long));

                char transmit[size];
                memset(&transmit, 0, sizeof(transmit));
                fprintf(stdout, "Start Receiving\n");
                read(client, transmit, sizeof(transmit));

                fprintf(stdout, "Start Writing\n");
                write(F, transmit, sizeof(transmit));
                
                fprintf(stdout, "Done\n");

            }else{
                fprintf(stderr, "File not Found!\n");
                break;
            }

        }else if(strncmp(cmd, "GET", 4) == 0){
        
        /* DEBUG MESSAGE
            char *testG = "Find GET\n";
            write(client, testG, strlen(testG));
            fprintf(stdout, "%s\n", testG);
        */
            char *get = "Insert Filename: ";
            write(client, get, strlen(get));
            fprintf(stdout, "%s", get);

            read(client, buffer, sizeof(buffer));
            fprintf(stdout, "%s\n", buffer);

            int F = open(buffer, O_RDONLY);
            
            if(F != -1){

                fstat(F, &obj);
                long size = obj.st_size;
                write(client, &size, sizeof(long));

                char transmission[size];
                memset(&transmission, 0, sizeof(transmission));

                fprintf(stdout, "Start Transmitting\n");
                read(F, transmission, sizeof(transmission));

                fprintf(stdout, "Transmitting ..\n");
                write(client, transmission, sizeof(transmission));
                fprintf(stdout, "Done\n");
            }else {
                fprintf(stderr, "File not Found!\n");
                break;
            }
        }else if(strncmp(cmd, "LS", 3) == 0){
        
        /* DEBUG MESSAGE
            char *testL = "Find LS\n";
            write(client, testL, strlen(testL));
            fprintf(stdout, "%s\n", testL);
        */
            char* filename = "/bin/ls";
            
            FILE* p = popen(filename,"r"); 
            
            if(p != NULL){
                int b = 0;
                while((b = fread(buffer, 1, sizeof(buffer), p)) > 0 ){
                    write(client, buffer, b);
                }
                pclose(p);
            }else{   
                fprintf(stderr, "Error Executing LS");
                break;
            }         

        }else if(strncmp(cmd, "QUIT", 5) == 0){
        
        /* DEBUG MESSAGE
            char *testQ = "Find Q\n";
            write(client, testQ, strlen(testQ));
            fprintf(stdout, "%s\n", testQ);
        */   

            char *bye = "That's Folk!\n";
            write(client, bye, strlen(bye));
            fprintf(stdout, "%s\n", bye);
            break; 

        }else{
            char *notCmd = "Invalid Command\n";
            write(client, notCmd, strlen(notCmd));
        }
    }
    
    close(server);
    close(client);
    return 0;
}
