/**********************************************
        CS415  Project 3
        Spring  2015
        Student Version
**********************************************/

#ifndef ATTR_H
#define ATTR_H

typedef union {int num; char *str;} tokentype;

typedef enum type_expression {TYPE_INT=0, TYPE_BOOL, TYPE_ERROR} Type_Expression;

typedef struct {
        Type_Expression type;
        int targetRegister;
    } regInfo;

typedef struct {  
        int targetLabel;
    } labelInfo;

typedef struct {  
        Type_Expression type;
        int overallSize;
        int rowSize;
    } typeInfo;

typedef struct {
        int trueLabel;
        int falseLabel;
    } condexprInfo;

typedef struct {
        char *inductionVariable;
        int topOfLoopLabel;
        int exitLabel;
    } controlexprInfo;

typedef struct ID_node {
  char *IDstring;
  struct ID_node *next;
    }* IDInfoPtr;


#endif


  
