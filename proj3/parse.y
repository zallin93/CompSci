%{
#include <stdio.h>
#include "attr.h"
#include "instrutil.h"
#include "deptest.h"

int yylex();
void yyerror(char * s);
#include "symtab.h"

FILE *outfile;
FILE *depstats;
char *CommentBuffer;
 
%}

%union {tokentype token;
        regInfo targetReg;
        labelInfo targetLab;
        typeInfo tInfo;
        condexprInfo cexprInfo;
        controlexprInfo ctrlexprInfo;
        IDInfoPtr idInfo;
       }

%token PROG PERIOD VAR 
%token INT BOOL PRINT DO ENDFOR 
%token ARRAY OF 
%token BEG END ASG  
%token AND OR TRUE FALSE
%token <targetLab> FOR
%token <token> ID ICONST 

%type <targetReg> exp 
%type <targetReg> lhs 
%type <ctrlexprInfo> ctrlexp
%type <idInfo> idlist
%type <tInfo> type
%type <tInfo> stype

%start program

%left '+' '-' AND
%left '*' OR

%%
program : {
        emitComment("Assign STATIC_AREA_ADDRESS to register \"r0\"");
        emit(NOLABEL, LOADI, STATIC_AREA_ADDRESS, 0, EMPTY);
    } 
    
    PROG ID ';' block PERIOD { 
    }
;

block   : variables cmpdstmt { 
    }
;

variables: /* empty */
    | VAR vardcls { 
    }
;

vardcls : vardcls vardcl ';' { 
    }

    | vardcl ';' { 
    }
    
    | error ';' { 
        yyerror("***Error: illegal variable declaration\n");
    }  
;

vardcl  : idlist ':' type { 
        IDInfoPtr idinfoPtr = $1;
        if (idinfoPtr != NULL) 
        {
            insert(idinfoPtr->IDstring, $3.type, NextOffset($3.overallSize), $3.rowSize);
            while (idinfoPtr->next != NULL) 
            {
                insert(idinfoPtr->next->IDstring, $3.type, NextOffset($3.overallSize), $3.rowSize);            /* This is in int units, not bytes!. */
                idinfoPtr = idinfoPtr->next;
            }
        }
    }
;

idlist  : idlist ',' ID { 
        char *temp = (char *) malloc (strlen($3.str)+1);
        IDInfoPtr idinfoPtr = NULL;
        IDInfoPtr lastElem = NULL;

        strcpy(temp, $3.str);
        /* insert at end of singly-linked list */
        lastElem = $1;
        while (lastElem->next != NULL)
            lastElem = lastElem->next;
        idinfoPtr = (IDInfoPtr) malloc (sizeof(struct ID_node)); 
        idinfoPtr->IDstring = temp;
        idinfoPtr->next = NULL;
        lastElem->next = idinfoPtr;
        $$ = $1;
    }

    | ID   { 
        char *temp = (char *) malloc (strlen($1.str)+1);
        strcpy(temp, $1.str);
        $$ = (IDInfoPtr) malloc (sizeof(struct ID_node)); 
        $$->IDstring = temp;
        $$->next = NULL;
    }
;


type    : ARRAY '[' ICONST ']' OF stype { 
        $$.overallSize = $3.num;
        $$.type = $6.type;   
        $$.rowSize = 0; 
    }

    | stype { 
        $$.overallSize = $1.overallSize;
        $$.type = $1.type;
        $$.rowSize = $1.rowSize;
    }
;

stype   : INT { 
        $$.type = TYPE_INT; 
        $$.overallSize = 1; 
        $$.rowSize = -1; 
    }

    | BOOL { 
        $$.type = TYPE_BOOL; 
        $$.overallSize = 1; 
        $$.rowSize = -1; 
    }
;

stmtlist : stmtlist ';' stmt { 
    }

    | stmt { 
    }
    
    | error { yyerror("***Error: ';' expected or illegal statement \n");
    }
;

stmt    : fstmt { 
    }

    | astmt { 
    }

    | writestmt { 
    }
;

cmpdstmt: BEG stmtlist END { 
    }
;

writestmt: PRINT '(' exp ')' { 
        int newOffset = NextOffset(1); /* call generates byte address */
        emitComment("Code for \"PRINT\"");
        emit(NOLABEL, STOREAI, $3.targetRegister, 0, newOffset);
        emit(NOLABEL, OUTPUT, STATIC_AREA_ADDRESS + newOffset, EMPTY, EMPTY);
    }
