#include <stdio.h>
#include "jscheme.h"


int
isDigit(char ch) {
	return (ch >= '0') && (ch <= '9'); 
}


OBJ
js_read(){

	OBJ input = NULL;
	
	char ch = fgetc(stdin);

	while( !isDigit(ch) ){
		if( (ch != '\n') && (ch !=' ')){
			printf("<unimpl. read> %c\n", ch);
		}
		ch = fgetc(stdin);
	}

	input = readNumber(ch);
	return input;
}


OBJ
readNumber(char ch){

	OBJ retVal = newInteger(ch);

	return retVal;
}

