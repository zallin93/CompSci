/*
 * Zachary Allin
 * RUID: 138-00-0613
 */


/*
 * 
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Fall 2014                                *
 *********************************************
 */

/* -------------------------------------------------

            CFG for tinyL LANGUAGE

     PROGRAM ::= STMTLIST .
     STMTLIST ::= STMT MORESTMTS
     MORESTMTS ::= ; STMTLIST | epsilon
     STMT ::= ASSIGN | SWAP | READ | PRINT
     ASSIGN ::= VARIABLE = EXPR
     SWAP  ::= % VARIABLE VARIABLE
     READ  ::= & VARIABLE
     PRINT ::= # VARIABLE
     EXPR ::= + EXPR EXPR |
              - EXPR EXPR |
              * EXPR EXPR |
              VARIABLE | 
              DIGIT
     VARIABLE ::= a | b | c | d  
     DIGIT ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

     NOTE: tokens are exactly a single character long

     Example expressions:

           +12.
           +1b.
           +*34-78.
           -*+1+2a58.

     Example programs;

         &a;&b;c=+3*ab;d=+c1;#d.
         b=-*+1+2a58;#b.

 ---------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Instr.h"
#include "InstrUtils.h"
#include "Utils.h"

#define MAX_BUFFER_SIZE 500
#define EMPTY_FIELD 0xFFFFF
#define token *buffer

/* GLOBALS */
static char *buffer = NULL;	/* read buffer */
static int regnum = 1;		/* for next free virtual register number */
static FILE *outfile = NULL;	/* output of code generation */

/* Utilities */
static void CodeGen(OpCode opcode, int field1, int field2, int field3);
static inline void next_token();
static inline int next_register();
static inline int is_digit(char c);
static inline int to_digit(char c);
static inline int is_identifier(char c);
static char *read_input(FILE * f);

/* Routines for recursive descending parser LL(1) */
static void program();
static void stmtlist();
static void morestmts();
static void stmt();
static void assign();
static void swap();
static void read();
static void print();
static int expr();
static int variable();
static int digit();

/*************************************************************************/
/* Definitions for recursive descending parser LL(1)                     */
/*************************************************************************/
static int digit()
{
	int reg;

	if (!is_digit(token)) {
		ERROR("Expected digit\n");
		exit(EXIT_FAILURE);
	}
	reg = next_register();
	CodeGen(LOADI, reg, to_digit(token), EMPTY_FIELD);
	next_token();
	return reg;
}

static int variable()
{
	/* YOUR CODE GOES HERE */
	int reg;
	if(!is_identifier(token))
	{
		ERROR("Expected identifier\n");
		exit(EXIT_FAILURE);
	}
	reg = next_register();
	/* may need to be rewritten */
	CodeGen(LOAD, reg, token, EMPTY_FIELD); 
	next_token();
	return reg;
}

static int expr()
{
	int reg, left_reg, right_reg;
	char temp = token;

	switch (token) {
	case '+':
		next_token();
		left_reg = expr();
		right_reg = expr();
		reg = next_register();
		CodeGen(ADD, reg, left_reg, right_reg);
		return reg;
	/* YOUR CODE GOES HERE */
	case '-':
		next_token();
		left_reg = expr();
		right_reg = expr();
		reg = next_register();
		CodeGen(SUB, reg, left_reg, right_reg);
		return reg;
	case '*':
		next_token();
		left_reg = expr();
		right_reg = expr();
		reg = next_register();
		CodeGen(MUL, reg, left_reg, right_reg); 
		return reg;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return digit();
	case 'a':
	case 'b': 
	case 'c': 
	case 'd':
		return variable();
	default:
		ERROR("Symbol %c unknown\n", token);
		exit(EXIT_FAILURE);
	}
}

/* working good... */
static void assign()
{
	/* YOUR CODE GOES HERE */
	char var = token;
	next_token();
	if(token != '=')
	{
		ERROR("Program error in assign.  Current input symbol is %c\n", token);
		exit(EXIT_FAILURE);
	}
	next_token();
	int reg = expr();
	CodeGen(STORE, var, reg, EMPTY_FIELD);
}

/* DONE */
static void swap()
{
	int reg1, reg2;
	char var1 = token;
	reg1 = variable();
	char var2 = token;
	reg2 = variable();
	CodeGen(STORE, var1, reg2, EMPTY_FIELD);
	CodeGen(STORE, var2, reg1, EMPTY_FIELD);

}

