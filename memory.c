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
	fileStream->peekChar = 0;

	return (OBJ)fileStream;
}

OBJ
newStringStream(char *buffer){

	struct jschemeStringStream *stringStream;

	stringStream = (struct jschemeStringStream*)(malloc( sizeof( struct jschemeStringStream)));
	stringStream->tag = T_STRINGSTREAM;
	stringStream->buffer = buffer;
	stringStream->index = 0;

	return (OBJ)stringStream;
}

OBJ
newSymbol(char *chars){

	struct jschemeSymbol *theSymbol;

	theSymbol = (struct jschemeSymbol*)(malloc( sizeof( struct jschemeSymbol)));
	theSymbol->tag = T_SYMBOL;
	theSymbol->symbolVal = chars;

	return (OBJ)theSymbol;
}

OBJ
newString(char *chars){

	struct jschemeString *theString;

	theString = (struct jschemeString*)(malloc( sizeof( struct jschemeString)));
	theString->tag = T_STRING;
	theString->stringVal = chars;

	return (OBJ)theString;
}


const char* tag_lookup[8] = {
	"T_NIL",
	"T_TURE",
	"T_FALSE",
	"T_INTEGER",
	"T_SYMBOL",
	"T_STRING",
	"T_FILESTREAM",
	"T_STRINGSTREAM"
};
