/*
  * Universita' di Genova - Corso di Laurea in Ingegneria Informatica
  * Programma client da configurare per prendere i tempi di ricezione
  * Destination IP Address: 3.120.240.180
  * Destination TCP Port: 3500
  * Modalita' di compilazione del programma: gcc -w tcp_client.c -o tcp_client
  * Modalita' di attivazine del programma
  * ./tcp_client 3.120.240.180 3500 matricola_dello_studente
*
*/
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
   
int main(int argc, char const *argv[]) 
{ 
    	struct sockaddr_in address; 
    	int sock = 0, valread; 
	int i, j;
	int len;
    	struct sockaddr_in serv_addr; 
    	char matricola[100];
    	char server [100];
	char inbuf[1000];
    	int port;

    	 if (argc != 4) {
      		fprintf(stderr, "Usage %s destination-ip destination-port matricola\n", argv[0]);
      		exit(-1);
    	}
	strcpy(matricola, argv[3]);

    	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        	printf("\n Socket creation error \n"); 
        	return -1; 
    	} 
   
    	memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    	serv_addr.sin_family = AF_INET; 
    	serv_addr.sin_port = htons(atoi(argv[2])); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
  	  if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)  { 
        	printf("\nInvalid address/ Address not supported \n"); 
        	return -1; 
    	} 
   	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n"); 
        	return -1; 
    	} 

	for (i = 0; i < sizeof (inbuf); i++) inbuf[i]=0;
	len = recv(sock, inbuf, sizeof(inbuf), 0);
	printf("Received ");
	for (i=0; i < len; i++) {
		printf("%c", inbuf[i]);
	}
	printf(" - ");
    	send(sock , matricola , strlen(matricola) , 0 ); 
    	printf("Sent %s\n", matricola); 

	for (i=0; i < strlen(matricola); i++) {
		for (j = 0; j < sizeof (inbuf); j++) inbuf[j]=0;
		len = recv(sock, inbuf, sizeof(inbuf), 0);

		printf("Received ");
		for (j=0; j < len; j++) {
			printf("%c", inbuf[j]);
		}
		printf("\n");
	}
    	return 0; 
} 
