/*
 * Instruction-level simulator for the LC
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define NUMMEMORY 256 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

#define ADD 0
#define NAND 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5
#define HALT 6
#define DIV 7
#define XADD 8
#define XIDIV 9
#define AND 10
#define XOR 11
#define CMPL 12
#define JMAE 13
#define JMLE 14
#define SBB 15
#define BT 16
#define RCR 17
#define SETBR 18


typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
    int BR;
    int CF; 
} stateType;

void printState(stateType *);
void run(stateType);
int convertNum(int);

int main(int argc, char *argv[])
{
    int i;
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    state.CF = 0;
    if (argc != 2) {
	printf("error: usage: %s <machine-code file>\n", argv[0]);
	exit(1);
    }

    /* initialize memories and registers */
    for (i=0; i<NUMMEMORY; i++) {
	state.mem[i] = 0;
    }
    for (i=0; i<NUMREGS; i++) {
	state.reg[i] = 0;
    }

    state.pc=0;

    /* read machine-code file into instruction/data memory (starting at
	address 0) */

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
	printf("error: can't open file %s\n", argv[1]);
	perror("fopen");
	exit(1);
    }

    for (state.numMemory=0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
		state.numMemory++) {
		if (state.numMemory >= NUMMEMORY) {
			printf("exceeded memory size\n");
			exit(1);
		}
		if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
			printf("error in reading address %d\n", state.numMemory);
			exit(1);
		}
		printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    printf("\n");
    
    /* run never returns */
    run(state);

    return(0);
}

