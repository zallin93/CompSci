/**********************************************
    Zachary Allin
    RUID: 138-00-0613
    CS 415 Project 2
    Spring 2015
**********************************************/

/*
    Description:  Additional information needed in symbol table and symbol table operations.
 */

#ifndef SYMTAB_H
#define SYMTAB_H

#include <string.h>
#include "attr.h"

/* The symbol table implementation uses a single hash     */
/* function. Starting from the hashed position, entries   */
/* are searched in increasing index order until a         */
/* matching entry is found, or an empty entry is reached. */


typedef struct { /* need to augment this */
  char *name;
  int offset;
  Type_Expression type;  
} SymTabEntry;

extern void InitSymbolTable();

extern SymTabEntry * lookup(char *name);

extern void insert(char *name, Type_Expression type, int offset);

extern void PrintSymbolTable();

#endif
