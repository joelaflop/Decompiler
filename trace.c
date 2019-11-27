#include "trace.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

int PC = 0x8200, PCP = 0x8200, NZP = 0;
int R[9];
int RegWE = 0, NZPWE = 0, DataWE = 0, LS = 0, PSR = 1;
int Mem = 0, Val = 0, RR = 0, RV = 0;
char **mem;
inst trac;
int quit;

int i;
char ret[30];

int trace(){
	init(&trac);
	quit = 0;
	for(i = 0; i < 8; i++){
		R[0] = 0;
	}
	mem = getMem();
	while(PC != 0x80FF && PC < 0xFFFF){
		if(type(mem[PC]) == 0){
			//instruction
			PCP = PC;
			char instr[4];
			for(i = 7; i<11; i++){
				instr[i-7] = mem[PC][i];
			}
			int instrn;
			sscanf(instr, "%04X", &instrn); 
			execute(instrn);
			if(quit == 1){
				return 1;
			}else{
				//printf("%04x : %s", PC, ret);
				add(&trac, getTrace(instrn)); //!!!!!!!!!!!!!!!!!!!!!!!!!!
				PC = PCP;
			}
		} else {
			return 1;
		}
		for(i = 0; i < 8; i++){
			R[i] = R[i] & 0xFFFF;
			//printf("R%d=%04X\n",i,R[i]);
		}
	}
	freeMem();
	return 0;
}

