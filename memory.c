#include <stdlib.h>
#include <stdio.h>
#include "jscheme.h"

OBJ
newInteger(jscheme_int64 iVal){

	struct jschemeInteger *theJSInt;

	theJSInt = (struct jschemeInteger*)(malloc( sizeof( struct jschemeInteger)));
	theJSInt->tag = T_INTEGER;
	theJSInt->intVal = iVal;
	return (OBJ) theJSInt;
}

OBJ
newFileStream(FILE* file){

	struct jschemeFileStream *fileStream;

	fileStream = (struct jschemeFileStream*)(malloc( sizeof( struct jschemeFileStream)));
	fileStream->tag = T_FILESTREAM;
	fileStream->file = file;

	return (OBJ)fileStream;
}