/* DONE */
static void read()
{
	CodeGen(READ, token, EMPTY_FIELD, EMPTY_FIELD);
	next_token();
}

/* DONE */
static void print()
{
	/* YOUR CODE GOES HERE */
	CodeGen(WRITE, token, EMPTY_FIELD, EMPTY_FIELD);
	next_token();
}

static void stmt()
{
	/* YOUR CODE GOES HERE */
	switch(token)
	{
		case 'a':
		case 'b':
		case 'c':
		case 'd':
			assign();
			break;
		case '%':
			next_token();
			swap();
			break;
		case '&':
			next_token();
			read();
			break;
		case '#':
			next_token();
			print();
			break;
		case ';':
			next_token();
			return;
		default:
			ERROR("Symbol %c unknown\n", token);
			exit(EXIT_FAILURE);
	}
}

static void morestmts()
{
	/* YOUR CODE GOES HERE */
	if(token == ';')
	{
		next_token();
		stmtlist();
	}
}

static void stmtlist()
{
	/* YOUR CODE GOES HERE */
	stmt();
	morestmts();
}

static void program()
{
	/* YOUR CODE GOES HERE */
	/* the following call to expr() is to get you started */
        /*       this needs to be changed to another call     */
	//expr();
	stmtlist();
	if (token != '.') {
		ERROR("Program error in program.  Current input symbol is %c\n", token);
		exit(EXIT_FAILURE);
	};
}

/*************************************************************************/
/* Utility definitions                                                   */
/*************************************************************************/
static void CodeGen(OpCode opcode, int field1, int field2, int field3)
{
	Instruction instr;

	if (!outfile) {
		ERROR("File error\n");
		exit(EXIT_FAILURE);
	}
	instr.opcode = opcode;
	instr.field1 = field1;
	instr.field2 = field2;
	instr.field3 = field3;
	PrintInstruction(outfile, &instr);
}

static inline void next_token()
{
	printf("%c ", *buffer);
	if (*buffer == ';')
		printf("\n");
	buffer++;
	if (*buffer == '.')
		printf(".\n");
}

static inline int next_register()
{
	return regnum++;
}

static inline int is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return 1;
	return 0;
}

static inline int to_digit(char c)
{
	if (is_digit(c))
		return c - '0';
	WARNING("Non-digit passed to %s, returning zero\n", __func__);
	return 0;
}

static inline int is_identifier(char c)
{
	if (c >= 'a' && c <= 'd')
		return 1;
	return 0;
}

static char *read_input(FILE * f)
{
	int max, i, c;
	char *b;

	max = MAX_BUFFER_SIZE;
	b = (char *)calloc(max, sizeof(char));
	if (!b) {
		ERROR("Calloc failed\n");
		exit(EXIT_FAILURE);
	}

	/* skip leading whitespace */
	for (;;) {
		c = fgetc(f);
		if (EOF == c) {
			break;
		} else if (!isspace(c)) {
			ungetc(c, f);
			break;
		}
	}

	i = 0;
	for (;;) {
		c = fgetc(f);
		if (EOF == c) {
			b[i] = '\0';
			break;
		}
		b[i] = c;
		if (max - 1 == i) {
			max = max + max;
			b = (char *)realloc(buffer, max * sizeof(char));
			if (!b) {
				ERROR("Realloc failed\n");
				exit(EXIT_FAILURE);
			}
		}
		++i;
	}
	return b;
}

/*************************************************************************/
/* Main function                                                         */
/*************************************************************************/

int main(int argc, char *argv[])
{
	const char *outfilename = "tinyL.out";
	char *input;
	FILE *infile;

	printf("------------------------------------------------\n");
	printf("CS314 compiler for tinyL\n      Fall 2014\n");
	printf("------------------------------------------------\n");

	if (argc != 2) {
		fprintf(stderr, "Use of command:\n  compile <tinyL file>\n");
		exit(EXIT_FAILURE);
	}

	infile = fopen(argv[1], "r");
	if (!infile) {
		ERROR("Cannot open input file \"%s\"\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	outfile = fopen(outfilename, "w");
	if (!outfile) {
		ERROR("Cannot open output file \"%s\"\n", outfilename);
		exit(EXIT_FAILURE);
	}

	input = read_input(infile);
	buffer = input;
	program();

	printf("\nCode written to file \"%s\".\n\n", outfilename);

	free(input);
	fclose(infile);
	fclose(outfile);
	return EXIT_SUCCESS;
}
