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
	unsigned char c, tmp,tmp1;
	unsigned char dmac[6], smac[6], ethertype[2],tmac[6];
	unsigned char ipvn, ihl,bit1,bit2,bit3,bit4;
	unsigned char ipttl, ipulp, tos;
	short tiplen;
	unsigned char sip[4], dip[4];
	unsigned short spn1, spn2, dpn1, dpn2, spn, dpn;
	unsigned char hardwaretype[2],protocoltype[2],opcode[2];
    unsigned char sender_ip[4],target_ip_address[4];
	unsigned int hardwareSize,protocolSize;

	int tiplen1, tiplen2;

	FILE *f, *fopen();
	unsigned int ts_sec, ts_usec, incl_len, orig_len;
	unsigned int first_ts_sec, first_ts_usec;

	if (argc!= 2) {
		fprintf(stderr, "Usage: %s file_name", argv[0]);
		exit(-1);
	}	

	if ((f = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Error in opening %s \n", argv[1]);
		exit(-1);
	}
	
	for (i=0; i < 24; i++) {
		c = getc(f) ;
	}
	iter = 1;

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
			fprintf(stderr, "Error in Reading file11\n");
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
        //PARTE PER ARP!
		//
		//PROVA DI CONTROLLO DI ETHERTYPE
		tmp1 = ethertype[0];
		bit1=(tmp1 & 0xf0)>>4;
		bit2 = tmp1 & 0xf;
		tmp1 = ethertype[1];
		bit3 = (tmp1&0xf0)>>4;
		bit4 = (tmp1&0xf);
		if (bit1 == 0x00 && bit2 == 0x08 && bit3 == 0x00 && bit4 == 0x06){
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
				printf ("ARP Hardware type :");
                    for (int i = 0; i < 2 ; i++){
                        printf("%02x ",hardwaretype[i]);
                    }
                
				printf ("ARP protocol type: ");
                    for (int i = 0; i < 2 ; i++){
                        printf("%02x ",protocoltype[i]);
                    }
                
				printf("ARP opcode: ");
                for (int i = 0; i < 2 ; i++){
                        printf("%02x",opcode[i]);
                    }

				printf ("\n");
				printf("Sender MAC address: ");
				for (i=0; i<6;i++) { 
					printf("%02x ", smac[i]);
				}
				
				printf("Sender IP address: ");
				for (i=0; i<4;i++) { 
					printf("%d.", sender_ip[i]);
				}
                printf(" \n "); 
                printf("Target MAC address: ");
                for (i = 0; i < 6; i++){
                    printf("%02x ",tmac[i]);
                }
                printf(" ");
                printf("Target IP address:");
                     for (i = 0; i < 4; i++){
                    printf("%d.",target_ip_address[i]);
                }
                for (i=0; i < (incl_len-40); i++) {
				c = getc(f) ;
				}
				
		}else if (bit1 == 0x0 && bit2 == 0x08 && bit3 == 0x00 && bit4 == 0x00){

			printf("IP");
		tmp = getc(f);
		ipvn=(tmp & 0xf0)>>4;
		ihl = tmp & 0xf;
		tos=getc(f);

		tiplen1=getc(f);
		tiplen2=getc(f);
		tiplen = tiplen1*256+tiplen2;

		for (i=0; i<4;i++) { 
			tmp=getc(f);
		}
		ipttl=getc(f);
		ipulp=getc(f);
		for (i=0; i<2;i++) { 
			tmp=getc(f);
		}
		for (i=0; i< 4; i++) { 
			sip[i]=getc(f);
		}
		for (i=0; i< 4; i++) { 
			dip[i]=getc(f);
		}
		spn1=getc(f);
		spn2=getc(f);
		spn=spn1*256+spn2;
		dpn1 = getc(f);
		dpn2 = getc(f);
		dpn=dpn1*256+dpn2;
 		for (i=0; i < (incl_len-38); i++) {
			c = getc(f) ;
		}
// printf("\n iter %d **** %d %d \n", iter, spn, dpn);
// getchar();

			//if ((spn == 3500) || (dpn == 3500)) {
    				diff_sec = ts_sec-first_ts_sec;
    				diff_usec = ts_usec-first_ts_usec;
				et_millis=((float) (diff_sec*1000000+diff_usec))/((float) 1000);
				printf ("Iter. %d: Offset in msecs: %.2f\n", iter, et_millis);

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
				printf ("IP Version Number: %02x ", ipvn);
				printf ("IP Header Length %02x ", ihl);
				printf ("IP TTL: %d ", ipttl);
				printf ("IP Upper Layer Protocol: %02x\n", ipulp);
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
				if (ipulp == 0x06) {
					printf("TCP Source Port: %d ", spn);
					printf("TCP Destin.  Port: %d\n", dpn);
				}else if (ipulp == 0x011){
                    printf("UDP Source Port: %d ", spn);
					printf("UDP Destin.  Port: %d\n", dpn);
                }
				else if (ipulp == 0x01){
					printf("ICMP type : %d ",spn1);
					if (spn1 == 0x08){
						printf("Echo Ping Request");
					}else if (spn1 == 0x00){
						printf("Echo Ping Reply");
					}
				}
			
		}
	
	iter++;
	}
