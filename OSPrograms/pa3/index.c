/* Zachary Allin
 * RUID: 138-00-0613
 * CS 214
 * Rutgers University
 *
 *
 * index.c
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> /* Directory commands */
#include <sys/types.h> /*stat command, Directory commands*/
#include <sys/stat.h> /*stat command*/
#include <unistd.h> /*stat command*/
#include "sorted-list.h"
#include "tokenizer.h"
#include "uthash.h"

#define BUFFSIZE 256

/* struct to be hash table */
struct tokenStruct
{
	char * key;		/* key for the hash table will be the tokens */
	
	/* Components of each of the entries.  Each will have it's own list of files
	 */
	SortedListPtr listOfFiles;
	SortedListIteratorPtr filePointer;
	
	UT_hash_handle hh;	
};

/* Global variable for hashing. */
struct tokenStruct *words = NULL;


/* Function will determine whether the specified fileName is a file or directory.
 * Returns:	0 if Directory
 * 			1 if File
 */
int isFile(char * fileName)
{
	return 1;
}


/* Function takes a file name, and buffer as arguments.  The buffer is tokenized based 
 * on the alphanumerics and added to the tokenStruct hash table.  
 *
 * Going to send tmp with the fileName into the hash table
 */
void makeTokens(char * fileName, char * buf)
{
	/* Variable Declarations */
	char * tmp;
	char * terminate;
	int initLength = strlen(buf);
	int tmpLength;
	int i;
	
	/* Make all lowercase */
	for(i=0; i<initLength; i++)
		buf[i] = tolower(buf[i]);
	
	/* Separate the buf into its tokens. */
	for(i=0; i<initLength; i=i+tmpLength+1)
	{
		if(buf=='\0')
		{
			printf("buf is null");
			break;
		}
		
		tmp = buf;
		while( isalnum(*buf) != 0)
			buf++;
		terminate = buf;
		buf = buf++;
		*terminate = '\0';
		
		tmpLength = strlen(tmp);		
		printf("tmp is: %s\n", tmp); /* testing the print out of the token */
		
	}
	
	/* End of function */
	return;
}

/* Function will accept a file stream and file name as parameters.  It will attempt to 
 * read the contents of the file into buf.    
 */
void readFile(char * buf, FILE * f)
{
	if(f == NULL)
	{
		printf("The file was not found.\n");
		exit(0);
	}
	
	fread(buf, sizeof(char), BUFFSIZE, f);
	
	printf("buf has been established: %s with size %d\n", buf, (int)strlen(buf));
	
	return;
}

/* Function will attempt to open the file passed through as a string argument. 
 */
void openFile(char * fileName, char * buf)
{
	struct stat s;
	int err = stat(fileName, &s);
	if(S_ISDIR(s.st_mode))
	{
		printf("opening directory %s\n", fileName);
		DIR *dirp = opendir(fileName);
		struct dirent * dp;
		do 
		{
			if ((dp = readdir(dirp)) != NULL && strcmp(dp->d_name,".") != 0 && \
				strcmp(dp->d_name, "..") !=0) 
			{
				printf("d_name is %s\n", dp->d_name);
				/*stat(dp->d_name, &s);
				if(S_ISDIR(s.st_mode))
				{
					printf("%s is a directory.\n", dp->d_name);
					openFile(dp->d_name, buf);
				}*/
				openFile(dp->d_name,buf);
				continue;

				(void) closedir(dirp);
				return;
			}
		} while (dp != NULL);
	}
	else if(S_ISREG(s.st_mode))
	{
		printf("file\n");
		FILE *f;
		f = fopen(fileName, "r");
		readFile(buf, f);
		fclose(f);
	}
	else
	{
		printf("The object is neither a file or directory.\n");
		exit(0);
	}
	
	return;
	
}


/* Program will take the <inverted-index file name> and <directory or file name> as 
 * parameters, in that order.
 */
int main(int argc, char * argv[])
{
	/* There must be two arguments taken as input. 
	if(argc != 3)
	{
		printf("Error: Incorrect number of arguments.\n");
		exit(0);
	}
	*/
	
	
	char buf[BUFFSIZE+1];
	char * fileName = argv[1];
	openFile(fileName, buf);
	
	
	printf("Read in: %s\n", buf);
	makeTokens(fileName, buf);
	
	
	
	return 0;
}