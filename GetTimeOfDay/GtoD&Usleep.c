#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	struct timeval tv1, tv2;
    	int et_millis, diff_sec, diff_usec;
	if (argc != 2) {
		fprintf (stderr, "Usage: %s sleep time in msecs\n", argv[0]);
		exit(-1);
	}
    	gettimeofday(&tv1, NULL);
    	usleep (atoi(argv[1])*1000);
    	gettimeofday(&tv2, NULL);
    	diff_sec = tv2.tv_sec-tv1.tv_sec;
    	diff_usec = tv2.tv_usec-tv1.tv_usec;
	et_millis=(diff_sec*1000000+diff_usec)/1000;
	printf ("%d\n",et_millis);
}