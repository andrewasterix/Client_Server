# include <stdio.h>
#include <stdlib.h>  
# define NITEMS 100
# define NIL 0
# define MAXINT 0x7ffffff
#define print_ip print_dotted

struct ipnet {
	int id;
	int prefix;
	int initial;
	int final;
	int size;
	int rounded_size;
	struct ipnet *next;
};

struct ipnet *t, *first, *last, *position;
int id = 0;
int n_elements[NITEMS];

int n_items;

int bit (int nbit, int t) {
	int x;
	x= ((t >> nbit) &0x1);
	return (x);
}

int set_bit(int nbit, int number) {
	int t;
	t=number | (1 << nbit);
	return(t);
}


int reset_bit(int nbit, int number) {
	int t;
	t=number | (1 << nbit);
	t-= (1 << nbit);
	return(t);
}

int prefix (int n_el) {
	int t, i, j;
	// t=n_el+2;
	t= n_el;
	j=0;
	for (i=1; i < t ; i= i << 1) 
		j++;
	return (32-j);
}

void print_dotted(int n, int prefix) {
	int first, second, third, fourth;
	first=n & 0xff;
	second=(n >> 8) & 0xff;
	third=(n >> 16) & 0xff;
	fourth=(n >> 24) & 0xff;
	printf ("%d.%d.%d.%d/%d", fourth, third, second, first, prefix);
}

void print_binary(int n, int prefix) {
	int i;
	char a[33]; 
	a[32]='\0';
	for (i=31; i >= 0 ; i--) {
		a[i]=0x30 + (n & 0x1);
		n=n >> 1;
	}
	printf ("%s/%d", a, prefix);
}

int round_up(int i) {
	return i;
}

void print_net(struct ipnet *t){
	printf("Id: %d", t->id);
	printf("\tInitial: ");
	print_binary(t->initial, t->prefix);
	printf("\t");
	print_ip(t->initial, t->prefix);
	printf("\n");

	printf("\tFinal:   ");
	print_binary(t->final, t->prefix);
	printf("\t");
	print_ip(t->final, t->prefix);
	printf("\n");
}

set_initial_net(struct ipnet *t){
	int j;
	t->initial=set_bit(0, t->initial);
	t->final= reset_bit(0, t->final);
	for (j=1; j < (32-t->prefix); j++){
      		t->initial=reset_bit(j, t->initial);
      		t->final=set_bit(j, t->final);
	}
	for (j=(32-t->prefix); j< 32; j++){
      		t->initial=reset_bit(j, t->initial);
      		t->final=reset_bit(j, t->final);
	}
	// print_net(t);
}

struct ipnet *lookup (struct ipnet *t){
	struct ipnet *p, *pos;
	int min = MAXINT;
	int max = 0;
	int diff;
	p=first;
	while (p->next) {
		diff = p->next->initial - p->final;
		if (diff > t->size) {
			if (diff < min){
				min = diff;
				pos = p;
			}
		}
		p=p->next;
	}
	if (min == MAXINT) {
		p=first;
		while (p) {
			if (p->final > max) {
				max = p->final;
				pos = p;
			}
		p=p->next;
		}
	}
	return pos;
}

int main() {
	int i, j;
	int index_of_msb_set_to_1;
	int tmp;
	int general_prefix;
	// struct ipnet *t, *first, *last, *position;
	int size;

	fprintf(stderr, "N. items: ");
	scanf ("%d", &n_items);
	fprintf(stderr, "Size: ");
	scanf ("%d", &size);

	first = (struct ipnet *) malloc (sizeof (struct ipnet));
	first->id=id++;
	first->size=size;
	first->rounded_size = round_up(n_items);
	first->prefix=prefix(first->size);
	first->next = NIL;
	last=first;
	set_initial_net(first);
	for (i=0; i < (n_items-1); i++) {
		fprintf(stderr, "Size: ");
		scanf ("%d", &size);
		t = (struct ipnet *) malloc (sizeof (struct ipnet));
		t->id = id++;
		t->size = size;
		t->rounded_size = round_up(n_items);
		t->prefix=prefix(t->size);
		t->next=NIL;
	

		position = lookup(t);
		tmp=position->final;
	      	tmp=set_bit(0, tmp);
      		for (j=1; j < (32-t->prefix); j++)
      			tmp=reset_bit(j, tmp);
      		for (j=32-t->prefix; ((bit (j, tmp) == 1) && (j < 32)); j++) 
      			tmp=reset_bit(j, tmp);
        	tmp=set_bit(j, tmp);
     	 
		t->initial=tmp;
      
		tmp=reset_bit(0, tmp);
		for (j=1; j < (32-t->prefix); j++){
      			tmp=set_bit(j, tmp);
			if(j>index_of_msb_set_to_1)
				index_of_msb_set_to_1=j;
		}

		t->final=tmp;

		t->next=position->next;
		position->next = t;
		last = t;
	}
	t=first;
	while (t) {
		print_net(t);
		t=t->next;
	}

	general_prefix=32-(index_of_msb_set_to_1+1);


	printf("General Prefix: %d", general_prefix);

	/*
	for (i=0; i<n_items; i++){
		printf("\nip[%d].initial: ", i);
		print_binary(ip[i].initial, ip[i].prefix);
		printf("\t");
		print_ip(ip[i].initial, ip[i].prefix);
		printf("\n");

		printf("ip[%d].final:   ", i);
		print_binary(ip[i].final, ip[i].prefix);
		printf("\t");
		print_ip(ip[i].final, ip[i].prefix);
		printf("\n");
	}
	*/
}
