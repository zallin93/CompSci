/*  Zachary Allin
 *  RUID:  138-00-0613
 *  CS 214
 *  Rutgers University
 */

/*
 *  test.c
 */


/* Used as a test file for the othermalloc implementation. */

#include "othermalloc.h"
#include <string.h>

#define malloc(x) my_malloc(x, __FILE__, __LINE__)
#define free(x)	  my_free(x, __FILE__, __LINE__)


/*  Test 1:  Free memory that has been dynamically allocated.
 *  Dynamically allocate a string (character array). 
 *  	Free the string. 
 *		Free 2 bytes behind the string.  An error should occur.  
 * 		Free the string again.  An error should occur.
 *  
 */
void test1()
{
	printf("Begin Test 1...\n");
	
	char * string = (char *) malloc(sizeof(char) * 5);
	
	free(string);
	free(string-2);
	free(string);
	
	return;
}



/*  Test 2:  Attempt to free memory that was never dynamically allocated.
 *  Declare an integer.  
 *  	Attempt to free the integer.  An error should occur.
 */
void test2()
{
	printf("Begin Test 2...\n");
	
	int x = 9;
	free(&x);
	
	return;
}



/*  Test 3:  One section too full, other has room
 *  Expected Output:  Both arrays should be successfully allocated.
 */
void test3()
{
	printf("Begin Test 3...\n");
	
	char * array = (char *) malloc(sizeof(char) * 2495);
	char * brray = (char *) malloc(sizeof(char) * 500);
	
	if(array)
		printf("array was allocated\n");
		
	if(brray)
		printf("brray was allocated\n");
		
	free(array);
	free(brray);
}



/*  Test 4:  Redundantly freeing the same pointer
 *  Expected Output:  Error message on line 84 for freeing a pointer that was already
 *  freed.
 */
void test4()
{
	printf("Begin Test 4...\n");
	char * p;
	p = (char *) malloc(100);
	free(p);
	p = (char *) malloc(100);
	free(p);
	
	free(p);
}



int main()
{
	int casenum = 0;
	
	while(1)
	{
		printf("Please enter a test case number: \n");
		scanf("%d", &casenum);
	
		switch(casenum)
		{
			case 1:
				test1();
				break;
			case 2: 
				test2();
				break;
			case 3:
				test3();
				break;
			case 4:
				test4();
				break;
			default:
				printf("That is not a valid case number.  Please try again...\n");
				break;
		}
	}
	
	return 0;
	
}