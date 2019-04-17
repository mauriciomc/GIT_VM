/******************************************************************
*                                                                 *
*   Title: vm.c                                                   *
*   Description: Implements the virtual machine in task 1         * 
*   Developer: Mauricio De Carvalho                               *
*   email: mauricio.m.c@gmail.com                                 *
*   Last updated: 08/06/2016                                      *
*                                                                 *
*   Details: This exercise has been coded for better human        *
*            interpretation. Therefore it has not been optimized. *
*******************************************************************/
//Necessary libraries
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

// Data structure for storing data parsed from binary file
typedef struct {
	int *data;
	int data_size;
	int image_size;
}stMyData;

// Alternatively, these function prototypes could be inserted in a .h header file
int decode(int instruction, char *binop, char *operation, int *optional);
void f(int v, int *sp, int *stack);
int g(int *sp, int *stack);
int execute(char binop, char operation, int optional, int *sp, int *ip, int *stack);


int main (int argc, char **argv){

	//Declare file pointer for opening argv[1]
	FILE *fp;

	//Declare the data structure (Our main Instruction, data and stack memory)
	stMyData stData;

	// Verify number of arguments
	if(argc != 2 && argc != 3) {
		printf("\n*** Usage ./vm.o <Name of file> (optional) *** \n*** optional = -p");
		exit(0);
	}
	
	// Open file and verify if it can be opened
	if(!(fp=fopen(argv[1],"r+"))){
		printf("\n*** Cannot open file ***\n");
		exit(0);
	}
	//Declare temporary unsigned variable for data acquisition
	int tmp;
	
	//Read first data-size value
	fscanf(fp,"%x\n",&tmp);
	
	//Add to structure
	stData.data_size=tmp;
	
	
	//Read second image-size value
	fscanf(fp,"%x\n",&tmp);
	stData.image_size=tmp;

	//Allocate array
	stData.data = (int *)malloc(sizeof(int)*(stData.data_size));

	// Declare temporary index to store data into each data[index].
	int index=0;

	//Iterate over the file inserting each value in its correct position according to the quantity of values
	for(index=0;index < stData.image_size+1; index++){
		fscanf(fp,"%x\n",&tmp);
		stData.data[index]=tmp;
	}
	//Consider we don't need the file anymore. Better to close it to avoid problems while executing VM.
	fclose(fp);

	//Data array allocated inside structure stData.data
	
	//Second part: VM operation

	//Declare binop, operation, optional, sp, ip
	int ip=0;
	int sp = stData.data_size; 
	int instruction;
	char binop;
	char operation;
	int optional;

	
	printf("\n *** Virtual Machine Execution Started *** \n\n");
	//Main instruction processing datapath (Fetch, decode, execute) as explained on task1.txt 
	do {
		instruction = stData.data[ip];
		ip++;
		decode(instruction, &binop, &operation, &optional);
		int retvalue = execute(binop, operation, optional, &sp, &ip, &stData.data[0]);
		if(!retvalue) break;
		else if(retvalue==-1) 
			printf("\n *** Instruction %x not supported *** \n",instruction);
		
	} while (1);
	
	//Print memory contents if -p option was selected 
	if(argc == 3){
		if(!strcmp(argv[2],"-p")){
			printf("\n \t*********** Memory contents ********** \n\n");
			for(index=0;index<stData.data_size;index++)
				printf("\t   data[%d] = %08x | %9d\n",index,stData.data[index], stData.data[index]);
		}
		printf("\n \t************************************** \n\n");
	}
	printf("\n *** Virtual Machine Execution Ended *** \n");

}

// Instruction decode. Perform bitwise operations on signed 32bit to extract binop, operation and optional data.
// Note: Operations resembles bitwise operation on floating point numbers.
int decode(int instruction, char *binop, char *operation, int *optional){
	*binop = instruction <= -1 ? 1 : 0;
	unsigned int tmp = (unsigned int)instruction & 0x7F000000; 
	*operation = (char)((tmp >> 24)&(unsigned int)0x000000FF); 
	*optional =  (int)((unsigned int)instruction & 0x00FFFFFF) ;
}


