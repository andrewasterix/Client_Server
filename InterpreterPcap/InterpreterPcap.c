#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>

// Variabili Stampa
bool ARP = false;
bool TCP = false;
bool UDP = false;
bool ICMP = false;

// Variabili Per La Lettura del File e dell Iterazioni
int iteratio = 1;
unsigned char c, tmp;
unsigned int i;

// Variabili dei Tempi
unsigned int ts_sec, ts_usec, incl_len, orig_len,
    first_ts_sec, first_ts_usec;
unsigned int diff_sec, diff_usec;
float et_millis;	

// Variabili Ethernet
unsigned char dmac[6], smac[6], ethertype[2],
    ehter1, ehter2, ehter3, ehter4;

// Variabili ARP
unsigned char hwType[2], protocolType[2], opCode[2];
unsigned int hwSize, protocolSize;

// Variabili IP
unsigned char dip[4], sip[4], tos, ipTtL, ipUlP,
    ip_version, header_len;
unsigned int tiplen1, tiplen2;
unsigned short tiplen, Spn, Dpn, spn1, spn2, dpn1, dpn2;

// Nome del File
FILE *fp = NULL;

// Reader Frame Ethernet
void ethernet(FILE *f){

    for (i = 0; i < 6; i++){
        dmac[i] = getc(f);    
    }
    
    for (i = 0; i < 6; i++){
        smac[i] = getc(f);
    
    }
    
    for (i = 0; i < 2; i++){
        ethertype[i] = getc(f);        
    }
    return;
}

// Reader ARP frame
void arp(FILE *f){
    for(i = 0; i < 2; i++){
        hwType[i] = getc(f);
    }

    for(i = 0; i < 2; i++){
        protocolType[i] = getc(f);
    }

    hwSize = getc(f);
    protocolSize = getc(f);

    for(i = 0; i < 2; i++){
        opCode[i] = getc(f);
    }

    for(i = 0; i < 6; i++){
        tmp = getc(f);
    }

    for ( i = 0; i < 4; i++){
        sip[i] = getc(f);
    }
    
    for(i = 0; i < 6; i++){
        tmp = getc(f);
    }

    for ( i = 0; i < 4; i++){
        dip[i] = getc(f);
    }

    ARP = true;
    return;
}

// Reader IP AND (TCP OR UDP) frame 
void ip(FILE *f){

    tmp = getc(f);
    ip_version = (tmp & 0xf0) >> 4;
    header_len = tmp & 0xf;

    tos = getc(f);
	tiplen1 = getc(f);
	tiplen2 = getc(f);
	tiplen = tiplen1*256+tiplen2;

    for (i = 0; i < 4; i++){
        tmp = getc(f);
    }
    
    ipTtL = getc(f);
    ipUlP = getc(f);

    for (i = 0; i < 2; i++){
        tmp = getc(f);
    }
    
    for(i = 0; i < 4; i++){
        sip[i] = getc(f);
    }

    for(i = 0; i < 4; i++){
        dip[i] = getc(f);
    }

    spn1 = getc(f);
    spn2 = getc(f);
    Spn = spn1*256 + spn2;

    dpn1 = getc(f);
    dpn2 = getc(f);
    Dpn = spn1*256 + spn2;

    if(ipUlP == 0x6){
        TCP = true;
    }else if(ipUlP == 0x011){
        UDP = true;
    }else if(ipUlP == 0x01){
        ICMP = true;
    }
    return;
}

