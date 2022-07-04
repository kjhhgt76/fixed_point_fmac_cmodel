#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "stdint.h"
int main()
{
	time_t seed = time(NULL);
	srand(seed);
	int i = 0;
	double max = 1.0;
	double sum = 0;
	double coef[5] = {0};
	for (i = 0; i < 5; i++)
	{
		coef[i] = (rand()/(double)RAND_MAX)*(rand()%2?-1:1)*0.2;
		printf("coef[%d]=%.20f\n", i, coef[i]);
	}
	return 0;
}