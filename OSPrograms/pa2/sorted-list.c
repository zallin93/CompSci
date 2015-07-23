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

#include <stdlib.h>
#include "sorted-list.h"

// for iteration
int changes = 0;

/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator and destroyer function that can be used to order objects that will be
 * kept in the list.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object.
 * Else, it returns NULL.
 *
 */

SortedListPtr SLCreate(CompareFuncT cf,DestructFuncT df)
{
	// make sure that cf and df have been passed before creating
	if(cf && df)
	{
      SortedListPtr sl = malloc(sizeof(SortedListPtr));
      //sl->cf = malloc(sizeof(cf));	no effect
      //sl->df = malloc(sizeof(df));	no effect
      sl->cf = cf;
      sl->df = df;
      sl->start = malloc(sizeof(Node));
      sl->start = NULL;
      //sl->start->next = NULL;
      return sl;
	}

  return NULL;

}

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 */
void SLDestroy(SortedListPtr list)
{
  	Node curr = list->start;

    while(curr)
    {
		Node ptr = curr;
		curr = curr->next;
		(*list).df(ptr->obj);
		free(ptr);
    }

    // free the pointers
    free(curr);
    free(list);

    return;  
}


/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLInsert(SortedListPtr list, void *newObj)
{
	//printf("%d\n", (int) newObj);

	/* If either parameters do not exist, return an error, as it was not specified
		in the project requirements.
	*/  
	if(!list || !newObj)
		return 0;

	// parameters exist, continue on.

	Node ptr1 = malloc(sizeof(Node));
	Node ptr2 = malloc(sizeof(Node));
	
	// New node in the list for the new item.  It will always be added to the list.
	Node put = malloc(sizeof(Node));
	put->obj = malloc(sizeof(newObj));
	put->next = malloc(sizeof(Node));
	put->obj = newObj;
	
	ptr1 = list->start;
	
	// if the first element in the list is null, set the start to the new object
	if(!ptr1)
	{
		list->start = put;
		put->next=NULL;
	}

	// Step through the list 
	while(ptr1)
	{
	  if((*list).cf(newObj, ptr1->obj) > 0)
		{
			//printf("inside %d and %d",newObj,ptr1->obj);
			// The new node will always go before the first pointer.
			put->next = ptr1;
			
					
			//trying this	
			if(&(list->start->obj) == &(ptr1->obj))
				list->start = put;
			else
				ptr2->next = put;
			
			//return 1;
			return (int)put->obj;
		}

	  ptr2 = ptr1;
	  ptr1 = ptr1->next;
	}
  
  	// New Object is the smallest in the list, added at the end.
  	ptr2->next = put;
  	put->next = NULL;
  	
  	//return 1;
  	return (int)put->obj;
}


/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 */

int SLRemove(SortedListPtr list, void *newObj)
{
	// Create two pointers to traverse the list, searching for the newObj.
	Node ptr1 = malloc(sizeof(Node));
	Node ptr2 = malloc(sizeof(Node));
	
	ptr1 = list->start;
	
	while(ptr1)
	{
		// if found
		if((*list).cf(newObj, ptr1->obj) == 0)
		{		
			
			// newObj was found at the start of the list.
			if(!ptr2)
				list->start = ptr1->next;	
				
			// newObj was found elsewhere in the list.
			else
				ptr2->next = ptr1->next;
			
			(*list).df(ptr1->obj);
			free(ptr1);
			
			// tell iterator a change was made to the list
			changes++;
			
			return 1;
		}
		
		ptr2 = ptr1;
	  	ptr1 = ptr1->next;
		
	}
	
	// newObj was not found in the list.  Return 0 indicating a failure.
	return 0;
}


/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL pointer to a
 * SortedListIterT object.  Otherwise, it returns NULL.  The SortedListT
 * object should point to the first item in the sorted list, if the sorted
 * list is not empty.  If the sorted list object is empty, then the iterator
 * should have a null pointer.
 *
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list)
{
	//printf("here\n");
	fflush(0);
	// Check for NULL input
	if(!list)
		return NULL;
	
	// Create and initialize iterator object.
	SortedListIteratorPtr iter = malloc(sizeof(SortedListIteratorPtr));
	iter->list = malloc(sizeof(SortedListPtr));
	iter->list = list;
	iter->ptr = malloc(sizeof(Node));
	iter->ptr = list->start;
	iter->count = changes;
	iter->lastObj = NULL;
	
	return iter;
}


/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affect the original list used to create
 * the iterator in any way.
 *
 */

void SLDestroyIterator(SortedListIteratorPtr iter)
{
	free(iter);
	return;

}


/*
 * SLNextItem returns the next object in the list encapsulated by the
 * given iterator.  It should return a NULL when the end of the list
 * has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 */

void *SLNextItem(SortedListIteratorPtr iter)
{
/*
	In the case that we have removed an item from the list, we want 
*/
	if(changes != iter->count)
	{
		iter->ptr = iter->list->start;
		
		// If iter wasn't at the start of the list.
		if(iter->lastObj)
		{
			iter->ptr = iter->list->start;
			while(iter->list->cf(iter->lastObj,iter->ptr->obj) <= 0)
			{
				iter->ptr = iter->ptr->next;
			}
		}
		changes = 0;
		iter->count = 0;
	}
	else
	{
		if(iter->ptr == NULL)
			return NULL;
		
		iter->lastObj = iter->ptr->obj;
		iter->ptr = iter->ptr->next;
		
		return iter->lastObj;
	}
	
	return iter->ptr->obj;
	
}

/* 
 * Main function needed to properly compile.

 void main()
 {
 	return;
 }  */