void print(){
    diff_sec = ts_sec-first_ts_sec;
	diff_usec = ts_usec-first_ts_usec;
    et_millis=((float) (diff_sec*1000000+diff_usec))/((float) 1000);
	printf ("Iter. %d: Offset in msecs: %.2f\n", iteratio, et_millis);

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

    if(ARP){
        printf ("ARP Hardware type: ");
        for (int i = 0; i < 2 ; i++){
            printf("%02x ", hwType[i]);
        }
                
		printf ("ARP protocol type: ");
        for (int i = 0; i < 2 ; i++){
            printf("%02x ", protocolType[i]);
        }
                
		printf("ARP opCode: ");
        for (int i = 0; i < 2 ; i++){
            printf("%02x", opCode[i]);
        }

		printf ("\n");
		
        printf("Sender MAC address: ");
		for (i=0; i<6;i++) { 
			printf("%02x ", smac[i]);
		}
				
	    printf("Sender IP address: ");
		for (i=0; i<4;i++) { 
			printf("%d.", sip[i]);
		}
                
        printf("\n"); 
        
        printf("Target MAC address: ");
        for (i = 0; i < 6; i++){
            printf("%02x ", dmac[i]);
        }
        
        printf(" ");
        
        printf("Target IP address:");
        for (i = 0; i < 4; i++){
            printf("%d.", dip[i]);
        }

        printf("\n");

    }else if(TCP || UDP){

        printf ("IP Version Number: %02x ", ip_version);
		printf ("IP Header Length %02x ", header_len);
		printf ("IP TTL: %d ", ipTtL);
		printf ("IP Upper Layer Protocol: %02x\n", ipUlP);
		
        printf("IP Source Address: ");
		
        for (i=0; i<4;i++) { 
			printf("%d.", sip[i]);
		}
		
        printf(" ");
		
        printf("IP Destination Address: ");
		for (i=0; i<4;i++) { 
			printf("%d.", dip[i]);
		}
		
        printf("\n");
        
        if (TCP){
            printf("TCP Source Port: %d ", Spn);
            printf("TCP Destination Post: %d \n", Dpn);
        }else if (UDP){
            printf("UDP Source Port: %d ", Spn);
            printf("UDP Destination Post: %d \n", Dpn);
        }
        
    }else if(ICMP){
        printf("ICMP type: %d ", spn1);

        if(spn1 == 0x08){
            printf("Echo Ping Request\n");
        }else if(spn1 == 0x00){
            printf("Echo Ping Reply\n");
        }
    }

    //printf("Arp = %d, TCP = %d, UDP = %d, ICMP = %d", ARP, TCP, UDP, ICMP);
    ARP = TCP = UDP = ICMP = false; 
    //printf("Arp = %d, TCP = %d, UDP = %d, ICMP = %d", ARP, TCP, UDP, ICMP);
}

int main(int argc, char const *argv[])
{
    if(argc != 2){
        fprintf(stderr, "Using: %s name_file.pcap\n", argv[0]);
        exit(-1);
    }

    if((fp = fopen(argv[1], "r")) == 0){
        fprintf(stderr, "Error in reading file %s\n", argv[1]);
        exit(-1);
    }

    for (i = 0; i < 24; i++){
        c = getc(fp);
    }

    while (1){
        if(fread(&ts_sec, sizeof(int), 1, fp) != 1){
            fprintf(stderr, "Error in Reading File\n");
            exit(-1);
        }

        if(fread(&ts_usec, sizeof(int), 1, fp) != 1) {
			fprintf(stderr, "Error in Reading file\n");
			exit (-1);
		}
		if (iteratio == 1) {
			first_ts_sec=ts_sec;
			first_ts_usec=ts_usec;
		}
		if(fread(&incl_len, sizeof(int), 1, fp) != 1) {
			fprintf(stderr, "Error in Reading file\n");
			exit (-1);
		}
		if(fread(&orig_len, sizeof(int), 1, fp) != 1) {
			fprintf(stderr, "Error in Reading file\n");
			exit (-1);
		}

        ethernet(fp);

        tmp = ethertype[0];
        ehter1 = (tmp & 0xf0) >> 4;
        ehter2 = (tmp & 0xf);

        tmp = ethertype[1];
        ehter3 = (tmp & 0xf0) >> 4;
        ehter4 = (tmp & 0xf);

        if(ehter1 == 0x00 && ehter2 == 0x08 && ehter3 == 0x00 && ehter4 == 0x06){
            arp(fp);
            for (i=0; i < (incl_len-40); i++) {
			    c = getc(fp) ;
		    }
        }else if (ehter1 == 0x00 && ehter2 == 0x08 && ehter3 == 0x00 && ehter4 == 0x00){
            ip(fp);
            for (i=0; i < (incl_len-38); i++) {
			    c = getc(fp) ;
		    }   
        }
        
        print();
        iteratio++;
        getchar();
    }
    

    return 0;
}
