/**
 *  MenuLib.c
 *
 *  A library of C functions for simplifying typical 
 *  menu operations.
 *
 *  @version 1.0
 *  @author Zachary Jones <zacharytamas@gmail.com>
 *  @releasedate October 11, 2011
 */

#include <stdio.h>
#include "MenuLib.h"

void clearBuffer(void)
{
   while(getchar() != '\n');
}

void screenFlush()
{
   printf("\033[2J");
   printf("\033[0;0f");    
}

void pressEnterKey()
{
   printf("\n\nPress ENTER to continue.");
   
   clearBuffer();
   
   while(1) {
      int c = getchar();
      if(c == '\n' || c == EOF || c== '\r') break;
   }
}

int getMenuChoice(int numOfMenuOptions)
{
   int menuChoice;
   printf("===\n");
   while (1) {
      printf("Your choice: ");
      scanf("%d", &menuChoice);
      if (menuChoice > 0 && menuChoice <= numOfMenuOptions) {
         return menuChoice;
      }
      printf("Invalid choice.\n");
   }
}