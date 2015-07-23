/*  Zachary Allin
 *  RUID: 138-00-0613
 *  CS 214
 *  Rutgers University
 */

/*
 *  mymalloc.c
 */

#include "mymalloc.h"

static char     myblock[5000]; /* used to manage dynamic memory. 
				  Given by problem description */

static Header   base; /* empty list to get started */
static Header * freeptr = NULL;  /* start of free list */


/* m_malloc: general-purpose storage allocator */
void * m_malloc(size_t nbytes)
{
  Header * p;
  Header * prevptr;
  size_t   nunits;  

  nunits = (nbytes + sizeof(Header)-1) / sizeof(Header) + 1; 

  prevptr = freeptr;
  if( prevptr == NULL) /* no free list yet */
    {
      base.s.ptr = freeptr = prevptr = &base;
      base.s.size = 0;
    }

  for(p = prevptr->s.ptr; ; prevptr = p, p = p->s.ptr)
    {
      if(p->s.size >= nunits) /* big enough */
	{
	  if(p->s.size == nunits) /* exactly the right size */
	    prevptr->s.ptr = p->s.ptr;
	  else  /* allocate tail end */
	    {
	      p->s.size -= nunits;
	      p+= p->s.size;
	      p->s.size = nunits;
	    }
	  freeptr = prevptr;
	  return (void *) (p+1);
	}
      if(p == freeptr) /* wrapped around free list */
	{
	  if( (p = morecore(nunits)) == NULL)
	    return NULL;  /* none left */
	}
    }

}


/* Morecore as defined by Karnighan and Ritchie */
#define NALLOC 1024 /* minimum #units to request */

/* morecore:  ask the system for more memory */
static Header * morecore(size_t nu)
{
  char * cp;
  char * sbrk(int);
  Header * up;

  if(nu < NALLOC)
    nu = NALLOC;
  cp = sbrk(nu * sizeof(Header));
  if(cp == (char *) -1)  /* no space at all */
    return NULL;
  up = (Header *) cp;
  up->s.size = nu;
  m_free( (void *)(up + 1) );
  return freeptr;

}


/*  Free as defined by Karnighan and Ritchie
 *  free:  out block ap in free list
 */
void m_free(void * ap)
{
  Header * bp;
  Header * p;

  bp = (Header *)ap - 1;  /* point to block header */  

  for(p = freeptr; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    {
      if(p >= p->s.ptr && (bp > p || bp < p->s.ptr) )
	break;  /* freed block at start or end of arena */
    }

  if(bp + bp->s.size == p->s.ptr)
    {
      bp->s.size += p->s.ptr->s.size;
      bp->s.ptr = p->s.ptr->s.ptr;
    }
  else
    bp->s.ptr = p->s.ptr;
  
  if(p + p->s.size == bp)  /* join to lower nbr */
    {
      p->s.size += bp->s.size;
      p->s.ptr = bp->s.ptr;
    }
  else
    {
      p->s.ptr = bp;
    }
  freeptr = p;

}


int main()
{
  

  return 0; 
}
