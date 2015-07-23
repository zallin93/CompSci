/*
 * Zachary Allin
 * RUID: 138000613
 * 
 */

/*
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// functional ops
#define NOP     "nop"
#define ADD     "add"
#define SUB     "sub"
#define MULT    "mult"
#define DIV     "div"
#define ADDI    "addI"
#define SUBI    "subI"
#define MULTI   "multI"
#define DIVI    "divI"
#define LSHIFT  "lshift"
#define LSHIFTI "lshiftI"
#define RSHIFT  "rshift"
#define RSHIFTI "rshiftI"
#define AND     "and"
#define OR      "or"
#define XOR     "xor"

// memory ops
#define LOADI   "loadI"
#define LOAD    "load"
#define STORE   "store"

// output op
#define OUTPUT "output"

// constant definitions
#define BUFF_SIZE 100
*/

#include "allocator.h"

/*
 * keep track of the range for each register that is used
 */
/*struct live_ranges
{
    int regNum; // the register number we are looking at (names aren't defined)
    int count;
    int startingPos; // live range begins here
    int endingPos;  // live range ends here
};
typedef struct live_ranges live_ranges_t;
*/

/* ****************
 * GLOBAL VARIABLES
 * ****************/

char algorithm;
char* buffer;
int k;
FILE* input_file;

live_ranges_t* liveRangeArray[256]; // worst case, all 256 registers used

/* ********************
 * FUNCTION DEFINITIONS
 * ********************/ 

/*
 * Function to update the live range information for the register that is 
 * passed as an argument, "registerNumber".
 * Target = 0, register was destination. 
 */
void editLiveRange(int registerNumber, int lineNumber, int target)
{
    liveRangeArray[registerNumber]->regNum = registerNumber; // may not need
    liveRangeArray[registerNumber]->count++;
    if(liveRangeArray[registerNumber]->startingPos == 0)
        liveRangeArray[registerNumber]->startingPos = lineNumber;

    // will have to modify for correct ending of live range
    if(liveRangeArray[registerNumber]->endingPos < lineNumber)
    {
        liveRangeArray[registerNumber]->endingPos = lineNumber-target; 
    }

    return;
}

/*
 *  Test function used to print out the live ranges of all registers used.
 */
void printLiveRanges()
{
    int i;
    for(i=0; i < 256; i++)
    {
        if(liveRangeArray[i]->regNum > -1)
        {
            printf("The live range of r%d is %d to %d...", liveRangeArray[i]->regNum, liveRangeArray[i]->startingPos, liveRangeArray[i]->endingPos);
            printf("It occurs %d times...\n", liveRangeArray[i]->count);
        }
        
    }

    return;    
}

/*
 *  Function needed to determine the MAX_LIVE of the basic block.
 *  argument is the total number lines of ILOC code in the program
 *  Return the value of MAX_LIVE
 */
int calculateMaxLife(int totalLines)
{
    int maxLive = 0;
    int currLive = 0; // maxlive for this lineNum
    int lineNum = 1;
    int i;
    for(lineNum=1; lineNum < totalLines; lineNum++)
    {
        for(i=0; i < 256; i++)
        {
            if(liveRangeArray[i]->regNum > -1) // the register was used
            {
                if(liveRangeArray[i]->startingPos >= lineNum && liveRangeArray[i]->endingPos <= lineNum)
                {
                    currLive++;
                }
            }
        }
        
        if(currLive > maxLive)
            maxLive = currLive;

        currLive = 0;
    }
    return maxLive;
}


/*
 *  Adjust maxReg array to make room for a val that is being added. 
 */
void adjustMaxRegs(int* maxRegs, int start, int length)
{
    int i;
    for(i = length-1; i > start; i--)
    {
        if(*(maxRegs+i) > 0) // there is a reg in this spot of the array
        {
            *(maxRegs+i) = *(maxRegs+(i-1));
        }
        //printf("GOT HERE\n");
    }

    return;
}


// needs to be rewritten
/*
 *  Function needed to determine which registers are used the most.
 *  maxRegs is an int[] that will contain the 'length' number of regs with 
 *  the highest occurrences.
 *  Length = size of maxRegs
 */
