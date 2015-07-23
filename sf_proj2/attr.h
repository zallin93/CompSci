/**********************************************
    Zachary Allin
    RUID: 138-00-0613
    CS 415 Project 2
    Spring 2015
**********************************************/

/*
	Description:  Declaration of types and attribute operations.
 */

#ifndef ATTR_H
#define ATTR_H

/* tokentype */
typedef union {int num; char *str;} tokentype;

/* Type_Expression */
typedef enum type_expression {TYPE_INT=0, TYPE_BOOL, TYPE_ERROR} Type_Expression;

/* regInfo */
typedef struct {
        Type_Expression type;
        int targetRegister;
        } regInfo;

#endif


  
