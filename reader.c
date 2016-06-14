#include <stdio.h>
#include <stdlib.h>
#include "jscheme.h"


int
isWhiteSpace(char ch){
	
	return( (ch == ' ') || (ch == '\n') || (ch == '\t') || (ch == '\r'));

}

static char
nextChar(OBJ inStream){

	// this will sometimes read one char ahead so we will need a peekChar or something...
	char ch;
	ch = fgetc(inStream->u.fileStream.file);
	return ch;
}

char
skipWhiteSpace(OBJ inStream){

	char ch;
       
	do{
		ch = nextChar(inStream);
	} while (isWhiteSpace(ch));

	return ch;
}

int
isDigit(char ch) {
	return (ch >= '0') && (ch <= '9'); 
}

OBJ
readNumber(OBJ inStream, char firstChar){

	jscheme_int64 iVal = 0;
	char ch;
	OBJ retVal;

	// substract the ASCII value of '0' from char to get the actual value between 0 and 9. 
	iVal = (int)firstChar - '0';

	while( isDigit( ch = nextChar(inStream) )){
		iVal = iVal * 10 + ( (int)ch - '0');
	}
	
	retVal = newInteger( iVal );
	return retVal;
}

OBJ
js_read(OBJ inStream){

	char ch = skipWhiteSpace(inStream);
	
	if(isDigit(ch)){
		return readNumber(inStream, ch);
	}
	printf("#<unimpl. read> %c\n", ch);
	exit(-1);
}