void calcMostOccurrences(int* maxRegs, int length)
{
    int i;
    int j;
    for(i=0; i < 256; i++)
    {
        if(liveRangeArray[i]->regNum > -1) // if the reg is used. r0 doesn't count
        {
            for(j=0; j < length; j++)
            {
                // a value was previously entered in this position of maxRegs
                if( *(maxRegs+j) > -1) 
                {
                        //printf("maxRegsj is %d...\n", *(maxRegs+j));
                    if(liveRangeArray[i]->count > liveRangeArray[ *(maxRegs+j)]->count)
                    {
                        adjustMaxRegs(maxRegs, j, length); // shift everything   
                        *(maxRegs+j) =liveRangeArray[i]->regNum;
                        //printf("if, reg val is now %d...\n", liveRangeArray[i]->regNum);
                        break;
                    }
                }
                else
                {
                    *(maxRegs+j) = liveRangeArray[i]->regNum;
                    //printf("reg val is now %d..\n", liveRangeArray[i]->regNum);
                    //printf("i is %d...\n", i);
                    //printf("*maxRegs is %d.\n", *(maxRegs+j));
                    break;
                }
            }
        }
    }
    
}

/*
 * Function sets 'buffer' to the next line from 'input_file'. 
 */
int readLine()
{
    // zero out buffer
    memset(buffer, '\0', BUFF_SIZE);
    
    // read file, dump into buffer
    //fgets(buffer, BUFF_SIZE-1, input_file);    

    //if(!input_file) // end of file reached
    if( !fgets(buffer, BUFF_SIZE-1, input_file) )
    {
        //printf("end of file reached!\n"); //debug
        return 0;
    }

    // ignore lines that start with comments
    if(buffer)
    {
        if(buffer[0] == buffer[1] && buffer[0] == '/')
        {
            readLine();
        }
        else if(buffer[0] == '\n')
        {
            readLine();
        }
    }
    
    // removes the newline from the end of the buffer if it is there
    char *pos;
    if((pos=strchr(buffer, '\n')) != NULL)
    {
        *pos = '\0';
    }

    return 1;
}

/*
 *  Function gets the opcode from the current BUFFER string.
 */
char * getOpCode()
{
    char * stringy = malloc(8*sizeof(char));
    sscanf(buffer, "%s", stringy);
    return stringy;
}

/*
 * Function parses the LOADI instruction and extracts the register values
 * FLAG=1, edit live ranges
 * FLAG=0, comparing for code gen
 */
void parseLoadI(int lineNumber)
{
    char * stringy = malloc(50*sizeof(char));
    //printf(buffer);
    int location = 0;
    int regNum = 0;
    //sscanf(buffer, "%*s %d%*[=>]%*2c%*[r]%*c%d", location, regNum );
    
    stringy = strtok(buffer, " \t");
    location = atoi( strtok(NULL, " \t"));
    strtok(NULL, "r"); // consume the equal sign
    regNum = atoi(strtok(NULL, " \n"));
//    printf("LoadI op.  the location is %d, the register is %d\n", location, regNum); // debug

    editLiveRange(regNum, lineNumber, 0);
    return;
}

/*
 * FLAG=1, edit live ranges
 * FLAG=0, comparing for code gen
 */
void parseLoad(int lineNumber)
{
    int reg1 = 0;
    int reg2 = 0;

    strtok(buffer, "r");
    reg1 = atoi( strtok(NULL, " \t") );
    strtok(NULL, "r");
    reg2 = atoi( strtok(NULL, " \n") );

  //  printf("Load op.  reg1 is %d, reg2 is %d. \n", reg1, reg2); // debug

    editLiveRange(reg1, lineNumber, 1);
    editLiveRange(reg2, lineNumber, 0);
    return;
}

/*
 * FLAG=1, edit live ranges
 * FLAG=0, comparing for code gen
 */
void parseStore(int lineNumber)
{
   int reg1 = 0;
   int reg2 = 0;
   
   printf("at the start of store, buffer is:\n");
   printf(buffer);
   printf("\n");
    
   strtok(buffer, " \t");
   char* reggy = malloc(10*sizeof(char));
   reggy = strtok(NULL, " \t");
   sscanf(reggy, "%*c%d", &reg1);
      
   strtok(NULL, "r");
   reg2 = atoi( strtok(NULL, " \n") );

   //printf("Store op.  reg1 is %d, reg2 is %d. \n", reg1, reg2); // debug
   
    editLiveRange(reg1, lineNumber, 1);
    editLiveRange(reg2, lineNumber, 0);

    return;
}

/*
 * Function parses ADD, SUB, MULT, DIV, AND, OR, XOR
 * FLAG=1, edit live ranges
 * FLAG=0, comparing for code gen
 */
