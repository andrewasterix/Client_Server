# include <stdio.h>
# include <stdlib.h>
# include<string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

void main(int argc, char *argv[]){

	int iter;
    int diff_sec, diff_usec;
	float et_millis;
	unsigned int i;
	unsigned char c, tmp;
	unsigned char dmac[6], smac[6], ethertype[2],tmac[6];
    unsigned char hardwaretype[2],protocoltype[2],opcode[2];
    unsigned char sender_ip[4],target_ip_address[4];
	unsigned char ipvn, ihl;
	unsigned char ipttl, ipulp, tos;
    unsigned int hardwareSize,protocolSize;
	short tiplen;
	unsigned char sip[4], dip[4];
	unsigned short spn1, spn2, dpn1, dpn2, spn, dpn;

	int tiplen1, tiplen2;

	FILE *f, *fopen();
	unsigned int ts_sec, ts_usec, incl_len, orig_len;
	unsigned int first_ts_sec, first_ts_usec;

	if (argc!= 2) {
		fprintf(stderr, "Usage: %s file_name\n", argv[0]);
		exit(-1);
	}	

	if ((f = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Error in opening %s \n", argv[1]);
		exit(-1);
	}
	
	for (i=0; i < 24; i++) {
		c = getc(f) ;
	}
	
	
	if(fread(&ts_sec, sizeof(int), 1, f) != 1) {
		fprintf(stderr, "Error in Reading file1\n");
		exit (-1);
	}
	
	if(fread(&ts_usec, sizeof(int), 1, f) != 1) {
		fprintf(stderr, "Error in Reading file2\n");
		exit (-1);
	}

	if (iter == 1) {
		first_ts_sec=ts_sec;
		first_ts_usec=ts_usec;
	}

	if(fread(&incl_len, sizeof(int), 1, f) != 1) {
		fprintf(stderr, "Error in Reading file1\n");
		exit (-1);
	}

	if(fread(&orig_len, sizeof(int), 1, f) != 1) {
		fprintf(stderr, "Error in Reading file22\n");
		exit (-1);
    }

    for (i=0; i<6;i++) { 
		dmac[i]=getc(f);
	}
	
	for (i=0; i<6;i++) { 
		smac[i]=getc(f);
	}
	
	for (i=0; i<2;i++) { 
		ethertype[i]=getc(f);
	}
	
	//PROVA DI CONTROLLO DI ETHERTYPE
    //COMINCIA LA PARTE DI ARP
    for (i = 0; i <2; i++){
        hardwaretype[i] = getc(f);
    }
    
	for (i = 0; i <2 ; i++){
        protocoltype[i] = getc(f);
    }
    
	hardwareSize = getc(f);
    protocolSize = getc(f);
    
	for (int i = 0; i  < 2; i++){
        opcode[i] = getc(f);
    }
    
	for (int i = 0; i < 6; i++){
        tmp = getc(f);
    }
    
	for (int i = 0; i < 4; i++){
        sender_ip[i] = getc(f);
    }
    
	for (int i = 0; i < 6; i++){
        tmac[i] = getc(f);
    }
    
	for (int i = 0; i < 4; i++){
        target_ip_address[i] = getc(f);
    }
    
	printf("DMAC: ");
	for (i=0; i<6;i++) { 
		printf("%02x ", dmac[i]);
	}
	
	printf(" ");
	printf("SMAC: ");
	for (i=0; i<6;i++) { 
		printf("%02x ", smac[i]);
	}
	
	printf(" ");
	printf("ETHERTYPE: ");
	for (i=0; i<2;i++) { 
		printf("%02x ", ethertype[i]);
	}
	printf("\n");
	
	printf ("ARP HARDWARE TYPE :");
    for (int i = 0; i < 2 ; i++){
        printf("%02x ",hardwaretype[i]);
    }
                
	printf ("ARP PROTOCOL TYPE: ");
    for (int i = 0; i < 2 ; i++){
        printf("%02x ",protocoltype[i]);
    }
               
	printf("ARP OPCODE: ");
    for (int i = 0; i < 2 ; i++){
        printf("%02x",opcode[i]);
    }

	printf ("\n");
	printf("SENDER MAC ADDRESS ");
	for (i=0; i<6;i++) { 
		printf("%02x ", smac[i]);
	}
				
	printf("SENDER IP ADDRESS: ");
	for (i=0; i<4;i++) { 
		printf("%d.", sender_ip[i]);
	}
    
	printf("\n");
	 
    printf("TARGET MAC ADDRESS: ");
    for (i = 0; i < 6; i++){
        printf("%02x ",tmac[i]);
    }
    
	printf(" ");
    printf("TARGET IP ADDRESS:");
    for (i = 0; i < 4; i++){
        printf("%d.",target_ip_address[i]);
    }

	printf("\n");
}
        
   