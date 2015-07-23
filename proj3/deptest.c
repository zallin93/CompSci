/**********************************************
        CS415  Project 3
        Spring  2015
        Student version
**********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "deptest.h" 

/* Global rightHandList */
rightHandList_t * LIST;
leftHandVal_t * leftside;


/*
 * Function to make the lefthand and righthand data structures for dependence testing.
 */
void makeList()
{
    LIST = (rightHandList_t *) malloc(sizeof(rightHandList_t));
    LIST->head = NULL;
    LIST->tail = NULL;

    leftside = (leftHandVal_t *) malloc(sizeof(leftHandVal_t));
    leftside->identifier = NULL;
    leftside->a = NULL;
    leftside->c = NULL;

    return;
}

/*
 * Creates a node object from the arguments passed. 
 */
rightHandNode_t * makeNode(char identifier, int a, int c)
{
    rightHandNode_t * newNode = (rightHandNode_t *) malloc(sizeof(rightHandNode_t));
    newNode->identifier = identifier;
    newNode->a = a;
    newNode->c = c;
    newNode->next = NULL;

    return newNode;
}

/*
 * Adds the passed node to the tail. 
 */
void addToList(rightHandNode_t * node) 
{
    if(LIST->head == NULL)
    {
        LIST->head = node;
        LIST->tail = node;
        return;
    }

    LIST->tail->next = node;
    LIST->tail = node;

    return;
}

/* Return the result of the Zero Induction Variable test. 
 * 0 for failed, 1 for passed 
 */
int zivTest()
{
    
    return 0;
}

/* Return the result of the Single Variable Induction test. 
 * 0 for failed, 1 for passed
 */
int sivTest()
{
    
    return 0;
}

void emitFoundTrueDependenceWithDistance(char *arrayName, int distance) 
{
    sprintf(CommentBuffer, " <<< Found TRUE dependence on variable \"%s\" with distance %d >>>", 
      arrayName, distance);
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n", CommentBuffer);  
}

void emitAssumeTrueDependence(char *arrayName)
{
    sprintf(CommentBuffer, " <<< Assume TRUE dependence on variable \"%s\" with unknown distance >>>", 
      arrayName);
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n", CommentBuffer);  
}

void emitFoundOutputDependence(char *arrayName)
{
    sprintf(CommentBuffer, " <<< Found OUTPUT dependence on variable \"%s\" >>>", 
      arrayName);
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n", CommentBuffer);  
}

void emitAssumeOutputDependence(char *arrayName)
{
    sprintf(CommentBuffer, " <<< Assume OUTPUT dependence on variable \"%s\" >>>", 
      arrayName);
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n", CommentBuffer);  
}

void emitFoundNoDependenciesAndWillVectorize()
{
    sprintf(CommentBuffer, " <<< Found NO dependencies: VECTORIZE >>>");
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n\n", CommentBuffer);  
}

void emitFoundDependenciesAndWillNotVectorize()
{
    sprintf(CommentBuffer, " <<< Found at least one dependence: CANNOT VECTORIZE >>>");
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n\n", CommentBuffer);  
}