void parseThreeReg(int lineNumber)
{
    int reg1 = 0;
    int reg2 = 0;
    int reg3 = 0;

    strtok(buffer, "r");
    reg1 = atoi( strtok(NULL, ",") );
    //char* buf = malloc(5*sizeof(char));
    //buf = strtok(NULL, "r");
    //sscanf(buf, "%*c%d", &reg2 );
    strtok(NULL, "r");
    reg2 = atoi( strtok(NULL, " \t") );
    strtok(NULL, "r");
    reg3 = atoi( strtok(NULL, " \n") );

    //printf("reg1 is %d, reg2 is %d, reg3 is %d. \n", reg1, reg2, reg3); // debug
    
    editLiveRange(reg1, lineNumber, 1);
    editLiveRange(reg2, lineNumber, 1);

    return;
}

/*
 * Function parses ADDI, SUBI, MULTI, DIVI, LSHIFTI, RSHIFTI
 */
void parseImmediate(int lineNumber)
{
    int source = 0;
    int constant = 0;
    int dest = 0;

    strtok(buffer, "r");
    source = atoi( strtok(NULL, ",") );
    constant = atoi( strtok(NULL, " \t") );
    strtok(NULL, "r");
    dest = atoi( strtok(NULL, " \n") );

    //printf("immediate op.  src is %d, constant is %d, dest is %d.\n", source, constant, dest); // debug

    editLiveRange(source, lineNumber, 1);
    editLiveRange(dest, lineNumber, 0);

    return;
}

int checkMaxRegs()
{
    int i;
   // for(i=0; i < ; )
    return 0;
}

void readLoadI(int* maxRegs, int size)
{
    char * stringy = malloc(50*sizeof(char));
    //printf(buffer);
    int location = 0;
    int regNum = 0;
    //sscanf(buffer, "%*s %d%*[=>]%*2c%*[r]%*c%d", location, regNum );
    
    stringy = strtok(buffer, " \t");
    location = atoi( strtok(NULL, " \t"));
    strtok(NULL, "r"); // consume the equal sign
    regNum = atoi(strtok(NULL, " \n"));
//    printf("LoadI op.  the location is %d, the register is %d\n", location, regNum); // debug

    
    //liveRangeArray[regNum]

    return;
}

void readLoad(int* maxRegs, int size)
{

}

void readStore(int* maxRegs, int size)
{

}

void readThreeReg(int* maxRegs, int size)
{

}

void readImmediate(int* maxRegs, int size)
{

}


void genLoadI(int loc, int target)
{   
    printf("loadI %d => r%d\n", loc, target);
}

void genLoad(int source, int target)
{
    printf("load r%d => r%d\n", source, target);
}

void genStore(int source, int target)
{
    printf("store r%d => r%d\n", source, target);
}

void genThreeReg(char* opcode, int reg1, int reg2, int reg3)
{
    printf("%s r%d, r%d => r%d", opcode, reg1, reg2, reg3);
}

void genImmediate(char*opcode, int reg1, int reg2, int immediate)
{
    printf("%s r%d, %d => r%d", opcode, reg1, immediate, reg2);
}


/*
 * Function to implement the Simple Top-Down allocator.
 * argument k is the total number of physical registers.
 */
void simpleTopDown(int k)
{
    int lineNumber = 0;

    while( readLine() )
    {
        //printf(buffer);
        //printf("\n");

        lineNumber++; // just added
        char* opcode = getOpCode();
     
        // decide how to parse the line depending on the opcode
        if(!strcmp(opcode, "loadI"))
            parseLoadI(lineNumber);
        else if(!strcmp(opcode, "load"))
            parseLoad(lineNumber);
        else if(!strcmp(opcode, "store"))
            parseStore(lineNumber);
        else if(!strcmp(opcode, "add") || !strcmp(opcode, "sub") || 
            !strcmp(opcode, "mult") || !strcmp(opcode, "div") || 
            !strcmp(opcode, "lshift") || !strcmp(opcode, "rshift") || 
            !strcmp(opcode, "and") || !strcmp(opcode, "or") || 
            !strcmp(opcode, "xor") )
            parseThreeReg(lineNumber);
        else if(!strcmp(opcode, "addI") || !strcmp(opcode, "subI") || 
            !strcmp(opcode, "multI") || !strcmp(opcode, "divI") || 
            !strcmp(opcode, "lshiftI") || !strcmp(opcode, "rshiftI") )
            parseImmediate(lineNumber);
    }

    rewind(input_file);

    int maxRegs[k-3];
    memset(maxRegs, -1, (k-3)*sizeof(int));
    calcMostOccurrences(&maxRegs[0], (k-3));

    int i;
    for(i=0; i < (k-3); i++)
    {
        if(maxRegs[i] > -1)
            printf("The reg in maxRegs[%d] is %d...\n", i, maxRegs[i]);
    }

    lineNumber=0;
    
    printf("loadI 1020 => r0\n");
    
    // read through the program again, producing new code when needed
    while( readLine() )
    {
        lineNumber++; // just added
        char* opcode = getOpCode();
     
        // decide how to parse the line depending on the opcode
        if(!strcmp(opcode, "loadI"))
            readLoadI( &maxRegs[0], (k-3) );
        else if(!strcmp(opcode, "load"))
            readLoad( &maxRegs[0], (k-3) );
        else if(!strcmp(opcode, "store"))
            readStore( &maxRegs[0], (k-3) );
        else if(!strcmp(opcode, "add") || !strcmp(opcode, "sub") || 
            !strcmp(opcode, "mult") || !strcmp(opcode, "div") || 
            !strcmp(opcode, "lshift") || !strcmp(opcode, "rshift") || 
            !strcmp(opcode, "and") || !strcmp(opcode, "or") || 
            !strcmp(opcode, "xor") )
            readThreeReg( &maxRegs[0], (k-3) );
        else if(!strcmp(opcode, "addI") || !strcmp(opcode, "subI") || 
            !strcmp(opcode, "multI") || !strcmp(opcode, "divI") || 
            !strcmp(opcode, "lshiftI") || !strcmp(opcode, "rshiftI") )
            readImmediate( &maxRegs[0], (k-3) );
    }

    return;
}

