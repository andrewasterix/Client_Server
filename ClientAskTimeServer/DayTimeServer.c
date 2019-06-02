#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char *argv[]){
        int s, ns, len;
        int csa_len;
        int port;
        char inbuf[65536];
        struct sockaddr_in sa, csa;
        struct timeval t1,t2;
        int et_millis, diff_sec, diff_usec;

        if (argc != 2) {
                fprintf(stderr,"Usage: %s port\n", argv[0]);
                exit(-1);
        }

        port= atoi(argv[1]);
        if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                fprintf(stderr, "Error in socket\n");
                exit(-1);
        }
        fprintf (stderr, "socket done\n");
       
        bzero((char *) &sa, sizeof(sa));
        sa.sin_family=AF_INET;
        sa.sin_addr.s_addr=INADDR_ANY;
        sa.sin_port= htons(port);
       
        if (bind(s, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
                fprintf(stderr, "Error in bind\n");
                exit(-1);
        }
        fprintf (stderr, "bind done\n");
       
        if (listen(s, 5) < 0) {
                fprintf(stderr, "Error in listen\n");
                exit(-1);
		}
        fprintf (stderr, "listen done\n");
       
        csa_len=sizeof(csa);
        if ((ns=accept(s, (struct sockaddr *) &csa, &csa_len)) < 0) {
                fprintf(stderr, "Error in accept\n");
                exit(-1);
        }
       
        fprintf (stderr, "accept done\n");
       
        gettimeofday(&t1, NULL);
	et_millis = (t1.tv_sec*1000000 + t1.tv_usec)/1000;
        send(ns, &et_millis, sizeof(int) , 0 );
        return 0;
}