#include "LC4.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
char mem[65536][50];
int i;
char ret[30];

char* findInst(int n){;
	int f = n >> 12;
	strcpy(ret, "");
	//printf("\nn: %04x, firstFour: %04x ||", n, f);
	//printf("n: %d, firstFour: %d ||", n, f);
	int rt, rs, rd;
	n = n & 0x0FFF;
	rd = (n & 0xE00) >> 9;
	rs = (n & 0x1C0) >> 6; //hellpo eight
	rt = (n & 0x07);	
			
	switch (f){
		case 0:
			rt = n & 0x1FF;
			if(rt >= 0x100){
				rt = rt - 0x200;
				}
			switch (rd){
				case 0:
					sprintf(ret, "");
					break;
				case 1:
					sprintf(ret, "BRp #%d", rt);
					break;
				case 2:
					sprintf(ret, "BRz #%d", rt);
					break;
				case 3:
					sprintf(ret, "BRzp #%d", rt);
					break;
				case 4:
					sprintf(ret, "BRn #%d", rt);
					break;
				case 5:
					sprintf(ret, "BRnp #%d", rt);
					break;
				case 6:
					sprintf(ret, "BRnz #%d", rt);
					break;
				case 7:
					sprintf(ret, "BRnzp #%d", rt);
					break;
				default:
					sprintf(ret, "INVALID INSTRUCTION");
			}
			break;
		case 1:
			f = (n >> 3) & 0x7;
			if(f ==0){
				sprintf (ret, "ADD R%d, R%d, R%d", rd, rs, rt);
			}else if(f == 1){
				sprintf(ret,"MUL R%d, R%d, R%d", rd, rs, rt);
			}else if(f == 2){
				sprintf(ret,"SUB R%d, R%d, R%d", rd, rs, rt);
			}else if(f == 3){
				sprintf(ret,"DIV R%d, R%d, R%d", rd, rs, rt);
			}else {
				rt = n & 0x1F;
				if(rt >= 0x10){
					rt = rt - 0x20;
				}
				sprintf(ret, "ADD R%d, R%d, #%d", rd, rs, rt);
			}
			break;
		case 2:  
			rs = (n & 0x180)>>7;
			switch (rs){
				case 0:
					sprintf(ret, "CMP R%d, R%d", rd, rt);
					break;
				case 1:
					sprintf(ret, "CMPU R%d, R%d", rd, rt);
					break;
				case 2:
					rt = n & 0x7F;
					if(rt >= 0x40){
						rt = rt - 0x80;
					}
					sprintf(ret, "CMPI R%d, #%d", rd, rt);
					break;
				case 3:
					rt = n & 0x7F;
					sprintf(ret, "CMPIU R%d, #%d", rd, rt);
					break;
				default:
					sprintf(ret, "INVALID INSTRUCTION");
			}
			//printf("CMP");
			break;
		case 4:
			rd = n >> 11;
			if(rd == 1){
				rt = n & 0x7FF;
				if(rt >= 0x400){
						rt = rt - 0x800;
					}
				sprintf(ret, "JSR #%d", rt);
			} else if(rd == 0){
				sprintf(ret, "JSRR R%d", rs);
			}
			//printf("JSr");
			break;
		case 5:
			//printf("logic");
			f = (n >> 3) & 0x7;
			if(f ==0){
				sprintf (ret, "AND R%d, R%d, R%d", rd, rs, rt);
			}else if(f == 1){
				sprintf(ret,"NOT R%d, R%d", rd, rs);
			}else if(f == 2){
				sprintf(ret,"OR R%d, R%d, R%d", rd, rs, rt);
			}else if(f == 3){
				sprintf(ret,"XOR R%d, R%d, R%d", rd, rs, rt);
			}else {
				rt = n & 0x1F;
				if(rt >= 0x10){
						rt = rt - 0x20;
					}
				sprintf(ret, "AND R%d, R%d, #%d", rd, rs, rt);
			}
			break;
		case 6:
			//printf("ldr");
			rt = n & 0x3F;
			if(rt >= 0x20){
						rt = rt - 0x40;
					}
			sprintf(ret, "LDR R%d, R%d, #%d", rd, rs, rt);
			break;
		case 7:
			rt = n & 0x3F;
			if(rt >= 0x20){
				rt = rt - 0x40;
			}
			sprintf(ret, "STR R%d, R%d, #%d", rd, rs, rt);
			//printf("str");
			break;
		case 8:
			//printf("RTI");
			sprintf(ret, "RTI");
			break;
		case 9:
			rt = n & 0x1FF;
			if(rt >= 0x100){
				rt = rt - 0x200;
			}
			sprintf(ret, "CONST R%d, #%d", rd, rt);
			//printf("CONST");
			break;
		case 10:
			//printf("shift & mod");
			rt = (n >> 4) & 0x3;
			if(rt == 0){
				rt = n & 0xF;
				sprintf(ret, "SLL R%d, R%d, #%d", rd, rs, rt);
			}else if(rt == 1){
				rt = n & 0xF;
				sprintf(ret, "SRA R%d, R%d, #%d", rd, rs, rt);

			}else if(rt == 2){
				rt = n & 0xF;
				sprintf(ret, "SRL R%d, R%d, #%d", rd, rs, rt);

			}else if(rt == 3){
				rt = n & 0x7;
				sprintf(ret, "MOD R%d, R%d, R%d", rd, rs, rt);

			} else{
				sprintf(ret, "INVALID INSTRUCTION");
			}
			break;
		case 12:
			//printf("JMP");
			rt = (n >> 11);
			if(rt == 1){
				rt = n & 0x7FF;
				if(rt >= 0x400){
						rt = rt - 0x800;
					}
				sprintf(ret, "JMP #%d", rt);
			}else if(rt == 0){
				sprintf(ret, "JMPR R%d", rs);
			}else{
				sprintf(ret, "INVALID INSTRUCTION");
			}
			break;
		case 13:
			//printf("HICONST");
			rt = n & 0xFF;
			sprintf(ret, "HICONST R%d, #%d", rd, rt);
			break;
		
		case 15:
			//printf("trap");
			rt = n & 0xFF;
			sprintf(ret, "TRAP #%d", rt);
			break;
		default:
			sprintf(ret, "INVALID INSTRUCTION");
			break;
	}

	return ret;
}

