#ifndef _20212022_H_
#define _20212022_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char **Maze;
int **Set;
int Width, Height, i, j, k, tmp;
FILE * fp;

void Init_Maze();
void Make_Maze();
void Save_Maze();
void Free_Maze();

#endif