void run(stateType state)
{
    int arg0, arg1, arg2, addressField, NewAddr, bit;
    int Temp;
    int instructions=0;
    int opcode;
    int maxMem=-1;	/* highest memory address touched during run */
	state.BR = 0;
    for (; 1; instructions++) { /* infinite loop, exits when it executes halt */
		printState(&state);

	if (state.pc < 0 || state.pc >= NUMMEMORY) {
	    printf("pc went out of the memory range\n");
	    exit(1);
	}
	bit = (state.mem[state.pc]) & 0x1;
	if (bit == 1)
	{
		NewAddr = 1;
	}
	else if(bit == 0)
	{
		NewAddr = 0;
	}
	else {
		printf("error value of bit");
	}
	maxMem = (state.pc > maxMem) ? state.pc : maxMem;

	printf("bit = %d\n", bit);
	opcode = state.mem[state.pc] >> 15;
	arg0 = (state.mem[state.pc] >> 12) & 0x7;
	arg1 = (state.mem[state.pc] >> 9) & 0x7;
	arg2 = (state.mem[state.pc] >> 1) & 0x7;

	addressField = convertNum((state.mem[state.pc] >> 1) & 0xFF); 

	state.pc++;
	if (opcode == ADD) {
		state.reg[arg2] = state.reg[arg0] + state.reg[arg1];
		if (state.pc >= NUMMEMORY)
			state.CF = 1;
	}
	else if (opcode == NAND) {
		state.reg[arg2] = ~(state.reg[arg0] & state.reg[arg1]);
	}
	else if (opcode == XOR) {
			state.reg[arg2] = state.reg[arg0] ^ state.reg[arg1];
	}
	else if (opcode == DIV) {
		if (state.reg[arg0] < 0 || state.reg[arg1] < 0)
		{
			printf("Error: function DIV can only operate with positive numbers\n");
			exit(1);
		}
		if (state.reg[arg1] == 0)
		{
			printf("Error: division by zero!");
			exit(1);
		}
			state.reg[arg2] = state.reg[arg0] / state.reg[arg1];
	}
	else if (opcode == XADD) {
			state.reg[arg2] = state.reg[arg0] + state.reg[arg1];
			Temp = state.reg[arg0];
			state.reg[arg0] = state.reg[arg1];
			state.reg[arg1] = Temp;
			if (state.pc >= NUMMEMORY)
				state.CF = 1;
	}
	else if (opcode == XIDIV) {
		if (state.reg[arg1] == 0)
		{
			printf("Error: division by zero!");
			exit(1);
		}
			state.reg[arg2] = state.reg[arg0] / state.reg[arg1];
			Temp = state.reg[arg0];
			state.reg[arg0] = state.reg[arg1];
			state.reg[arg1] = Temp;
	}
	else if (opcode == AND) {
			state.reg[arg2] = state.reg[arg0] & state.reg[arg1];
	}
	else if (opcode == CMPL) {
		state.reg[arg2] = (state.reg[arg0] < state.reg[arg1]) ? state.reg[arg0] : state.reg[arg1];
	}
	else if (opcode == SBB) {
		state.reg[arg2] = state.reg[arg0] - state.reg[arg1] - state.CF;
		if (state.pc >= NUMMEMORY)
			state.CF = 1;
	}
	else if (opcode == RCR) {
		int temp = state.reg[arg0], temp2;
		int i, iCount = 0;
		while (1)
		{
			temp = temp >> 1;
			iCount++;
			if (temp == 0)
				break;
		}
		temp = state.reg[arg0];
		for (i = 0; i < state.reg[arg1]; i++)
		{
			state.CF = temp & 0x001;
			temp2 = state.CF << (iCount - 1);
			temp = temp >> 1;
			temp = temp2 | temp;
		}
		state.reg[arg2] = temp;
		state.CF = 0;
	}
	else if (opcode == BT) {
		state.CF = (state.reg[arg0] >> state.reg[arg1]) & 1;
	}
	else if (opcode == LW) {
		if (state.reg[arg0] + addressField < 0 ||
			state.reg[arg0] + addressField >= NUMMEMORY || 
			state.mem[state.reg[arg0] + state.BR + addressField] < 0 ||
			state.mem[state.reg[arg0] + state.BR + addressField] >= NUMMEMORY) {
			printf("address out of bounds\n");
			exit(1);
		}
		if (NewAddr == 1) {
			state.reg[arg1] = state.mem[state.reg[arg0] + state.BR + addressField];
			if (state.reg[arg0] + state.BR + addressField > maxMem)
			{
				maxMem = state.BR + arg1;
			}
		}
		else if (NewAddr == 0) {
		state.reg[arg1] = state.mem[state.reg[arg0] + addressField];
		if (state.reg[arg0] + addressField > maxMem) {
			maxMem = state.reg[arg0] + addressField;
		}
	}
	}
	else if (opcode == SW) {
		if (state.reg[arg0] + addressField < 0 ||
			state.reg[arg0] + addressField >= NUMMEMORY  ||
			state.mem[state.reg[arg2] + state.BR + addressField] <0 ||
			state.mem[state.reg[arg2] + state.BR + addressField] >= NUMMEMORY) {
			printf("address out of bounds\n");
			exit(1);
		}
		if (NewAddr == 1) {
			state.mem[state.reg[arg2] + state.BR + addressField] = state.reg[arg1];
			if (state.reg[arg2] + state.BR + addressField > maxMem)
			{
				maxMem = state.BR + state.reg[arg1];
			}
		}
		else if(NewAddr == 0) {
		state.mem[state.reg[arg0] + addressField] = state.reg[arg1];
		if (state.reg[arg0] + addressField > maxMem) {
			maxMem = state.reg[arg0] + addressField;
		}
	}
	} 
	else if (opcode == BEQ) {
		if (state.reg[arg0] == state.reg[arg1]) {
			state.pc += addressField;
		}
	}
	else if (opcode == JMAE) {
		if (state.reg[arg0] < 0 || state.reg[arg1] < 0)
		{
			printf("Error: function JMAE can compair only unsigned operands!\n");
			exit(1);
		}
		if (NewAddr == 1) {
			if (state.reg[arg0] >= state.reg[arg1]) {
				state.pc += state.BR + addressField;
			}
		}
		else if (NewAddr == 0){
			if (state.reg[arg0] >= state.reg[arg1]) {
				state.pc += addressField;
			}
		}
	}
	else if (opcode == JMLE) {
		if (NewAddr == 1) {
			if (state.reg[arg0] <= state.reg[arg1]) {
				state.pc += state.BR + addressField;
			}
		}
		else if (NewAddr == 0){
			if (state.reg[arg0] <= state.reg[arg1]) {
				state.pc += addressField;
			}
		}
	}
	else if (opcode == SETBR)
	{
		state.BR = state.reg[arg0];
	}
	else if (opcode == JALR) {
		state.reg[arg1] = state.pc;
		if (arg0 != 0)
			state.pc = state.reg[arg0];
		else
			state.pc = 0;
	}
	else if (opcode == HALT) {
		printf("machine halted\n");
		printf("total of %d instructions executed\n", instructions + 1);
		printf("final state of machine:\n");
		printState(&state);
		exit(0);
	}
	else {
		printf("error: illegal opcode 0x%x\n", opcode);
		exit(1);
	}
	state.reg[0] = 0;
	}
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
	for (i=0; i<statePtr->numMemory; i++) {
	    printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}
    printf("\tregisters:\n");
	for (i=0; i<NUMREGS; i++) {
	    printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
    printf("\t\tBase: %d\n", statePtr->BR);
    printf("\tFlags:\n");
    printf("\t\tCF %d\n", statePtr->CF);
    printf("end state\n");
}

int
convertNum(int num)
{
    /* convert a 8-bit number into a 16-bit Sun integer */
    if (num & (1<<7) ) {
	num -= (1<<8);
    }
    return(num);
}
