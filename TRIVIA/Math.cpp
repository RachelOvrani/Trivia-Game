#include <stdlib.h>
#include <time.h>
#include <xlocinfo>

void init_random()
{
	int init_random_number = time(0); // seconds from 1970

	srand(init_random_number);
}

int random(int min, int max)
{
	int range = (max - min) + 1;
	return (rand() % range) + min;
}