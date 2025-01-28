#include <corecrt_malloc.h>


void free_arr(char** strings, int size)
{
	for (int i = 0; i < size; i++)
	{
		char* str = strings[i];
		free(str);
	}

	free(strings);
}