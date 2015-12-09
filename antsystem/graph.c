#include "graph.h"


graph* newgraph(int n)
{
	int i,j;
	graph* g = (graph*)  malloc(sizeof(graph));
	g->n = n;
	g->edges = (edge**) malloc(n*sizeof(edge*));

	for(i=0;i<n;i++)
	{
		g->edges[i] = (edge*)  malloc(n*sizeof(edge));
		for(j=0;j<n;j++)
		{
			g->edges[i][j].predecessor = i;
			g->edges[i][j].successor = j;
			g->edges[i][j].pheromone = 1;
		}
	}

	return g;
}


void initgraph(graph* g)
{
	int i,j;

	for(i=0;i<g->n;i++)
	{
		for(j=0;j<g->n;j++)
		{
			g->edges[i][j].predecessor = i;
			g->edges[i][j].successor = j;
			g->edges[i][j].pheromone = 1;
		}
	}
}


edge* getedge(graph* g,int a,int b)
{
	if(a<b)
		return &g->edges[a][b];
	else
		return &g->edges[b][a];
}