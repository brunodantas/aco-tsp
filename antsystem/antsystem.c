#include "antsystem.h"


graph* g;
antcolony * a;
edge** bestpath;
double bestdistance = DBL_MAX;
double* pathdistances;
int strt;




antcolony* newantcolony(int n)
{
	int i;
	antcolony* a = (antcolony*) malloc(sizeof(antcolony));
	(*a) = malloc(n*sizeof(ant));

	for(i=0;i<n;i++)
	{
		(*a)[i].visited = (int*) malloc(n*sizeof(int));
		(*a)[i].path = (edge**)  malloc(n*sizeof(edge*));
	}

	return a;
}


void initantcolony(antcolony* a,int n)
{
	int i,j;

	for(i=0;i<n;i++)
	{
		(*a)[i].position = i;

		for(j=0;j<n;j++)
			(*a)[i].visited[j] = 0;
	}
}


int getavailableedges(ant* a,edge** availableedges)
{
	int i;
	int edgeqty=0;
	edge* e;

	for(i=0;i<g->n;i++)
	{
		if(!a->visited[i])
		{
			e = getedge(g,a->position,i);
			availableedges[edgeqty] = e;
			edgeqty++;
		}
	}

	return edgeqty;
}


double visibility(edge* e)
{
	return 1/e->distance;
}


void getprobabilities(int edgeqty, edge** availableedges, double* probabilities)
{
	int i;
	double divisor = 0;

	for(i=0;i<edgeqty;i++)
	{
		probabilities[i] = availableedges[i]->pheromone * visibility(availableedges[i]);
		divisor += probabilities[i];
	}
	for(i=0;i<edgeqty;i++)
	{
		probabilities[i] /= divisor;
	}
}


int roulette(int n,double* probabilities)
{
	int i,r,acc=0;

	r = random()%1000;
	for(i=0;i<n;i++)
	{
		r -= (int) (probabilities[i]*1000);
		if(r<0)
		{
			//printf("%d\n",i);
			return i;
		}
	}
}


void printedges(int edgeqty,edge** availableedges)
{
	int i;
	for(i=0;i<edgeqty;i++)
	{
		printf("%d,%d\t",availableedges[i]->predecessor,availableedges[i]->successor);
	}
	printf("\n");
}


void printprob(int edgeqty,double* probabilities)
{
	int i;
	for(i=0;i<edgeqty;i++)
	{
		printf("%f\t",probabilities[i]);
	}
	printf("\n");
}


edge* chooseedge(ant* a)
{
	int i;
	int edgeqty;
	edge* e;
	edge** availableedges = (edge**) malloc(g->n*sizeof(edge*));
	double* probabilities = (double*) malloc(g->n*sizeof(double));

	edgeqty = getavailableedges(a,availableedges);	//printedges(edgeqty,availableedges);
	getprobabilities(edgeqty,availableedges,probabilities);	//printprob(edgeqty,probabilities);
	e = availableedges[roulette(edgeqty,probabilities)];

	free(availableedges);
	free(probabilities);

	return e;
}


void updateposition(ant* a, edge* e)
{
	if(a->position == e->predecessor)
		a->position = e->successor;
	else
		a->position = e->predecessor;
}


void makepath(ant* a)
{
	int i;
	int initialposition = a->position;

	for(i=0;i<g->n-1;i++)
	{
		a->visited[a->position] = 1;
		a->path[i] = chooseedge(a);
		updateposition(a,a->path[i]);
	}

	a->path[i] = getedge(g,a->position,initialposition);
}


void makepaths(antcolony* a,int n)
{
	int i;

	for(i=0;i<n;i++)
	{
		makepath(&(*a)[i]);
	}
}


void updatepheromone()
{
	int i,j;
	ant* an;

	for(i=0;i<g->n;i++)
	{
		for(j=0;j<g->n;j++)
		{
			g->edges[i][j].pheromone *= (1 - evaporationrate);
		}
	}

	for(i=0;i<g->n;i++)
	{
		an = &(*a)[i];

		for(j=0;j<g->n;j++)
		{
			an->path[j]->pheromone += pheromonedeposit/pathdistances[i];
		}
	}
}


double pathdistance(edge** path)
{
	int i;
	double d = 0;
	for(i=0;i<g->n;i++)
	{
		d += path[i]->distance;
	}
	return d;
}


void updatebestpath(edge** path)
{
	int i;

	for(i=0;i<g->n;i++)
	{
		bestpath[i] = path[i];
	}
}


void evaluatepaths()
{
	int i;

	for(i=0;i<g->n;i++)
	{
		pathdistances[i] = pathdistance((*a)[i].path);
		if(pathdistances[i] < bestdistance)
		{
			bestdistance = pathdistances[i];
			updatebestpath((*a)[i].path);
			strt = i;
		}
	}
}


void printpath(edge** path,int start)
{
	int i;
	edge *e;
	int last;

	printf("path: %s, ",names[start]);
	last = start;

	for(i=0;i<g->n;i++)
	{
		e = path[i];

		if(e->predecessor == last)
		{
			printf("%s, ",names[e->successor]);
			last = e->successor;
		}
		else
		{
			printf("%s, ",names[e->predecessor]);
			last = e->predecessor;
		}
	}
	printf("\ndistance = %f\n",pathdistance(path));
}


double antsystem(int print)
{
	int i;
	a = newantcolony(g->n);
	bestpath = (edge**)  malloc(g->n*sizeof(edge*));
	pathdistances = (double*) malloc(g->n*sizeof(double));

	for(i=0;i<iterations;i++)
	{
		initantcolony(a,g->n);
		makepaths(a,g->n);
		evaluatepaths();
		updatepheromone();
	}

	if(print)
		printpath(bestpath,strt);

	free(a);
	free(bestpath);
	free(pathdistances);

	return bestdistance;
}


void aco(char* filename)
{
	srand(time(NULL));
	g = getinput(filename);
	initgraph(g);
	antsystem(1);
}


void testconvergence(char* filename,int tests,int it,double p,double Q)
{
	int i;
	int seed = time(NULL);
	g = getinput(filename);
	double best = DBL_MAX;
	double result,r;

	int count=0;

	iterations = it;
	evaporationrate = p;
	pheromonedeposit = Q;

	for(i=0;i<tests;i++,seed++)
	{
		bestdistance = DBL_MAX;
		initgraph(g);
		srand(seed);
		result = antsystem(0);
		//r = result; printf("%f\n", r);

		if(result - best < -0.1)	//result < best (result - best) < 0.0
		{
			best = result;
			count = 1;
			//printf(" reset\n");
		}
		else if (abs(result - best) < 1.0) // result == best
		{
			count++;
			//printf(" inc\n");
		}
		else
		{
			//printf(" nope\n");	
		}
	}

	printf("%.1f\t%.1f\t%f\t%d\n",p,Q,best,count);
}


void getpathlessthan(char* filename,double distance)
{
	srand(time(NULL));
	g = getinput(filename);

	do
	{
		bestdistance = DBL_MAX;
		initgraph(g);
	}
	while(antsystem(1) > distance);
}


int main(int argc,char* argv[])
{
	if(argc==2)
		aco(argv[1]);

	else if(argc==3)
		getpathlessthan(argv[1],atof(argv[2]));

	else
		testconvergence(argv[1],atoi(argv[2]),atoi(argv[3]),atof(argv[4]),atof(argv[5]));

	return 0;
}