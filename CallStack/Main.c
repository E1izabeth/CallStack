#include <stdio.h>

void printStackTrace();
void f();
void g();
int main();


typedef struct StateInfo {
	intptr_t prev;
	int retAddr;
}StateInfo;

unsigned short funciIn[10];
char* names[10];
int deepthInd = 0;

void printStackTrace()
{/*
	FuncInfo info;

	int eipR;
	__asm
	{
		call get_eip
		mov eipR, eax
		jmp lpb
	get_eip: mov eax, [esp]
			 ret
			lpb:
	}
	info.begin = eipR;*/

	funciIn[deepthInd] = 1;
	names[deepthInd] = "printStackTrace";
	deepthInd++;

	StateInfo* state = 0;
	int address;
	__asm
	{
		mov address, ebp;
	};

	printf("------------------------------------------\n");
	printf("saved ebp\treturn address\n");
	state = (StateInfo*)address;
	int i = deepthInd - 1;
	while ((int)(state->prev) != 0)
	{
		printf("------------------------------------------\n");
		if(i > 0)
			printf("%x\t\t%x  %s\n", (int)state->prev, state->retAddr, names[i]);
		else
			printf("%x\t\t%x\n", (int)state->prev, state->retAddr);
		printf("------------------------------------------\n");
		state = (StateInfo*)(state->prev);
		if ((int)(state->prev) != 0)
		{
			printf("\t^\t\n");
			printf("\t|\t\n");
		}
		i--;
	}
	printf("\n");


	funciIn[deepthInd] = 0;
	names[deepthInd] = '\0';
	deepthInd--;
}

intptr_t retAddress;

void capture()
{
	StateInfo* state = 0;
	__asm
	{
		mov state, ebp;
	};
	retAddress = state->retAddr;
}

void splice()
{
	StateInfo* state = 0;

	__asm
	{
		mov state, ebp;
	};
	state->retAddr = retAddress;
}

void f()
{	

	funciIn[deepthInd] = 1;
	names[deepthInd] = "f";
	deepthInd++;


	printf("f1!\n");
	capture();
	printf("f2!\n");
	
	printStackTrace();

	funciIn[deepthInd] = 0;
	names[deepthInd] = '\0';
	deepthInd--;
}

void g()
{

	funciIn[deepthInd] = 1;
	names[deepthInd] = "g";
	deepthInd++;

	// f();
	printf("g1!\n");
	splice();
	printf("g2!\n");

	funciIn[deepthInd] = 0;
	names[deepthInd] = '\0';
	deepthInd--;
}


int main()
{

	funciIn[deepthInd] = 1;
	names[deepthInd] = "main";
	deepthInd++;

	//f();
	//g();

	funciIn[deepthInd] = 0;
	names[deepthInd] = '\0';
	deepthInd--;
	return 0;
}