;

fstmt   : FOR  {
        emitComment("Unconditional branch should go here");
        emit(NOLABEL, VECTON, EMPTY, EMPTY, EMPTY);
                        } /* BOGUS */
    ctrlexp DO 
    stmt { 
        SymTabEntry *entry = lookup($3.inductionVariable);
        int newReg1 = NextRegister(); /* address or value of induction variable */ 
        int newReg2 = NextRegister(); /* address or value of induction variable */ 

        if (entry == NULL) 
            printf("\n*** ERROR ***: Induction variable %s not declared.\n", $3.inductionVariable);
        else 
            if ((entry->type != TYPE_INT) || (entry->rowSize != -1))
                printf("\n*** ERROR ***: Induction variable %s not a scalar of type integer.\n", $3.inductionVariable);
            else 
            {
                emitComment("in ELSE... incrementing loop index..\n");
                /* increment induction variable and jump to top of loop */
                emit(NOLABEL, LOADAI, 0, entry->offset, newReg1);
                emit(NOLABEL, ADDI, newReg1, 1, newReg2);
                emit(NOLABEL, STOREAI, newReg2, 0, entry->offset);
                emit(NOLABEL, BR, $3.topOfLoopLabel, EMPTY, EMPTY);
                emit($3.exitLabel, NOP, EMPTY, EMPTY, EMPTY); // associated with 'stmt'
            }
         
                        }
    ENDFOR { 
        // do the dependence testing here... maybe?  

        emitFoundTrueDependenceWithDistance("A", 2);
        emitAssumeTrueDependence("A");
        emitFoundOutputDependence("A");
        emitAssumeOutputDependence("A");
        emitFoundNoDependenciesAndWillVectorize();
        emitFoundDependenciesAndWillNotVectorize();

        emit(NOLABEL, VECTOFF, EMPTY, EMPTY, EMPTY); /* BOGUS */
    }
;

astmt : lhs ASG exp     { 
        if (! ((($1.type == TYPE_INT) && ($3.type == TYPE_INT)) || 
            (($1.type == TYPE_BOOL) && ($3.type == TYPE_BOOL)))) 
        {
            printf("\n*** ERROR ***: Assignment types do not match.\n");
        }
        emit(NOLABEL, STORE, $3.targetRegister, $1.targetRegister, EMPTY);
    }
;

lhs : ID    { 
        SymTabEntry *entry = lookup($1.str);
        int newReg1 = NextRegister(); 
        int newReg2 = NextRegister(); 
        $$.targetRegister = newReg2;

        if (entry == NULL) 
            printf("\n*** ERROR ***: Variable %s not declared.\n", $1.str);
        else 
        {
            if (entry->rowSize != -1) 
                printf("\n*** ERROR ***: Variable %s is not a scalar variable.\n", $1.str);

            $$.type = entry->type;
            sprintf(CommentBuffer, "Load LHS value of variable \"%s\" at offset %d", entry->name, entry->offset);
            emitComment(CommentBuffer);
            emit(NOLABEL, LOADI, entry->offset, newReg1, EMPTY);
            emit(NOLABEL, ADD, 0, newReg1, newReg2);
        }
    }

    |  ID '[' exp ']'   { 
        SymTabEntry *entry = lookup($1.str);
        int newReg = NextRegister();
        int constFourReg = NextRegister();
        int offsetReg = NextRegister();
        /* int exprReg2 = NextRegister(); */
        int baseAddrReg = NextRegister();
        int relativeAddrReg = NextRegister();

        $$.targetRegister = newReg;

        if (entry == NULL) 
            printf("\n*** ERROR ***: Variable %s not declared.\n", $1.str);
        else 
        {
            if (entry->rowSize != 0) 
                printf("\n*** ERROR ***: Variable %s is not an array variable.\n", $1.str);

            if ($3.type != TYPE_INT) 
                printf("\n*** ERROR ***: Array variable %s index type must be integer.\n", $1.str);

            $$.type = entry->type;
            sprintf(CommentBuffer, "Load LHS value of array variable \"%s\" with based address %d", 
                    entry->name, entry->offset);
            emitComment(CommentBuffer);
            emit(NOLABEL, LOADI, 4, constFourReg, EMPTY);
            emit(NOLABEL, MULT, $3.targetRegister, constFourReg, offsetReg);
            /* emit(NOLABEL, SUB, offsetReg, constFourReg, exprReg2);  1-based indexing */
            emit(NOLABEL, LOADI, entry->offset, baseAddrReg, EMPTY);
            emit(NOLABEL, ADD, baseAddrReg, offsetReg, relativeAddrReg);
            emit(NOLABEL, ADD, 0, relativeAddrReg, newReg);
        }
    }
