/*  Zachary Allin
 *  RUID:  138-00-0613
 *  CS 214
 *  Rutgers University
 */

/*
 *  othermalloc.c
 */

#include "othermalloc.h"


static char big_block[BIGSIZE];
static char small_block[SMALLSIZE];

static int initialized = 0;
static MemEntry_t * big_root;
static MemEntry_t * small_root;

	

/* FUNCTION DEFINITIONS */

/*  Function:  Malloc
 *  This function is an implementation of malloc.
 */
void * my_malloc(unsigned int size, char * filename, int linenum)
{
	
	MemEntry_t * p;
 	MemEntry_t * succ; 
 	//int incrAmt = 0;

	if(!initialized)
	{
		/* create big root */
		big_root = (MemEntry_t *) big_block;
		big_root->prev = big_root->succ = 0;
		big_root->size = BIGSIZE - sizeof(MemEntry_t);
		big_root->isfree = 1;

		/* create small root */
		small_root = (MemEntry_t *) small_block;
		small_root->prev = small_root->succ = 0;
		small_root->size = SMALLSIZE - sizeof(MemEntry_t);
		small_root->isfree = 1;
		
		initialized = 1;
    }
  	
  	if(size < BIGORSMALL) /* Determine if this is a "small" or "big" chunk */
  		p = small_root;
  	else
  		p = big_root;
  		
  	int triedAlready = 0;
  	
  retry:
  	do
  	{
  		/* If we tried once, try searching in the opposite section. */
  		if(triedAlready)
  		{
  			if(size < BIGORSMALL)
  				p = big_root;
  			else
  				p = small_root;
  		}
  	
  		if(p->size < size) /* current block is too small */
  		{
  			p = p->succ;
  			
  			/* if there isn't any more memory */
  			if(p == 0)
  			{
  				/* first time, switch the section */
  				if(!triedAlready)
  				{
  					triedAlready = 1;
  					//continue;
  					goto retry;
  				}
  				
  				/* This is the second time, produce an error since there isn't enough room */
  				fprintf(stderr, "Error:  There is no more available memory for this size chunk in file %s line %d.\n", filename, linenum);
  				return NULL;
  			}
  		}
  		else if(!p->isfree) /* if already taken */
  		{
  			p = p->succ;
  		}
  		else if( p->size < (size + sizeof(MemEntry_t) ) ) /* can't fit anymore after this */
  		{
  			p->isfree = 0;
  			return (char *) p + sizeof(MemEntry_t);
  		}
  		else /* take this memory, create header of what is left */
  		{
  			succ = (MemEntry_t *) ( (char *)p + sizeof(MemEntry_t) + size );
  			succ->prev = p;
  			succ->succ = p->succ;
  			if(p->succ != 0)
  				p->succ->prev = succ;
  			p->succ = succ;
  			
  			succ->isfree = 1; /* leftovers are available */
  			succ->size = p->size - sizeof(MemEntry_t) - size; /* leftover size */
  			
  			p->size = size;
  			p->isfree = 0;
  			return (char *)p + sizeof(MemEntry_t);
  		}
  		
  	}while(p);
  	
  	return 0;
}



/*  Function:  Free
 *  This function is an implementation of free.  It will search for the pointer and free 
 *  it from memory if it was allocated by the above implementation of Malloc.  At all 
 *  other conditions, it produces an error.  
 */
void my_free(void * p, char * filename, int linenum)
{
	if( (p < (void *)big_block || p > ( (void *)big_block + BIGSIZE) ) && ( p < (void *)small_block || p > ((void *)small_block + SMALLSIZE) ) )
	{
		fprintf(stderr, "Error:  Cannot free pointer that was not allocated in file %s line %d.\n", filename, linenum);
		return;
	}
	
	MemEntry_t * ptr;
	MemEntry_t * prev;
	MemEntry_t * succ;
	
	int isMemEntry = 0;
	
	/* check if p is in big_block */
	ptr = (MemEntry_t *)big_block;
	while(ptr)
	{
		if(p == (void *)ptr + sizeof(MemEntry_t) )
		{
			isMemEntry = 1;
			break;
		}
		
		ptr = ptr->succ;
	}
	
	/* check if p is in small_block */
	ptr = (MemEntry_t *)small_block;
	while(ptr && !isMemEntry)
	{
		if(p == (void *)ptr + sizeof(MemEntry_t) )
		{
			isMemEntry = 1;
			break;
		}
		
		ptr = ptr->succ;
	}
	
	/* had malloc created it?  Will have MemEntry if it was... */
	if(!isMemEntry)
	{
		fprintf(stderr, "Error:  Cannot free a pointer that was not initialized by malloc in file %s line %d.\n", filename, linenum);
		return;
	}
	
	ptr = (MemEntry_t *)( (char *)p - sizeof(MemEntry_t) );
	
	/* is it already free? */	
	if(ptr->isfree)
	{
		fprintf(stderr,"Error:  Cannot free pointer that is free in file %s line %d.\n", filename, linenum);
		return;
	}
	
	/* If the previous chunk is free, combine with current */
	if( (prev = ptr->prev) && prev->isfree)
	{
		prev->size += sizeof(MemEntry_t) + ptr->size;
		ptr->isfree = 1;
		prev->succ = ptr->succ;
		if(ptr->succ)
			ptr->succ->prev = prev;
	}
	else
	{
		ptr->isfree = 1;
		prev = ptr;
	}
	
	/* If the next chunk is free, combine with current */
	if( (succ = ptr->succ) && succ->isfree)
	{
		prev->size += sizeof(MemEntry_t) + succ->size;
		prev->isfree = 1;
		prev->succ = succ->succ;
		if(succ->succ)
			succ->succ->prev = prev;
	}
	
	return;
	
}