/* F operations ( As defined on the task text )         *
* sp and stack need to be passed on to function so it   *
* can also alter their statuses in PC memory.           *
* Thus available for every function                     */

void f(int v, int *sp, int *stack){
	*sp = *sp - 1;
	stack[*sp] = v;
}

// G operations ( As defined on the task text )
int g(int *sp, int *stack){
	int v = stack[*sp];
	*sp = *sp + 1;
	return v; 
}


// This is the main execution operation function which treats 0 and 1 binop instructions,
// then decodes and executes the desired operations, based on the operation field
int execute(char binop, char operation, int optional, int *sp, int *ip, int *stack){
	
	//for Store
	int tmp;

	//for getc
	char x;

	//for putc
	char out;

	//For conditional jmp
	int cond;
	int addr;
	
	if(!binop){
		//BinOp == 0 Operations, exactly as defined on the task text
		switch (operation) {
			// Pop 
			case 0: //*sp++
				*sp = *sp+1;
				break;
			// Push <constant>
			case 1:
				f(optional, sp, stack);
				break;
			// Push ip
			case 2:
				f(*ip, sp, stack);
				break;
			// Push sp
			case 3:
				f(*sp, sp, stack);
				break;
			// Load
			case 4:
				tmp = g(sp, stack);
				f(stack[tmp], sp, stack);
				break;
			// Store
			case 5:
				tmp  = g(sp, stack);
				addr = g(sp, stack);
				stack[addr] = tmp;
				break;
			// Jmp
			case 6:
				cond=g(sp,stack);
				addr=g(sp,stack);
				if ( cond != (int)0 ) *ip = addr;
				break;
			// Not  
			case 7:
				g(sp, stack) == 0 ? f(1, sp, stack) : f(0, sp, stack);
				break;
			// Putc
			case 8:
				out = (char)((unsigned int)g(sp, stack) & 0x000000FF);
				fprintf(stdout,"%c",out);
				fflush(stdout);
				break;
			// Getc 
			case 9:
				x = getc(stdin);
				f(((unsigned int)x & 0x000000FF), sp, stack);
				break;

			// Halt
			case 10:
				return 0;
				break;
			default:
			//Exception should be raised
			printf("\n**** Instruction %x%x%x cannot be executed ****\n",binop,operation,optional);
			printf("binop=%x , operation=%x, optional=%x \n", binop, operation, optional);
			return -1; 
			break;
		}
		
	}
	else{
		int b = g(sp, stack);
		int a = g(sp, stack);
		int result;
		// BinOp == 1 instructions
		switch (operation) {
			// Add
			case 0:
				result=a+b;
				break;
			// Sub	
			case 1:
				result=a-b;
				break;
			// Mul
			case 2:
				result=(int)a*b;
				break;
			// Div
			case 3: 
				//Condition if b==0, this should raise an NaN or div by 0 exception!
				result=(int)(a / b);
				break;
			// And
			case 4:
				result=a&b;
				break;
 			// Or
			case 5:
				result=a|b;
				break;
			// Xor
			case 6:
				result=a^b;
				break;
			// eq
			case 7:
				result=(a==b)?1:0;
				break;
			// lt
			case 8:
				result=a<b?1:0;
				break;
	
			default:
                    	    //Exception should be raised
                       	     printf("\n**** Instruction %x%x%x cannot be executed ****\n",binop,operation,optional);
                             printf("binop=%x , operation=%x, optional=%x \n", binop, operation, optional);
                             return -1;
			     break; //just in case
		}
		
		f(result,sp,stack);
	}
	// Return the value 1 to calling routing indicating instruction was correctly executed
	return 1;
}		

