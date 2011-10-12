/**
 * COT 3011, University of West Florida
 *
 * @author Zachary Jones <zacharytamas@gmail.com>
 * @date April 1, 2011
 *
 * The following are a few string manipulation 
 * functions I had to write for an algorithms class. 
 * I had been programming in C for about two months.
 */

#include <stdio.h>

int findString(char*, char*);
void removeString(char*, int, int);
void insertString(char*, char*, int);

int main(int argc, const char* argv[])
{
   char str1[15] = "Zachary 89";
   printf("\nremoveString:\n");
   printf("  Original String: %s\n", str1);
   removeString(str1, 4, 4);
   printf("  After removed  : %s\n", str1);

   char str2[14] = "Zach Jones";
   printf("insertString:\n");
   printf("  Original String: %s\n", str2);
   insertString(str2, "ary", 4);
   printf("  After insertion: %s\n", str2);

   printf("findString:\n");
   printf("  Finding 'ary' in 'Zachary Jones': %d\n", findString("Zachary Jones", "ary"));
   printf("  Finding 'ary' in 'Jones, Zachary': %d\n", findString("Jones, Zachary", "ary"));
   printf("  Finding 'ary' in 'Adam Smith': %d\n", findString("Adam Smith", "ary"));
   printf("  Finding 'ary' in 'University of West Florida': %d\n", findString("University of West Florida", "ary"));

   printf("\n");
   return 0;
}

/**
* Searches a string for a substring and returns the 
* position it occurs or -1 if it does not occur.
*
* @param *str The string to search inside.
* @param *substr The sub string to search for.
* @return int The index the substring occurs, or 
*   -1 if it does not.
*/
int findString(char* str, char* substr)
{
   char* p1 = str;
   char* p2 = substr;
   char* j;

   while (*p1 != 0) {
      j = p1; // set the jumpback pointer
      if (*p1 == *p2) {
         while ( (*p1 == *p2) && *p1 != 0) { p1++; p2++; }
         if (*p2 == 0) {
            return j - str - 1; // -1 to 0-index it
         } else { // reset
            p1 = j; // jump back, the string didn't pan out to what we wanted
            p2 = substr; 
         }
      }
      p1++;
   }

   return -1;
}


/**
* Removes a portion of a string given a starting index and length.
*
* @param *str String to be manipulated.
* @param loc Starting index
* @param length The length of characters to be removed.
*/
void removeString(char* str, int loc, int length)
{
   str += loc;
   while ( *(str+length) != 0) *str++ = *(str+length);
   *str = 0; // insert a new terminator
}


/**
* Inserts a string inside another string at a given position.
*
* @param *str The string to be extended.
* @param *inserStr The string to be inserted.
* @param loc The 0-indexed position in the host string to insert the new string.
*/
void insertString(char* str, char* insertStr, int loc)
{
   int len2 = 0, i = 0;
   char* insertionPoint = str + loc; // address to the insertion point
   
   // find the length of the string we're inserting
   while (*(insertStr+len2) != 0) len2++;
   // advance to the end
   while (*str != 0) str++;
   
   // working backwards, move characters over to make room for insertion
   while (str >= insertionPoint) {
      *(str + len2) = *str--;
   }

   // Now insert the new string into the string at the insertion point
   while (*insertStr != 0)
      *insertionPoint++ = *insertStr++;
}

