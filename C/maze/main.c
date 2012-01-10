//
//  main.c
//  Project4
//
//  Created by Zachary Jones on 10/23/11.
//

#include <stdio.h>

#include "Maze.h"

int main (int argc, const char * argv[])
{
   
   if (argc != 2) {
      printf("Usage instructions: \n");
      printf("  ./maze input.txt\n\n");
      return 1;
   }
   
   FILE* mazeFile;
   
   if ((mazeFile = fopen(argv[1], "r")) == NULL) {
      printf("The maze file could not be opened.\n");
      return 1;
   }
   
   // mazeFile is opened now, we know this
   Maze* maze = newMazeFromFile(mazeFile);
   int result = solveMaze(maze);
   
   if (!result) {
      printf("\nMaze is unsolvable.\n");
   }
   
   return 0;
}

