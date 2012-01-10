//
//  Maze.c
//  Project4
//
//  Created by Zachary Jones on 10/23/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Maze.h"

Maze* newMazeFromFile(FILE* inputFile)
{
   Maze* maze = malloc(sizeof(Maze));
   char buffer[64]; // 64 bytes is decent, these lines shouldn't be long
   
   // Load first line in buffer for examination
   getNextLineFromFile(buffer, inputFile);
   
   // Obtain N and M
   sscanf(buffer, "%d %d", &(maze->N), &(maze->M));
   
   // Get the starting point
   getNextLineFromFile(buffer, inputFile);
   sscanf(buffer, "%d %d", &(maze->start.X), &(maze->start.Y));
   
   // Get the ending point
   getNextLineFromFile(buffer, inputFile);
   sscanf(buffer, "%d %d", &(maze->ending.X), &(maze->ending.Y));
   
   maze->mazeDefinition = allocateMatrix(maze->N, maze->M);
   maze->mazePath = allocateMatrix(maze->N, maze->M);
   
   int i, j;
   char* token;
   
   // Après moi, le deluge
   for (i=0; i < maze->N; ++i) 
   {
      getNextLineFromFile(buffer, inputFile);
      token = strtok(buffer, " ");
      for (j=0; j < maze->M; ++j) {
         sscanf(token, "%d", &(maze->mazeDefinition[i][j]));    
         token = strtok(NULL, " ");
      }
   }
   
   // Set start and end points
   setMazeCellFlag(maze, maze->start.X, maze->start.Y, 'S');
   setMazeCellFlag(maze, maze->ending.X, maze->ending.Y, 'F');
   
   return maze;
}

void setMazeCellFlag(Maze* maze, int x, int y, char flag)
{
   maze->mazePath[x][y] = flag;
}

int** allocateMatrix(int N, int M)
{
   int** matrix = malloc(sizeof(int*) * N);
   int i;
   for (i=0; i < N; ++i) {
      matrix[i] = malloc(sizeof(int) * M);
      memset(matrix[i], 0, M);
   }
   return matrix;
}


void getNextLineFromFile(char* buffer, FILE* file)
{
   fgets(buffer, 64, file);
}

void printMazeBorder(int M)
{
   int i;
   for (i=0; i < M; ++i) {
      printf("+---");
   }
   printf("+\n");
}

void printMaze(Maze* maze)
{
   int i, j, cellWalls;
   char rowDivider[64];
   
   printMazeBorder(maze->M);
   
   for (i=0; i < maze->N; ++i) {
      for (j=0; j < maze->M; ++j) {
         cellWalls = maze->mazeDefinition[i][j];
         
         if (j == 0) {
            if (cellWalls & kWest) printf("|");
            else printf(" ");  
         }
         
         if (maze->mazePath[i][j]) {
            printf(" %c ", maze->mazePath[i][j]);
         } else printf("   ");
         
         if (cellWalls & kEast) printf("|");
         else printf(" ");
         
         if (cellWalls & kSouth) {
            strncat(rowDivider, "+---", 4);
         } else {
            strncat(rowDivider, "+   ", 4);
         }

      }
      printf("\n%s+\n", rowDivider);
      rowDivider[0] = 0;
   }
}

int solveMaze(Maze* maze)
{
   return searchPath(maze, maze->start.X, maze->start.Y);
}

int searchPath(Maze* maze, int x, int y)
{

   if (x < 0 || x >= maze->N ||
       y < 0 || y >= maze->M ||
       maze->mazePath[x][y] == 'O' ||
       maze->mazePath[x][y] == 'X') 
   {
      return 0;
   }
   
   if (maze->ending.X == x && maze->ending.Y == y) {
      return 1;
   }
   
   stepMaze(maze);
   
   if (maze->mazePath[x][y] != 'S') {
      setMazeCellFlag(maze, x, y, 'O');
   }
   
   int cellWalls = maze->mazeDefinition[x][y];
   int search;
   
   if ((cellWalls & kNorth) == 0) {
      search = searchPath(maze, x-1, y);
      if (search) return 1;
   }

   if ((cellWalls & kSouth) == 0) {
      search = searchPath(maze, x+1, y);
      if (search) return 1;
   }

   if ((cellWalls & kEast) == 0) {
      search = searchPath(maze, x, y+1);
      if (search) return 1;
   }
   
   if ((cellWalls & kWest) == 0) {
      search = searchPath(maze, x, y-1);
      if (search) return 1;
   }
   
   if (maze->mazePath[x][y] != 'S') {
      stepMaze(maze);
      setMazeCellFlag(maze, x, y, 'X');
   }
   
   return 0;
   
}

void stepMaze(Maze* maze)
{
   printMaze(maze);   
   while(1) {
      int c = getchar();
      if(c == '\n' || c == EOF || c== '\r') break;
   }
}

