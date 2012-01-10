//
//  Maze.h
//  Project4
//
//  Created by Zachary Jones on 10/23/11.
//

#include <stdio.h>

#ifndef h_Maze
#define h_Maze

#define kNorth 1
#define kEast  2
#define kSouth 4
#define kWest  8

typedef struct MazeCell
{
   int X, Y;
} MazeCell;

typedef struct Maze
{
   int N, M;
   int** mazeDefinition;
   int** mazePath;
   MazeCell start;
   MazeCell ending;
} Maze;

/**
 * Reads a file of a maze description to initialize 
 * a Maze object.
 */
Maze* newMazeFromFile(FILE*);

/**
 * Accepts a char pointer and a FILE pointer. The next 
 * line of the file will be read into the char pointer.
 */
void getNextLineFromFile(char*, FILE*);

/**
 * Allocates a 2-dimensional array with N rows 
 * and M columns.
 */
int** allocateMatrix(int N, int M);

/**
 * Accepts a maze, x and y coordinates, and a flag as 
 * a char to place on that cell of the maze.
 */
void setMazeCellFlag(Maze* maze, int x, int y, char flag);

/**
 * Starts recursively solving a given maze.
 */
int solveMaze(Maze* maze);

/**
 * Searches a path of a given maze starting at the 
 * cell at the given coordinates.
 */
int searchPath(Maze* maze, int x, int y);

/**
 * Prints a textual representation of a given maze 
 * as it appears at the moment.
 */
void printMaze(Maze* maze);
void printMazeBorder(int M);
void stepMaze(Maze* maze);

#endif
