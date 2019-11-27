#include "trace.h"

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){ 
	FILE *files[argc - 1];
	int i, filesCnt = 0;

	for(i = 0; i < argc; i++){
		char *string = strchr(argv[i], '.');
		if(string != NULL && !strcmp(string, ".obj")){
			files[filesCnt++] = fopen(argv[i], "rb");
		} else if(i>0){
			files[filesCnt++] = fopen(argv[i], "w");
		}

	}
	
	load(files, filesCnt);
	decode();

	int c = trace(); 
	inst trac = getTrac();
	char* curr = malloc(100);
	
	while(isEmpty(&trac) != 0){
		pop(&trac, curr);
		fputs(curr, files[0]);
	}
	
	
	for(i = 0; i < argc - 1; i++){
		fclose(files[i]);
	}	
	free(curr);
	if(c == 1){
		exit(1);
	}
	
}
