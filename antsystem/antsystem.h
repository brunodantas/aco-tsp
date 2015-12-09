#ifndef ANTSYSTEM_H
#define ANTSYSTEM_H


#include "graph.h"
#include <stdio.h>
#include <float.h>


double evaporationrate = 0.5; //p
double pheromonedeposit = 1; //Q
int iterations = 50000;


typedef struct
{
	int position;
	int* visited;
	edge** path;
}ant;


typedef ant* antcolony;
graph* getinput(char* filename);


#endif