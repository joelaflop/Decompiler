trace : parser.c trace.c trace.h LC4.c LC4.h loader.c loader.h instructions.c instructions.h
	clang -Wall -o trace parser.c trace.c LC4.c loader.c instructions.c
clean : 
	rm trace *.o