;


exp : exp '+' exp   { 
        int newReg = NextRegister();

        if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) 
        {
            printf("\n*** ERROR ***: Operand type must be integer.\n");
        }
        $$.type = $1.type;

        $$.targetRegister = newReg;
        emitComment("EXP + EXP \n");
        emit(NOLABEL, ADD, $1.targetRegister, $3.targetRegister, newReg);
    }

    | exp '-' exp   { 
        int newReg = NextRegister(); 

        if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) 
        {
            printf("\n*** ERROR ***: Operand type must be integer.\n");
        }
        $$.type = $1.type;

        $$.targetRegister = newReg;
        emit(NOLABEL, SUB, $1.targetRegister, $3.targetRegister, newReg);
    }

    | exp '*' exp   { 
        int newReg = NextRegister(); 

        if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) 
        {
            printf("\n*** ERROR ***: Operand type must be integer.\n");
        }
        $$.type = $1.type;

        $$.targetRegister = newReg;
        emit(NOLABEL, MULT, $1.targetRegister, $3.targetRegister, newReg);
    }

    | exp AND exp   { 
        int newReg = NextRegister();

        if (! (($1.type == TYPE_BOOL) && ($3.type == TYPE_BOOL))) 
        {
            printf("\n*** ERROR ***: Operand type must be boolean.\n");
        }
        $$.type = $1.type;

        $$.targetRegister = newReg;
        emit(NOLABEL, AND_INSTR, $1.targetRegister, $3.targetRegister, newReg);
    }

    | exp OR exp    { 
        int newReg = NextRegister();

        if (! (($1.type == TYPE_BOOL) && ($3.type == TYPE_BOOL))) 
        {
            printf("\n*** ERROR ***: Operand type must be boolean.\n");
        }
        $$.type = $1.type;

        $$.targetRegister = newReg;
        emit(NOLABEL, OR_INSTR, $1.targetRegister, $3.targetRegister, newReg);
    }

    | ID    { 
        SymTabEntry *entry = lookup($1.str);
        int newReg = NextRegister();
        $$.targetRegister = newReg;

        if (entry == NULL) 
            printf("\n*** ERROR ***: Variable %s not declared.\n", $1.str);
        else 
        {
            if (entry->rowSize != -1) 
                printf("\n*** ERROR ***: Variable %s is not a scalar variable.\n", $1.str);

            $$.type = entry->type;
            sprintf(CommentBuffer, "Load RHS value of variable \"%s\" at offset %d", 
                    entry->name, entry->offset);
            emitComment(CommentBuffer);
            emit(NOLABEL, LOADAI, 0, entry->offset, newReg);
        }
    }

    | ID '[' exp ']'    { 
        SymTabEntry *entry = lookup($1.str);
        int newReg = NextRegister();
        int constFourReg = NextRegister();
        int offsetReg = NextRegister();
        /* int exprReg2 = NextRegister();*/
        int baseAddrReg = NextRegister();
        int relativeAddrReg = NextRegister();

        $$.targetRegister = newReg;

        if (entry == NULL) 
            printf("\n*** ERROR ***: Variable %s not declared.\n", $1.str);
        else 
        {
            if (entry->rowSize != 0) 
                printf("\n*** ERROR ***: Variable %s is not an array variable.\n", $1.str);

            if ($3.type != TYPE_INT) 
                printf("\n*** ERROR ***: Array variable %s index type must be integer.\n", $1.str);

            $$.type = entry->type;
            sprintf(CommentBuffer, "Load RHS value of array variable \"%s\" with based address %d", 
                    entry->name, entry->offset);
            emitComment(CommentBuffer);

            emit(NOLABEL, LOADI, 4, constFourReg, EMPTY);
            emit(NOLABEL, MULT, $3.targetRegister, constFourReg, offsetReg);
            emit(NOLABEL, LOADI, entry->offset, baseAddrReg, EMPTY);
            emit(NOLABEL, ADD, baseAddrReg, offsetReg, relativeAddrReg);
            emit(NOLABEL, LOADAO, 0, relativeAddrReg, newReg);
        }
    }

    | ICONST    { 
        int newReg = NextRegister();
        $$.targetRegister = newReg;
        $$.type = TYPE_INT;
        emit(NOLABEL, LOADI, $1.num, newReg, EMPTY); 
    }

    | TRUE  { 
        int newReg = NextRegister(); /* TRUE is encoded as value '1' */
        $$.targetRegister = newReg;
        $$.type = TYPE_BOOL;
        emit(NOLABEL, LOADI, 1, newReg, EMPTY); 
    }

    | FALSE     { 
        int newReg = NextRegister(); /* TRUE is encoded as value '0' */
        $$.targetRegister = newReg;
        $$.type = TYPE_BOOL;
        emit(NOLABEL, LOADI, 0, newReg, EMPTY); 
    }

    | error     { 
        yyerror("***Error: illegal expression\n");
    }
