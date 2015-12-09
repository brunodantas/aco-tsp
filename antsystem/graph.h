#ifndef GRAPH_H
#define GRAPH_H


#include <stdlib.h>


char names[100][50];


typedef struct
{
	int predecessor;
	int successor;
	double distance;
	double pheromone;
} edge;


typedef struct
{
	int n;
	edge** edges;
}graph;


graph* newgraph(int n);
edge* getedge(graph* g,int a,int b);
void initgraph(graph* g);


#endif