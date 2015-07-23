
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


struct live_ranges
{
    int regNum;
    int count;
    int startingPos;
    int endingPos;
};
typedef struct live_ranges live_ranges_t;


/* *********************
 * FUNCTION DECLARATIONS
 * *********************/

void editLiveRange(int registerNumber, int lineNumber, int target);
int calculateMaxLife(int totalLines);
void adjustMaxRegs(int* maxRegs, int start, int length);
void calcMostOccurrences(int* maxRegs, int length);

int readFile();
char * getOpCode();

void parseLoadI(int lineNumber);
void parseLoad(int lineNumber);
void parseStore(int lineNumber);
void parseThreeReg(int lineNumber);
void parseImmediate(int lineNumber);

void readLoadI(int* maxRegs, int size);
void readLoad(int* maxRegs, int size);
void readStore(int* maxRegs, int size);
void readThreeReg(int* maxRegs, int size);
void readImmediate(int* maxRegs, int size);

void genLoadI(int loc, int target);
void genLoad(int source, int target);
void genStore(int source, int target);
void genThreeReg(char* opcode, int reg1, int reg2, int reg3);
void genImmediate(char* opcode, int reg1, int reg2, int immediate);

void simpleTopDown();
void topDown();
void bottomUp();