int header(char *f){
	if(strcmp(f, "cade") == 0){
		return 5;
	}else if(strcmp(f, "dada") == 0){
		return 4;
	}else if(strcmp(f, "c3b7") == 0){
		return 3;
	}else if(strcmp(f, "f17e") == 0){
		return 2;
	}else if(strcmp(f, "715e") == 0){
		return 1;
	} else{
		return 0;
	}
}



void code(inst *instr){
	int address = 0;
	int lines = 0;
	int instNum = 0;
	char a[5] = "";
	char n[5] = "";
	int j = 0;
	int k = 0;
	pop(instr, a);
	sscanf(a, "%04X", &address);
	pop(instr, n);
	sscanf(n, "%04X", &lines);
	for(k = address; k < address + lines; k++){
		pop(instr, a);
		switch(header(a)){
			case 5:
				code(instr);
				break;
			case 4:
				data(instr);
				break;
			case 3:
				symbol(instr);
				break;
			case 2:
				fileName(instr);
				break;
			case 1:
				lineNumber(instr);
				break;
				
		}
		strcpy(mem[k], "");
		sprintf(mem[k], "%04X : ", k);
		for(j = 0; j < 5; j++){
			a[j] = toupper(a[j]);
		}
		strcat(mem[k], a);
		sscanf(a, "%04X", &instNum);
		strcat(mem[k], " -> ");
		char *instr = findInst(instNum);
		if(strcmp(instr, "") == 0){
			//strcpy(mem[k], "");
		} else {
			//strcat(mem[k], findInst(instNum));
			strcat(mem[k], "\n");
		}
	}
}

