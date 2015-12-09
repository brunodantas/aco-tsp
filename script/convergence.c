#include <stdio.h>


int main()
{
	double p,q;
	char command[200];
	// for(p=0.1;p<1.0;p+=0.4)
	// {
	// 	for(q=0.5;q<2.5;q*=2)
	// 	{
	// 		snprintf(command,200,"./../antsystem/antsystem m6.txt 50 10 %f %f",p,q);
	// 		system(command);
	// 	}
	// }

	// for(p=0.1;p<1.0;p+=0.4)
	// {
	// 	for(q=0.5;q<2.5;q*=2)
	// 	{
	// 		snprintf(command,200,"./../antsystem/antsystem m15.txt 50 600 %f %f",p,q);
	// 		system(command);
	// 	}
	// }

	// for(p=0.1;p<1.0;p+=0.4)
	// {
	// 	for(q=0.5;q<2.5;q*=2)
	// 	{
	// 		snprintf(command,200,"./../antsystem/antsystem m29.txt 50 5000 %f %f",p,q);
	// 		system(command);
	// 	}
	// }

	for(p=0.1;p<1.0;p+=0.4)
	{
		for(q=0.5;q<2.5;q*=2)
		{
			snprintf(command,200,"./../antsystem/antsystem m38.txt 50 15000 %f %f",p,q);
			system(command);
		}
	}

	return 0;
}