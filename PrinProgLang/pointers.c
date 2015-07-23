#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *a= (int *) malloc(sizeof(int));

	*a = 12;
	printf("%x %x: %d\n", &a, a, *a);
	
	//free(a);
	//a = (int *) 12;
	//printf("%d\n", *a);

}