;


ctrlexp : ID ASG ICONST ',' ICONST  { 
        int newReg1 = NextRegister(); 
        int newReg2 = NextRegister(); 
        int newReg3 = NextRegister(); 
        int condReg = NextRegister();/* contains 0 or 1 truth value for test expression */
        int lowerBoundReg = NextRegister(); /* value of lower bound */ 
        int upperBoundReg = NextRegister(); /* value of upper bound */
        int topOfLoopBranch = NextLabel(); 
        int trueBranch = NextLabel(); 
        int falseBranch = NextLabel();
        SymTabEntry *entry = lookup($1.str);
        char *temp = (char *) malloc (strlen($1.str)+1);

        strcpy(temp, $1.str);
        $$.inductionVariable = $1.str;
        $$.topOfLoopLabel = topOfLoopBranch; 
        $$.exitLabel = falseBranch; 

        if (entry == NULL) 
            printf("\n*** ERROR ***: Induction variable %s not declared.\n", $1.str);
        else 
        {
            sprintf(CommentBuffer, "Initialize ind. variable \"%s\" at offset %d with lower bound value %d", 
                entry->name, entry->offset, $3.num);
            emitComment(CommentBuffer);
            emit(NOLABEL, LOADI, entry->offset, newReg1, EMPTY);
            emit(NOLABEL, ADD, 0, newReg1, newReg2); /* newReg contains now address of ID */ 
            emit(NOLABEL, LOADI, $3.num, lowerBoundReg, EMPTY);
            emit(NOLABEL, LOADI, $5.num, upperBoundReg, EMPTY);
            emit(NOLABEL, STORE, lowerBoundReg, newReg2, EMPTY); 
            emitComment("Generate control code for \"FOR\" ");
            emit(topOfLoopBranch, LOADAI, 0, entry->offset, newReg3);
            emit(NOLABEL, CMPLE, newReg3, upperBoundReg, condReg);
            emit(NOLABEL, CBR, condReg, trueBranch, falseBranch);
            emitComment("below is TRUE branch label - zda3 \n");
            emit(trueBranch, NOP, EMPTY, EMPTY, EMPTY); 
        }
    }
;

%%

void yyerror(char* s) 
{
        fprintf(stderr,"%s\n",s);
}

int optimize_flag = 0;


int main(int argc, char* argv[]) 
{
    printf("\n     CS415 Spring 2015 Vectorizing Compiler\n\n");

    outfile = fopen("iloc.out", "w");

    if (outfile == NULL) 
    { 
        printf("ERROR: cannot open output file \"iloc.out\".\n");
        return -1;
    }

    depstats = fopen("stats.log", "w");
    if (depstats == NULL) 
    { 
        printf("ERROR: cannot open output file \"stats.log\".\n");
        return -1;
    }

    CommentBuffer = (char *) malloc(500);  
    InitSymbolTable();

    printf("1\t");
    yyparse();
    printf("\n");

    PrintSymbolTable();
  
    fclose(outfile);
    fclose(depstats);
  
    return 1;
}
