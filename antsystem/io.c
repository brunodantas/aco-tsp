#include "graph.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


char* text;
double coordinates[100][2];


void readtextfile(char* filename)
{
	int i;
	char filepath[100];
	snprintf(filepath,100,"../problems/%s",filename);
	FILE *f = fopen(filepath, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	text = (char*) malloc(fsize + 1);
	i = fread(text, fsize, 1, f);
	fclose(f);
	text[fsize] = 0;
}


int getnames()
{
	int count = 0;
	int i,j;

	for(i=1,j=0;text[i]!='\n';i++,j++)
	{
		if(text[i]=='\t')
		{
			names[count][j] = '\0';
			j = -1;
			count++;
		}
		else		
			names[count][j] = text[i];
	}
	names[count][j] = '\0';
	count++;
	i++;

	for(j=0;text[i]!='\t';i++,j++)
	{
		names[count][j] = text[i];
	}
	names[count][j] = '\0';
	//printf("%d\n",count+1);
	return count+1;
}


int getnameindex(char* name)
{
	int i;
	for(i=0;i<100;i++)
	{
		if(!strcmp(name,names[i]))
			return i;
	}
}


double readdistance(int* i)
{
	int j;
	char distance[100];

	for(j=0;text[*i]!='\t' && text[*i]!='\n' && text[*i]!='\0';j++,(*i)++)
	{
		distance[j] = text[*i];
	}
	(*i)++;
	distance[j] = '\0';
	return atof(distance);
}


graph* getdistances()
{
	int i,j,skip,lineindex,colindex;
	int count = getnames();
	graph* g = newgraph(count);
	char name[100];
	double d;
	edge* e;

	for(i=1;text[i-1]!='\n';i++);

	for(skip=0;skip<count-1;skip++)
	{
		for(j=0;text[i]!='\t';i++,j++)
		{
			name[j] = text[i];
		}

		i++;
		name[j] = '\0';
		lineindex = getnameindex(name);

		for(j=0;j<skip;i++)
		{
			if(text[i-1]=='\t')
				j++;
		}

		for(;j<count-1;j++)
		{
			colindex = j;
			d = readdistance(&i);
			e = getedge(g,lineindex,colindex);
			e->distance = d;
		}
	}
	return g;
}


graph* getcoordinates()
{
	int count = 0;
	int i,j,k,flag;
	graph* g;
	char number[50];
	edge *e;

	for(i=0;text[i]!='\0';i++)
	{
		if(text[i]=='\n')
		{
			snprintf(names[count],50,"%d",count+1);
			count++;
		}
	}
	if(text[i-1]!='\n')
	{
		snprintf(names[count],50,"%d",i+1);
		count++;
	}
	
	g = newgraph(count);

	for(i=0,j=0,k=0,flag=0;text[i]!='\0';i++)
	{
		if(flag>0)
		{
			number[k] = text[i];
			k++;
		}

		if(text[i]==' ')
		{
			if(flag>0)
			{
				number[k] = '\0';
				coordinates[j][flag-1] = atof(number);	//printf("%f\t",atof(number));
			}
			flag++;
			k=0;
		}

		if(text[i]=='\n')
		{
			number[k] = '\0';
			coordinates[j][flag-1] = atof(number);	//printf("%f\n",atof(number));
			j++;
			flag = 0;
		}
	}
	number[k] = '\0';
	coordinates[j][flag-1] = atof(number);

	for(i=0;i<count;i++)
	{
		for(j=i+1;j<count;j++)
		{
			e = getedge(g,i,j);
			e->distance = sqrt(pow(coordinates[i][0] - coordinates[j][0], 2) + pow(coordinates[i][1] - coordinates[j][1], 2));
			//printf("%f ",e->distance);
		}
		//printf("\n");
	}

	return g;
}


void printgraph(graph* g)
{
	int i,j,k;

	for(i=0;i<g->n;i++)
	{
		printf("\t%s",names[i]);
	}

	for(i=0;i<g->n;i++)
	{
		printf("\n%s",names[i]);
		for(j=0;j<g->n;j++)
		{
			if(i!=j)
			{
				printf("\t%f",getedge(g,i,j)->distance);
			}
			else
				printf("\t0");
		}
	}
	printf("\n");
}


graph* getinput(char* filename)
{
	graph* g;
	readtextfile(filename);

	if(text[0]=='\t')
		g = getdistances();
	else
		g = getcoordinates();

	//printgraph(g);

	return g;
}


// int main(int argc, char* argv[])
// {
// 	getinput(argv[1]);
// 	return 0;
// }