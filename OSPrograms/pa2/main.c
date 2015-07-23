/* Zachary Allin
   RUID: 138-00-0613 
   CS 214
   Rutgers University
   
   Collaboration with: 
   Jeff DiPaola
   CS 214
   Rutgers University
  
   sorted-list.c
*/

#include 	<stdio.h>
#include 	<stdlib.h>
#include	<string.h>
#include	"sorted-list.h"
#include 	"sorted-list.c"

int compareInts(void *p1, void *p2)
{
	int i1 = (int)p1;
	int i2 = (int)p2;
	
	return (i1 < i2) ? -1 : ((i1 > i2) ? 1 : 0);
}

int compareDoubles(void *p1, void *p2)
{
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

//Destructor functions
void destroyBasicTypeAlloc(void *p)
{
  //For pointers to basic data types (int*, char*, double*,...)
  //Use for allocated memory (malloc, calloc, etc.)
  
  free(p);
}

void destroyBasicTypeNoAlloc(void *p)
{
  //For pointers to basic data types (int*, char*, double*,...)
  //Use for memory that has not been allocated (e.g., "int x = 5;SLInsert(mylist,&x);SLRemove(mylist,&x);")
  return;
}

/*
	test1() will insert ints into the list.  It will insert them so that the front
	of the list is replaced.  An iterator then walks through the list.  The middle item 
	is then removed, and another iterator walks through the list.
*/
void test1()
{
	printf("Begin Test 1\n");
	SortedListPtr joe = SLCreate(compareInts, destroyBasicTypeNoAlloc);
	
	printf("%d is inserted.\n",SLInsert(joe, 4));
	printf("%d is inserted.\n",SLInsert(joe, 1));
	printf("%d is inserted.\n",SLInsert(joe, 5));
	
	printf("All inserted\n");
		
	SortedListIteratorPtr jim = SLCreateIterator(joe);

	int i;
	while((i = (int) SLNextItem(jim)) != NULL)
		printf("%d\n", i);
		
	// Remove "4" from the sorted list
	SLRemove(joe, 4);
	printf("4 has been removed from the list.\n");
	
	jim = SLCreateIterator(joe);
	while((i = (int) SLNextItem(jim)) != NULL)
		printf("%d\n", i);
	
	
	printf("End of Test 1\n");
	fflush(0);
	
	SLDestroyIterator(jim);
	SLDestroy(joe);
}


int main()
{
	test1();
	
	return 0;
}
