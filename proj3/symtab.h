/**********************************************
        CS415  Project 2/3
        Spring  2015
        Author: Ulrich Kremer
**********************************************/


#ifndef SYMTAB_H
#define SYMTAB_H

#include <string.h>
#include "attr.h"

/* The symbol table implementation uses a single hash     */
/* function. Starting from the hashed position, entries   */
/* are searched in increasing index order until a         */
/* matching entry is found, or an empty entry is reached. */


typedef struct {
  char *name;
  int offset;
  Type_Expression type;  
  int rowSize; /* only useful for two-dimensional arrays   */
               /* for one-dimensional arrays, rowSize == 0 */
               /* for scalar variables, rowSize == -1 */
} SymTabEntry;

extern void InitSymbolTable();

extern SymTabEntry * lookup(char *name);

extern void insert(char *name, Type_Expression type, int offset, int rowSize);

extern void PrintSymbolTable();


#endif
