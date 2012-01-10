//
//  mergesort.c
//  Project5
//
//  Usage: mergesort numbers.bin 5000
//
//  Created by Zachary Jones on 11/6/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Given a file pointer, a pointer to a block of integer space, 
 * and a count of numbers to read, reads that many numbers from 
 * the file and stores them at the integer pointer.
 */
void readNumbersIntoMemory(FILE* file, int* integerArray, int numsToRead);

/**
 * Splits the array in halves recursively and sorts the numbers.
 */
void z_mergesort(int* integerArray, int size);

/**
 * Merges two arrays of numbers together in order.
 */
void z_merge(int* wholeArray, int* left, int* right, int s_left, int s_right);

int main (int argc, const char * argv[])
{
   
   FILE* file;
   int* integerArray;
   int numsToRead;
   
   if (argc != 3) {
      printf("Incorrect number of arguments -- quitting!\n");
      exit(1);
   }
   
   if ((file = fopen(argv[1], "r")) == NULL) {
      printf("Could not open input file -- quitting!\n");
      exit(1);
   }
   
   numsToRead = atoi(argv[2]);
   integerArray = malloc(sizeof(int) * numsToRead);
   
   readNumbersIntoMemory(file, integerArray, numsToRead);
   
   long int start = clock();
   z_mergesort(integerArray, numsToRead);
   double runtime = (clock() - start) / (CLOCKS_PER_SEC / 1000.0);
   
   printf("%d\t%lf\n", numsToRead, runtime);
   
   // int i;
   // for (i=0; i < numsToRead; ++i) {
   //    printf("%8d %d\n", i+1, integerArray[i]);
   // }
   
   fclose(file);
   
   return 0;
}

void readNumbersIntoMemory(FILE* file, int* integerArray, 
                           int numsToRead)
{
   int numsInFile;
   fread(&numsInFile, sizeof(int), 1, file);
   
   if (numsInFile < numsToRead) {
      printf("There aren't enough numbers in the specified file.");
      exit(EXIT_FAILURE);
   }
   
   fread(integerArray, sizeof(int), numsToRead, file);
   
}

void z_merge(int* wholeArray, int* left, int* right, int s_left, int s_right)
{
   int i = 0, j = 0, k = 0;
   
   while (s_left > 0 || s_right > 0) 
   {
      if (s_left > 0 && s_right > 0) 
      {
         if (left[i] <= right[j]) {
            wholeArray[k++] = left[i++];
            s_left--;
         } else {
            wholeArray[k++] = right[j++];
            s_right--;
         }
      } else if (s_left > 0) {
         wholeArray[k++] = left[i++];
         s_left--;
      } else if (s_right > 0) {
         wholeArray[k++] = right[j++];
         s_right--;
      }
   }
   
}

void z_mergesort(int* integerArray, int size)
{
   if (size <= 1) { 
      // if size == 1, it is already sorted.
      // if size == 0, it's empty
      // if size < 0,  it's completely screwed up
      return;
   }
   
   int mid = size / 2; // midpoint
   int* left = malloc(sizeof(int) * mid);
   int* right = malloc(sizeof(int) * (size - mid));
   
   memcpy(left, integerArray, sizeof(int) * mid);
   memcpy(right, integerArray + mid, sizeof(int) * (size - mid));
   
   z_mergesort(left, mid);
   z_mergesort(right, size - mid);
   
   z_merge(integerArray, left, right, mid, size-mid);
   
   free(left);
   free(right);
   
}