void execute(int n){;
	//printf("FIRSTn:%04X\n", n);
	int f = n >> 12;
	strcpy(ret, "");
	//printf("\nn: %04x, firstFour: %04x ||", n, f);
	//printf("n: %d, firstFour: %d ||", n, f);
	int rt, rs, rd;
	n = n & 0x0FFF;
	rd = (n & 0xE00) >> 9;
	rs = (n & 0x1C0) >> 6;
	rt = (n & 0x07);	
			
	switch (f){
		case 0:
			RegWE = 0;
			NZPWE = 0;
			DataWE = 0;
			LS = 0;
			rt = n & 0x1FF;
			if(rt >= 0x100){
				rt = rt - 0x200;
			}
			switch (rd){
				case 0:
					sprintf(ret, "");
					PCP++;
					break;
				case 1:
					sprintf(ret, "BRp #%d", rt);
					if(NZP == 1){
						PCP += 1 + rt;
					} else {
						PCP++;
					}
					break;
				case 2:
					sprintf(ret, "BRz #%d", rt);
					if(NZP == 2){
						PCP += 1 + rt;
					} else {
						PCP++;
					}
					break;
				case 3:
					sprintf(ret, "BRzp #%d", rt);
					if(NZP == 2 || NZP == 1){
						PCP += 1 + rt;
					} else {
						PCP++;
					}
					break;
				case 4:
					sprintf(ret, "BRn #%d", rt);
					if(NZP == 4){
						PCP += 1 + rt;
					} else {
						PCP++;
					}
					break;
				case 5:
					sprintf(ret, "BRnp #%d", rt);
					if(NZP == 4 || NZP == 1){
						PCP += 1 + rt;
					} else {
						PCP++;
					}
					break;
				case 6:
					sprintf(ret, "BRnz #%d", rt);
					if(NZP == 4 || NZP == 2){
						PCP += 1 + rt;
					} else {
						PCP++;
					}
					break;
				case 7:
					sprintf(ret, "BRnzp #%d", rt);
					PCP += 1 + rt;
					break;
				default:
					sprintf(ret, "INVALID INSTRUCTION");
			}
			break;
		case 1:
			RegWE = 1;
			NZPWE = 1;
			DataWE = 0;
			LS = 0;
			f = (n >> 3) & 0x7;
			if(f == 0){
				R[rd] = R[rs] + R[rt];
				PCP++;
				NZP = nzpR(R[rd]);
				sprintf (ret, "ADD R%d=%04X, R%d, R%d", rd, R[rd], rs, rt);
			}else if(f == 1){
				R[rd] = R[rs] * R[rt];
				PCP++;
				NZP = nzpR(R[rd]);
				sprintf(ret,"MUL R%d, R%d, R%d", rd, rs, rt);
			}else if(f == 2){
				R[rd] = R[rs] - R[rt];
				PCP++;
				
				NZP = nzpR(R[rd]);
				sprintf(ret,"SUB R%d, R%d, R%d", rd, rs, rt);
			}else if(f == 3){
				R[rd] = R[rs] / R[rt];
				PCP++;
				NZP = nzpR(R[rd]);
				sprintf(ret,"DIV R%d, R%d, R%d", rd, rs, rt);
			}else {
				rt = n & 0x1F;
				if(rt >= 0x10){
					rt = rt - 0x20;
				}
				R[rd] = R[rs] + rt;
				PCP++;
				NZP = nzpR(R[rd]);
				sprintf(ret, "ADD R%d=%04X, R%d, #%d", rd, R[rd], rs, rt);
			}
			Mem = rd;
			Val = R[rd];
			break;
		case 2: 
			RegWE = 0;
			NZPWE = 1;
			DataWE = 0; 
			LS = 0;
			rs = (n & 0x180)>>7;
			switch (rs){
				case 0:
					if(R[rt] >= 0x8000 ^ R[rd] >= 0x8000)
						if(R[rt] >= 0x8000 && R[rd] < 0x8000)
							NZP = nzp(R[rd]+ 0xFFFF - R[rt]);
						else
							NZP = nzp(R[rd]- 0xFFFF - R[rt]);
					else
						NZP = nzp(R[rd] - R[rt]);
					
					PCP++;
					sprintf(ret, "CMP R%d=%04X, R%d=%04X", rd, R[rd], rt, R[rt]);
					break;
				case 1:
					NZP = nzp(abs(R[rd]) - abs(R[rt]));
					
					PCP++;
					sprintf(ret, "CMPU R%d, R%d", rd, rt);
					break;
				case 2:
					rt = n & 0x7F;
					if(rt >= 0x40){
						rt = rt - 0x80;
					}
					NZP = nzp(R[rd] - rt);
					PCP++;
					sprintf(ret, "CMPI R%d, #%d", rd, rt);
					break;
				case 3:
					rt = n & 0x7F;
					NZP = nzp(abs(R[rd]) - abs(rt));
					PCP++;
					sprintf(ret, "CMPIU R%d, #%d", rd, rt);
					break;
				default:
					sprintf(ret, "INVALID INSTRUCTION");
			}
			//printf("CMP");
			break;
		case 4:
			RegWE = 1;
			NZPWE = 1;
			DataWE = 0;
			LS = 0;
			rd = n >> 11;
			if(rd == 1){
				rt = n & 0x7FF;
				if(rt >= 0x400){
						rt = rt - 0x800;
					}
				R[7] = PC + 1;
				NZP = nzp(PC + 1);
				PCP = (PC & 0x8000) | (rt << 4);
				sprintf(ret, "JSR #%d", rt);
				//printf("JSr");
			} else if(rd == 0){
				PCP = R[rs];
				R[7] = PC + 1;
				NZP = nzp(PC + 1);
				
				sprintf(ret, "JSRR R%d", rs);
				//printf("JSRRR");
			}
			Mem = 7;
			Val = R[7];
			
			break;
		case 5:
			//printf("logic");
			RegWE = 1;
			NZPWE = 1;
			DataWE = 0;
			LS = 0;
			f = (n >> 3) & 0x7;
			if(f ==0){
				R[rd] = R[rs] & R[rt];
				NZP = nzpR(R[rd]);
				PCP++;
				sprintf (ret, "AND R%d, R%d, R%d", rd, rs, rt);
			}else if(f == 1){
				R[rd] = 0xFFFF - R[rs];
				NZP = nzpR(R[rd]);
				PCP++;
				sprintf(ret,"NOT R%d, R%d", rd, rs);
			}else if(f == 2){
				R[rd] = R[rs] | R[rt];
				NZP = nzpR(R[rd]);
				PCP++;
				sprintf(ret,"OR R%d, R%d, R%d", rd, rs, rt);
			}else if(f == 3){
				R[rd] = R[rs] ^ R[rt];
				NZP = nzpR(R[rd]);
				PCP++;
				sprintf(ret,"XOR R%d, R%d, R%d", rd, rs, rt);
			}else {
				rt = n & 0x1F;
				if(rt >= 0x10){
						rt = rt - 0x20;
				}
				R[rd] = R[rs] & rt;
				NZP = nzpR(R[rd]);
				PCP++;
				sprintf(ret, "AND R%d, R%d, #%d", rd, rs, rt);
			}
			Mem = rd;
			Val = R[rd];
			break;
		case 6:
			//printf("ldr");
			RegWE = 1;
			NZPWE = 1;
			DataWE = 0;
			LS = 1;
			//printf("n:%04X\n", n);
			int con = n & 0x3F;
			if(con >= 0x20){
				con = con - 0x40;
			}
			RR = R[rs] + con;
			
			int throwaway = 0;
			if(type(mem[RR]) == 0){
				quit = 1;
				printf("quitting on this load: %s", mem[RR]);
			}else {
				if(sscanf(mem[RR], "%04X : %04X",&throwaway, &R[rd])<2){
					if(sscanf(mem[RR], "%05X%04X",&throwaway, &R[rd])<2){
						R[rd] = 0;
					}
				}
			}
			if((RR<0x2000)||(RR >= 0x8000 && RR < 0xA000)){
				quit = 1;
			}

			if(R[rd] >= 0x8000){
				//printf("negated RD");
				NZP = nzp(-1);
			}else if(R[rd] == 0){
				NZP = nzp(0);
			}else
				NZP = nzp(1);
			PCP++;
			Mem = rd;
			Val = R[rd];
			RV = R[rd];
			if(PSR == 0 &&(RR >= 0x8000 || Mem >= 0x8000)){
				quit = 1;
			}
			sprintf(ret, "LDR R%d = %04X, R%d = %04X, #%d", rd, R[rd], rs, R[rs], con);
			break;
		case 7:
			RegWE = 0;
			NZPWE = 0;
			DataWE = 1;
			LS = 1;
			rt = n & 0x3F;
			if(rt >= 0x20){
				rt = rt - 0x40;
			}
			RR = R[rs] + rt;
			char a[7] = "";
			char d[7] = "";
			if((RR<0x2000)||(RR >= 0x8000 && RR < 0xA000)){
				quit = 1;
			}
			sprintf(a, "%04X : ", RR); 
			
			sprintf(d, "%04X", R[rd]);
			strcpy(mem[RR], a);
			strcat(mem[RR], d);	
			RV = R[rd];		
			PCP++;
			if(PSR == 0 && (RR >= 0x8000)){
				quit = 1;
			}
			sprintf(ret, "STR R%d=%04X, R%d=%04X, #%d", rd, R[rd], rs, R[rs], rt);
			break;
		case 8:
			RegWE = 0;
			NZPWE = 0;
			DataWE = 0;
			LS = 0;
			//printf("RTI");
			PCP = R[7];
			PSR = 0;
			sprintf(ret, "RTI");
			break;
		case 9:
			RegWE = 1;
			NZPWE = 1;
			DataWE = 0;
			LS = 0;
			rt = n & 0x1FF;
			if(rt >= 0x100){
				rt = rt - 0x200;
			}
			R[rd] = rt;
			PCP++;
			if(rt<0)
				NZP = 4;
			else if(rt > 0)
				NZP = 1;
			else
				NZP = 2;
			Mem = rd;
			Val = R[rd];
			sprintf(ret, "CONST R%d, #%d", rd, rt);
			//printf("CONST");
			break;
		case 10:
			//printf("shift & mod");
			RegWE = 1;
			NZPWE = 1;
			DataWE = 0;
			LS = 0;
			rt = (n >> 4) & 0x3;
			if(rt == 0){
				rt = n & 0xF;
				R[rd] = R[rs] << rt;
				NZP = nzpR(R[rd]);
				PCP++;
				sprintf(ret, "SLL R%d, R%d, #%d", rd, rs, rt);
			}else if(rt == 1){
				rt = n & 0xF;

				short slide = (((short)R[rd]) >> rt);
				
				
				R[rd] = slide;
				R[rd] = R[rd] & 0xFFFF;
				NZP = nzpR(R[rd]);
				PCP++;
				sprintf(ret, "SRA R%d=%04X, R%d, #%d", rd, R[rd], rs, rt);

			}else if(rt == 2){
				unsigned int shif = n & 0xF;
				//MAKE SURE THIS IS A LOGICAL SHIFTTT
				R[rd] = R[rs] >> shif;
				NZP = nzpR(R[rd]);
				PCP++;
				sprintf(ret, "SRL R%d, R%d, #%d", rd, rs, shif);

			}else if(rt == 3){
				rt = n & 0x7;
				R[rd] = R[rs] % R[rt];
				NZP = nzpR(R[rd]);
				PCP++;
				sprintf(ret, "MOD R%d, R%d, R%d", rd, rs, rt);

			} else{
				sprintf(ret, "INVALID INSTRUCTION");
			}
			Mem = rd;
			Val = R[rd];
			break;
		case 12:
			//printf("JMP");
			RegWE = 0;
			NZPWE = 0;
			DataWE = 0;
			LS = 0;
			rt = (n >> 11);
			if(rt == 1){
				rt = n & 0x7FF;
				if(rt >= 0x400){
						rt = rt - 0x800;
				}
				PCP = PCP + 1 + rt;
				sprintf(ret, "JMP #%d", rt);
			}else if(rt == 0){
				PCP = R[rs];
				sprintf(ret, "JMPR R%d", rs);
			}else{
				sprintf(ret, "INVALID INSTRUCTION");
			}
			break;
		case 13:
			//printf("HICONST");
			RegWE = 1;
			NZPWE = 1;
			DataWE = 0;
			LS = 0;
			rt = n & 0xFF;
			R[rd] = (R[rd] & 0xFF) | (rt << 8);
			PCP++;
			if(rt >= 0x80){
				//printf("negated RD");
				NZP = 4;
			}else if(rt == 0){
				NZP = 2;
			}else
				NZP = 1;
			Mem = rd;
			Val = R[rd];
			sprintf(ret, "HICONST R%d, #%d", rd, rt);
			break;
		
		case 15:
			//printf("trap");
			RegWE = 1;
			NZPWE = 1;
			DataWE = 0;
			LS = 0;
			rt = n & 0xFF;
			R[7] = PC + 1;
			NZP = nzp(R[7]);
			PCP = 0x8000 | rt;
			PSR = 1;
			Mem = 7;
			Val = R[7];
			sprintf(ret, "TRAP #%d", rt);
			break;
		default:
			sprintf(ret, "INVALID INSTRUCTION");
			break;
	}
	strcat(ret, "\n");
}

