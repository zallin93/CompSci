/*
 * Zachary Allin
 * RUID: 138-00-0613
 */

/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Fall 2014                                *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

int main()
{
	Instruction *head;
	Instruction *instr1, *instr2, *instr3;
	int opt_flag, opt_calc;


	head = ReadInstructionList(stdin);

        /* ---  FIRST: ALGEBRAIC SIMPLIFICATION PASS --- */

	if (!head) {
		ERROR("No instructions\n");
		exit(EXIT_FAILURE);
	}

	/* YOUR CODE GOES HERE */
	//instr1 = head;

	for(instr1 = head; instr1 != NULL; instr1 = instr1->next)
	{

		if(instr1->next)
			instr2 = instr1->next;
		else
			break;
		if(instr2->next)
			instr3 = instr2->next;
		else
			break;

		OpCode code1 = instr1->opcode;
		OpCode code2 = instr2->opcode;
		OpCode code3 = instr3->opcode;


		if(instr1->field3 != NULL || instr2->field3 != NULL || (code3 != ADD && code3 != SUB && code3 !=MUL) )
		{
			//break;
			continue;
		}

		/* add conditions to shorten */
		if(code3 == ADD)
		{
			/* (a + 0) */
			if( code1 == LOAD && code2 == LOADI && instr2->field2 == 0 && instr1->field1 == instr3->field2 && instr2->field1 == instr3->field3)
			{

				instr1->field1 = instr3->field1;
				instr1->next = instr3->next;
				if(instr3->next)
					instr3->next->prev = instr1;

				free(instr2);
				free(instr3);

				/* move onto next window */
				continue;
			}

			/* (0 + a) */
			if( code1 == LOADI && code2 == LOAD && instr1->field2 == 0 && instr1->field1 == instr3->field2 && instr2->field1 == instr3->field3)
			{

				instr2->field1 = instr3->field1;
				instr2->next = instr3->next;
				instr2->prev = instr1->prev;
				if(instr1->prev)
					instr1->prev->next = instr2;

				free(instr1);
				free(instr3);
				instr1 = instr2;

				/*move onto next window */
				continue;
			}
		}

		/* sub conditions to shorten */
		if(code3 == SUB)
		{
			/* (a - 0) */
			if( code1 == LOAD && instr1->field1 == instr3->field2 && code2 == LOADI &&  instr2->field2 == 0 && instr2->field1 == instr3->field3)
			{
				Instruction *newInstr = (Instruction *) malloc(sizeof(Instruction));
				newInstr->opcode = LOAD;
				newInstr->field1 = instr3->field1;
				newInstr->field2 = instr1->field2;
				newInstr->field3 = instr1->field3;
				newInstr->prev = instr1->prev;
				newInstr->next = instr3->next;

				instr1->prev->next = newInstr;
				instr3->next->prev = newInstr;

				free(instr1);
				free(instr2);
				free(instr3);

				/* move onto the next window */
				continue;
			}

			/* (a - a) */
			if( code1 == code2 && code1 == LOAD && instr1->field2 == instr2->field2)
			{
				Instruction *newInstr = (Instruction *) malloc(sizeof(Instruction));
				newInstr->opcode = LOADI;
				newInstr->field1 = instr3->field1;
				newInstr->field2 = 0;
				newInstr->field3 = instr1->field3;
				newInstr->prev = instr1->prev;
				newInstr->next = instr3->next;

				instr1->prev->next = newInstr;
				instr3->next->prev = newInstr;
				
				free(instr1);
				free(instr2);
				free(instr3);

				/* move onto the next window */
				continue;
			}
		}

		/* mul conditions to shorten */
		if(code3 == MUL)
		{
			/* (a * 0) */
			if( code1 == LOAD && code2 == LOADI && instr2->field2 == 0 && instr1->field1 == instr3->field2 && instr2->field1 == instr3->field3)
			{
				instr2->field1 = instr3->field1;
				instr2->next = instr3->next;
				instr2->prev = instr1->prev;

				if(instr1->prev)
					instr1->prev->next = instr2;
				if(instr3->next)
					instr3->next->prev = instr2;

				free(instr1);
				free(instr3);

				instr1 = instr2;
				continue;
			}

			/* (0 * a) */
			if( code1 == LOADI && code2 == LOAD && instr1->field2 == 0 && instr1->field1 == instr3->field2 && instr2->field1 == instr3->field3)
			{
				instr1->field1 = instr3->field1;
				instr1->next = instr3->next;

				if(instr3->next)
					instr3->next->prev = instr1;

				free(instr2);
				free(instr3);

				continue;
			}

			/* (a * 1) */
			if( code1 == LOAD && code2 == LOADI && instr2->field2 == 1 && instr1->field1 == instr3->field2 && instr2->field1 == instr3->field3)
			{
				instr1->field1 = instr3->field1;
				instr1->next = instr3->next;

				if(instr3->next)
					instr3->next->prev = instr1;

				free(instr2);
				free(instr3);

				continue;
			}

			/* (1 * a) */
			if( code1 == LOADI && code2 == LOAD && instr1->field2 == 1 && instr1->field1 == instr3->field2 && instr2->field1 == instr3->field3)
			{
				instr2->field1 = instr3->field1;
				instr2->prev = instr1->prev;
				instr2->next = instr3->next;

				if(instr1->prev)
					instr1->prev->next = instr2;
				if(instr3->next)
					instr3->next->prev = instr2;

				free(instr1);
				free(instr3);

				instr1 = instr2;
				continue;
			}
		}
	}




        /* --- SECOND: CONSTANT FOLDING PASS --- */

	if (!head) {
		ERROR("No instructions\n");
		exit(EXIT_FAILURE);
	}

	/* YOUR CODE GOES HERE */
	for(instr1 = head; instr1 != NULL; instr1 = instr1->next)
	{
		if(instr1->next)
			instr2 = instr1->next;
		else
			break;
		if(instr2->next)
			instr3 = instr2->next;
		else
			break;

		/* both opcodes have to be LOADI */
		if(instr1->opcode != LOADI || instr2->opcode != LOADI)
		{
			continue;
		}

		switch(instr3->opcode)
		{
			case ADD:
				instr1->field2 = instr1->field2 + instr2->field2;
				instr1->field1 = instr3->field1;

				break;
			case SUB: 
				instr1->field2 = instr1->field2 - instr2->field2;
				instr1->field1 = instr3->field1;

				break;
			case MUL:
				instr1->field2 = instr1->field2 * instr2->field2;
				instr1->field1 = instr3->field1;
				
				break;
			default:
				continue;
				break;
		}

		instr1->next = instr3->next;
		if(instr3->next)
			instr3->next->prev = instr1;

		free(instr2);
		free(instr3);
	}


	PrintInstructionList(stdout, head);
	DestroyInstructionList(head);
	return EXIT_SUCCESS;
}
