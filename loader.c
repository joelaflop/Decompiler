#include "loader.h"
//#include <stdio.h>
#include <string.h>
#include <ctype.h>

inst instructions;
void load(FILE **files, int filesCnt){
	int i;
	init(&instructions);
	char inst[5];
	for(i = 1; i < filesCnt; i++){
		int f, c, cc;
		c = fgetc(files[i]);
		//printf("%02X ", c);
		f = 1;
		cc=0;
		while(c != EOF){
			if(f == 1){
				cc = c << 8;
			} else if(f==2){
				cc += c;
				sprintf(inst, "%04x", cc);
				//printf("inst: %s\n", inst);
				add(&instructions, inst);
				f=0;
			}
			f++;
			c = fgetc(files[i]);
		}
	}
}

inst getInstructions(){
	return instructions;
}

void endLoader(){
	clear(&instructions);
}