void data(inst *instr){
	int address = 0;
	int lines = 0;
	int instNum = 0;
	char a[5] = "";
	char n[5] = "";
	int j = 0;
	int k = 0;
	//printf("DATA");
	pop(instr, a);
	sscanf(a, "%04X", &address);
	pop(instr, n);
	sscanf(n, "%04x", &lines);
	for(k = address; k < address + lines; k++){
		pop(instr, a);
				switch(header(a)){
			case 5:
				code(instr);
				break;
			case 4:
				data(instr);
				break;
			case 3:
				symbol(instr);
				break;
			case 2:
				fileName(instr);
				break;
			case 1:
				lineNumber(instr);
				break;
				
		}
		sprintf(mem[k], "%04X : ", k);
		for(j = 0; j < 5; j++){
			a[j] = toupper(a[j]);
		}
		if(strcmp(a, "") == 0 || strcmp(a, "0000") == 0){
			strcpy(mem[k], "");
		} else {
			strcat(mem[k], a);
			sscanf(a, "%04X", &instNum);
			strcat(mem[k], "\n");
		}
	}
}

void symbol(inst *instr){
	int address = 0;
	int lines = 0;
	char a[5] = "";
	char n[5] = "";
	int k = 0;
	pop(instr, a);
	sscanf(a, "%04X", &address);
	pop(instr, n);
	sscanf(n, "%04X", &lines);
	for(k = address; k < address + lines - 3; k++){
		pop(instr, a);
				switch(header(a)){
			case 5:
				code(instr);
				break;
			case 4:
				data(instr);
				break;
			case 3:
				symbol(instr);
				break;
			case 2:
				fileName(instr);
				break;
			case 1:
				lineNumber(instr);
				break;
				
		}
	}
}

void fileName(inst *instr){
	int lines = 0;
	char a[5] = "";
	char n[5] = "";
	int k =0;
	pop(instr, n);
	sscanf(n, "%04X", &lines);
	for(k = 0; k < lines; k++){
		pop(instr, a);
				switch(header(a)){
			case 5:
				code(instr);
				break;
			case 4:
				data(instr);
				break;
			case 3:
				symbol(instr);
				break;
			case 2:
				fileName(instr);
				break;
			case 1:
				lineNumber(instr);
				break;
				
		}
	}
}

void lineNumber(inst *instr){
	int address = 0;
	int lines = 0;
	int fileIndex = 0;
	char a[5] = "";
	char n[5] = "";
	char m[5] = "";
	int k = 0;
	pop(instr, a);
	sscanf(a, "%04X", &address);
	pop(instr, n);
	sscanf(n, "%04X", &lines);
	pop(instr, m);
	sscanf(m, "%04X", &fileIndex);
	for(k = address; k < address + lines - 3; k++){
		pop(instr, a);
				switch(header(a)){
			case 5:
				code(instr);
				break;
			case 4:
				data(instr);
				break;
			case 3:
				symbol(instr);
				break;
			case 2:
				fileName(instr);
				break;
			case 1:
				lineNumber(instr);
				break;
				
		}
	}
}


void decode(){
	for(i = 0; i < 65536; i++){
		strcpy(mem[i], "");
	}
	inst instr = getInstructions();
	char f[5] = "";
	do{
		pop(&instr, f);
		if(f[0] == 'c' && f[1] == 'a' && f[2] == 'd' && f[3] == 'e'){
			code(&instr);
		} else if(f[0] == 'd' && f[1] == 'a' && f[2] == 'd' && f[3] == 'a'){
	      	data(&instr);
		} else if(f[0] == 'c' && f[1] == '3' && f[2] == 'b' && f[3] == '7'){
	      	 symbol(&instr);
		} else if(f[0] == 'f' && f[1] == '1' && f[2] == '7' && f[3] == 'e'){
	      	fileName(&instr);
		} else if(f[0] == '7' && f[1] == '1' && f[2] == '5' && f[3] == 'e'){
	      	 lineNumber(&instr);
		}
		
	} while(isEmpty(&instr) != 0);
}

char** m;
char** getMem(){
	m = malloc(65536*sizeof(char*));
	for(i = 0; i < 65536; i++){
		m[i] = malloc(40*sizeof(char*));
		strcpy(m[i], mem[i]);
	}
	return m;
}
void freeMem(){
	for(i = 0; i < 65536; i++){
		free(m[i]);
	}
	free(m);
}
