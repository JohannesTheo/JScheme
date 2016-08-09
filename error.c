#include <stdlib.h>
#include <stdio.h>
#include "jscheme.h"

void
error(char* msg, char* fileName, int line){
	fprintf(stderr, "%s [%d]: %s\n", fileName, line, msg);
	abort();
}

void
js_error(char* msg, OBJ arg){

	fprintf(stderr,"-------------\n");
	fprintf(stderr,"JS-error: %s ", msg);
	js_print(stderr, arg);
	fprintf(stderr,"\n-------------\n");
	
	fflush(stdout);
	getMeOutOfHere();
}

void
js_function_error(char* msg, OBJ arg, int expected, int given){

	char msgBuffer[512];
	snprintf(msgBuffer, sizeof(msgBuffer), msg, expected, given);
	js_error(msgBuffer, arg);
}
