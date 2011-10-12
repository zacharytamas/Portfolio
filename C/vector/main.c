/*
 *  COP 3530, University of West Florida
 *  Project 3 - main.c
 *
 *  @author Zachary Jones <zacharytamas@gmail.com>
 *  @releasedate October 11, 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "node.h"
#include "MenuLib.h"

int main (int argc, const char * argv[])
{
   
   VectorT* vector = newVector();
   int menuChoice;
   NodeItemT nodeItemCatcher;
   int intCatcher; // used for catching integers temporarily

   while (1) {
      screenFlush();
      printf("\n");
      printf("Welcome to the Vector Proving Ground.\n");
      printf("=====================================\n");
      printf(" 1 - Add an element to the vector\n");
      printf(" 2 - Get an element in the vector\n");
      printf(" 3 - Set an element in the vector\n");
      printf(" 4 - Remove an element in the vector\n");
      printf(" 5 - Show vector size\n");
      printf(" 6 - Search vector for element\n");
      printf(" 7 - Get index of element in vector\n");
      printf(" 8 - Clear the vector\n");
      printf(" 9 - View vector contents\n");
      printf("10 - Exit program\n");
      
      menuChoice = getMenuChoice(10);
      printf("\n");
      clearBuffer();
      
      switch (menuChoice) {
         case 1:
            printf("Character to be added > ");
            scanf("%c", &nodeItemCatcher);
            addElement(vector, nodeItemCatcher);
            break;
         case 2:
            printf("Index to get > ");
            scanf("%d", &intCatcher);
            NodeItemT* item = get(vector, intCatcher);
            if (item == NULL) {
               printf("Item at that index was not found.");
            } else {
               printf("At %d is: %c", intCatcher, *item);
            }
            pressEnterKey();
            break;
         case 3:
            printf("Index to set > ");
            scanf("%d", &intCatcher);
            clearBuffer();
            printf("Node content > ");
            scanf("%c", &nodeItemCatcher);
            int nodeSet = set(vector, nodeItemCatcher, intCatcher);
            if (nodeSet == -1) {
               printf("Could not set at index, no node exists.");
               pressEnterKey();
            }
            break;
         case 4: 
            printf("Index to remove > ");
            scanf("%d", &intCatcher);
            removeElement(vector, intCatcher);
            break;
         case 5:
            printf("Vector size is currently ");
            int vectorSize = size(vector);
            // fancypants pluralization hack
            printf("%d node%c.\n", vectorSize, (vectorSize == 1) ? 0 : 's');
            pressEnterKey();
            break;
         case 6: 
         case 7:
            printf("Character to search for > ");
            scanf("%c", &nodeItemCatcher);
            int index = indexOf(vector, nodeItemCatcher);
            if (index == -1) {
               printf("That character was not found. Maybe they moved away.");
            } else {
               printf("Aha! Found it at %d", index);
            }
            pressEnterKey();
            break;
         case 8: 
            printf("Clearing vector...\n");
            clear(vector);
            printf("  Vector contents are gone like yesterday.");
            pressEnterKey();
            break;
         case 9: 
            dumpVector(vector);
            pressEnterKey();
            break;
         case 10: 
            screenFlush();
            printf("Thanks for testing. You're awesome.");
            exit(0);
            break;
         default:
            printf("Irgendetwas stimmt nicht.");
            break;
      }
   }
   
   return 0;
   
}