char* getTrace(int instruction){
	static char t[60];
	sprintf(t, "%04X %s %d ", PC, intToBinary(instruction), RegWE); //24 chars
	if(RegWE == 1){
		t[24] = intToHex(Mem & 0xF);
		t[25] = ' ';
		Val = Val & 0xFFFF;
		t[26] = intToHex(Val >> 12);
		t[27] = intToHex(Val >> 8 & 0xF);
		t[28] = intToHex(Val >> 4 & 0xF);
		t[29] = intToHex(Val & 0xF);
	} else {
		t[24] = '0';
		t[25] = ' ';
		t[26] = '0';
		t[27] = '0';
		t[28] = '0';
		t[29] = '0';
	}
	t[30] = ' ';
	t[31] = NZPWE + '0';
	t[32] = ' ';
	if(NZPWE == 1){
		t[33] = NZP + '0';
	} else {
		t[33] = '0';
	}
	t[34] = ' ';
	t[35] = DataWE + '0';
	t[36] = ' ';
	if(LS == 1){
		RV = RV & 0xFFFF;
		RR = RR & 0xFFFF;
		t[37] = intToHex(RR >> 12);
		t[38] = intToHex(RR >> 8 & 0xF);
		t[39] = intToHex(RR >> 4 & 0xF);
		t[40] = intToHex(RR & 0xF);
		t[41] = ' ';
		t[42] = intToHex(RV >> 12);
		t[43] = intToHex(RV >> 8 & 0xF);
		t[44] = intToHex(RV >> 4 & 0xF);
		t[45] = intToHex(RV & 0xF);
	} else {
		t[37] = '0';
		t[38] = '0';
		t[39] = '0';
		t[40] = '0';
		t[41] = ' ';
		t[42] = '0';
		t[43] = '0';
		t[44] = '0';
		t[45] = '0';
	}
	t[46] = '\n';
	
	return t;
}

char* intToBinary(int n){
	static char a[17];
	for(i = 0; i < 16; i++){
		a[15-i] = (n % 2) + '0';
		n = n / 2;
	}
	return a;
}

char intToHex(int n){
	//printf(" n: %d\n", n);
	if(n < 10 && n >=0){
		return n + '0';
	} else {
		switch(n){
			case 10:
				return 'A';
			case 11:
				return 'B';
			case 12:
				return 'C';
			case 13:
				return 'D';
			case 14:
				return 'E';
			case 15:
				return 'F';
			default:
				return 'X';
		}
	}
}

int type(char* curr){
	if(strstr(curr, "->") != NULL){
		return 0;
	} else {
	 	return 1;
	}
}

int nzpR(int n){
	n = n & 0xFFFF;
	if(n >= 0x8000){
		return 4;
	} else if(n == 0){
		return 2;
	} else {
		return 1;
	}
}
int nzp(int n){
	if(n < 0){
		return 4;
	} else if(n == 0){
		return 2;
	} else {
		return 1;
	}
}

inst getTrac(){
	return trac;
}
