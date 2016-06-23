#include <stdlib.h>
#include <stdio.h>
#include "jscheme.h"

void
error(char* msg, char* fileName, int line){
	fprintf(stderr, "%s [%d]: %s\n", fileName, line, msg);
	abort();
}

