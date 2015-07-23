/**********************************************
        CS415  Project 3
        Spring  2015
        Student version
**********************************************/

#ifndef _DEPTEST_H
#define _DEPTEST_H

struct leftHandVal
{
    char identifier;
    int a;
    int c;
};
typedef struct leftHandVal leftHandVal_t;

struct rightHandNode
{
    char identifier;
    int a;
    int c;
    struct rightHandNode * next;
};
typedef struct rightHandNode rightHandNode_t;

struct rightHandList
{
    rightHandNode_t * head;
    rightHandNode_t * tail;

}; 
typedef struct rightHandList rightHandList_t;


extern FILE *depstats;

extern  char *CommentBuffer;

extern rightHandNode_t * makeNode(char identifier, int a, int c);

extern void addToList(rightHandNode_t * node); 

extern void emitFoundTrueDependenceWithDistance(char *arrayName, int distance);

extern void emitAssumeTrueDependence(char *arrayName);

extern void emitFoundOutputDependence(char *arrayName);

extern void emitAssumeOutputDependence(char *arrayName);

extern void emitFoundNoDependenciesAndWillVectorize();

extern void emitFoundDependenciesAndWillNotVectorize();

#endif