/*
 *  Function to implement the Top-Down register allocator.
 *  Allocator takes advantage of MAX_LIVE.
 */
void topDown()
{
    int lineNumber = 0;

    while( readLine() )
    {
        //printf(buffer);
        //printf("\n");

        lineNumber++; // just added
        char* opcode = getOpCode();
     
        // decide how to parse the line depending on the opcode
        if(!strcmp(opcode, "loadI"))
            parseLoadI(lineNumber);
        else if(!strcmp(opcode, "load"))
            parseLoad(lineNumber);
        else if(!strcmp(opcode, "store"))
            parseStore(lineNumber);
        else if(!strcmp(opcode, "add") || !strcmp(opcode, "sub") || 
            !strcmp(opcode, "mult") || !strcmp(opcode, "div") || 
            !strcmp(opcode, "lshift") || !strcmp(opcode, "rshift") || 
            !strcmp(opcode, "and") || !strcmp(opcode, "or") || 
            !strcmp(opcode, "xor") )
            parseThreeReg(lineNumber);
        else if(!strcmp(opcode, "addI") || !strcmp(opcode, "subI") || 
            !strcmp(opcode, "multI") || !strcmp(opcode, "divI") || 
            !strcmp(opcode, "lshiftI") || !strcmp(opcode, "rshiftI") )
            parseImmediate(lineNumber);
    }

    int maxLive;
    maxLive = calculateMaxLife(lineNumber);

    return;

}

/*
 *  Function to implement the Bottom-Up register allocator.
 */
void bottomUp()
{
    
    return;   
}

/*
 * Main function that runs the register allocator.
 */
int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        printf("ERROR: Expecting 3 arguments\n");
        return 1;
    }    
    
    // first arg is the number of avail registers (k)
    k = *argv[1];
    algorithm = *argv[2];
    char* input_string = argv[3];

    int i;
    for(i=0; i < 256; i++)
    {
        // initialize the liveRangeArray
        liveRangeArray[i] = (live_ranges_t *) malloc(sizeof(live_ranges_t));
        liveRangeArray[i]->regNum = -1;
        liveRangeArray[i]->count = 0;
        liveRangeArray[i]->startingPos = 0;
        liveRangeArray[i]->endingPos = 0;
    }

    // set up the read-in buffer
    buffer = malloc(BUFF_SIZE*sizeof(char));
    memset(buffer, '\0', BUFF_SIZE);

    // open the input file, read-only
    input_file = fopen(input_string, "r");

    // choose which allocator is going to be used
    switch(*argv[2])
    {
        case 's':   // simple top-down allocator
            simpleTopDown(k);
            break;

        case 't':   // top-down with live ranges
            topDown();
            break;

        case 'b':   // bottom-up allocator
            bottomUp();
            break;

        default:
            printf("ERROR: Incorrect allocator type.\n");
            if(fclose(input_file))
            {
                printf("ERROR: Failure to close file.\n");
            }
            return 1;
    }


    if(fclose(input_file))
    {
        printf("ERROR: Failure to close file.\n");
    }

    printLiveRanges();
    
    return 0;
}

