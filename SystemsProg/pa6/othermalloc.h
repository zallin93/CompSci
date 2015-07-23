/*  Zachary Allin
 *  RUID:  138-00-0613
 *  CS 214
 *  Rutgers University
 */

/*
 *  othermalloc.c
 */

#ifndef OTHER_MALLOC_H
#define OTHER_MALLOC_H

#include <stdio.h>
//#include <unistd.h>

#define TOTALSIZE 5000
#define THRESHOLD .05
#define BIGORSMALL TOTALSIZE*THRESHOLD
#define SMALLSIZE TOTALSIZE/5*2
#define BIGSIZE TOTALSIZE-SMALLSIZE

struct MemEntry
{
  	unsigned int   	  size;
  	unsigned char     isfree;
  	struct MemEntry * prev;
  	struct MemEntry * succ;
};
typedef struct MemEntry MemEntry_t;

/* FUNCTION PROTOTYPES */
void * my_malloc(unsigned int size, char * filename, int linenum);
void   my_free(void * p, char * filename, int linenum);

#endif