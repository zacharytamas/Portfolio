#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HIGH 1000000
#define LOW  1

// prototype to make Xcode shutup
void genNums (FILE*,int);

/**
 * Generates numbers randomly and writes them into a binary file.
 *
 * @param fp the file pointer of the binary file to write data
 * @param n  the number of integers to be generated
 */
void genNums (FILE *fp, int n)
{
	int           i, num;
	unsigned long start, finish, elapsedTime;
   
	// write counter to binary file
	fwrite (&n, sizeof(int), 1, fp);
   
	// initialize random generater
	srand((int)time(NULL));
   
	// start timer
	
	start = clock();
   
	// generate numbers
	for (i=0; i < n; i++) {
		num = (rand() % (HIGH - LOW + 1)) + LOW;
		fwrite (&num, sizeof(int), 1, fp);
	}
   
	finish = clock();
	elapsedTime = (finish-start) / (CLOCKS_PER_SEC / 1000);
	
	printf ("Elapsed time: %li msecs.\n", elapsedTime);
}

int main(int argc, char *argv[]) 
{
	FILE *fp;
	int n;
	
	if (argc != 3) {
		printf ("Usage genNums <filename> <N>\n");
		printf ("<filename> the name to store randomly generated integers\n");
		printf ("<N> the number of integers to be generated\n");
	}
   
	fp = fopen(argv[1], "wb");
	if (fp == NULL) {
		printf ("Unable to open \"%s\"\n", argv[1]);
		return 1;
	}
   
	// number of integers to be generated
	n = atoi(argv[2]);
	
	// generate data
	genNums (fp, n);
   
	// clos binary file
	fclose (fp);
   
   return 0;
}
