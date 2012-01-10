//
//  quicksort.c
//  Project5
//
//  Usage: quicksort numbers.bin 5000
//
//  Created by Zachary Jones on 10/31/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Given a file pointer, a pointer to a block of integer space, 
 * and a count of numbers to read, reads that many numbers from 
 * the file and stores them at the integer pointer.
 */
void readNumbersIntoMemory(FILE* file, int* integerArray, int numsToRead);

/**
 * Implements the quicksort algorithm to recursively 
 * sort the list of integers.
 */
void quicksort(int* integers, int left, int right);

/**
 * Given two integer pointers, swaps the values at those pointers.
 */
void swap(int* A, int* B);


void r_swap(int* integers, int i, int j)
{
   int T = integers[i];
   integers[i] = integers[j];
   integers[j] = T;
}

int partition(int* num, int left, int right) 
{ 
   int i = left, j = right;
   int pivot = num[(left+right)/2];
   while (i <= j) {
      while (num[i] < pivot) i++; 
      while (pivot < num[j]) j--; 
      if (i <= j) {
         r_swap (num, i, j); i++; j--;
      } 
   }
   return i; 
}

void r_quicksort(int* num, int left, int right) {
   int index = partition(num, left, right); 
   
   if (left < index-1)
      quicksort(num, left, index-1);
   if (index < right)
      quicksort(num, index, right);
      
}




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
   quicksort(integerArray, 0, numsToRead);
   double runtime = (clock() - start) / (CLOCKS_PER_SEC / 1000.0);
   
   printf("%d\t%lf\n", numsToRead, runtime);
   
   // int i;
   //    for (i=0; i < numsToRead; ++i) {
   //       printf("%8d %d\n", i+1, integerArray[i]);
   //    }
   
   free(integerArray);
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

void swap(int* A, int* B)
{
   int T = *A;
   *A = *B;
   *B = T;
}

void quicksort(int* integers, int left, int right)
{

   if (right > left + 1) {
      int pivot = integers[left];
      int l = left + 1;
      int r = right;
      
      while (l < r) {
         if (integers[l] <= pivot) l++;
         else swap(&integers[l], &integers[--r]);
      }
      
      swap(&integers[--l], &integers[left]);
      
      quicksort(integers, left, l);
      quicksort(integers, r, right);
   }
   
}
