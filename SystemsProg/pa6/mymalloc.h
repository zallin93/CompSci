/*  Zachary Allin
 *  RUID: 138-00-0613
 *  CS 214
 *  Rutgers University
 */

/*
 *  mymalloc.h
 */

#include <stdlib.h>

typedef long Align; /* for alignment to long boundary */

union header /* block header */
{
  struct
  {
    union header * ptr;  /* next block if on free-list */
    size_t size;  /* size of this block */
  } s;
  Align x;  /* Force alignment of blocks */
};
typedef union header Header;

/* FUNCTION DECLARATIONS */

void * m_malloc(size_t nbytes);
static Header * morecore(size_t nu);
void m_free(void * ap);
