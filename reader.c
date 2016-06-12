#include <stdio.h>
#include <stdlib.h>
#include "jscheme.h"

int
isWhiteSpace(char ch){
	
	return( (ch == ' ') || (ch == '\n') || (ch == '\t') || (ch == '\r'));

}

char
skipWhiteSpace(){

	char ch;
       
	do{
		ch = fgetc(stdin);
	} while (isWhiteSpace(ch));

	return ch;
}

int
isDigit(char ch) {
	return (ch >= '0') && (ch <= '9'); 
}


OBJ
js_read(){

	char ch = skipWhiteSpace();
	
	if(isDigit(ch)){
		return readNumber(ch);
	}
	printf("#<unimpl. read> %c\n", ch);
	exit(-1);
}


OBJ
readNumber(char firstChar){

	jscheme_int64 iVal = 0;
	char ch;
	OBJ retVal;

	// substract the ASCII value of '0' from char to get the actual value between 0 and 9. 
	iVal = (int)firstChar - '0';

	while( isDigit( ch = fgetc(stdin))){
		iVal = iVal * 10 + ( (int)ch - '0');
	}
	
	retVal = newInteger( iVal );
	return retVal;
}

