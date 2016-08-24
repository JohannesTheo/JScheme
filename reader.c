#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jscheme.h"


int
isWhiteSpace(char ch){

	return( (ch == ' ') || (ch == '\n') || (ch == '\t') || (ch == '\r'));

}

static char
nextChar(OBJ inStream){


	if(inStream->u.any.tag == T_FILESTREAM){
		// this will sometimes read one char ahead so we will need a peekChar or something...
		char ch = inStream->u.fileStream.peekChar;

		if(ch != 0){
			inStream->u.fileStream.peekChar = 0;
			return ch;
		}

		ch = fgetc(inStream->u.fileStream.file);
		return ch;
	}
	else if(inStream->u.any.tag == T_STRINGSTREAM){
		
		int index = inStream->u.stringStream.index;
		char ch = inStream->u.stringStream.buffer[index];
		//printf("\nnextChar: %c at index %d, newIndex: %d", ch, index, index+1);

		inStream->u.stringStream.index++;
		
		if(ch == 0){
			//printf(RED "\nEOF\n" RESET);
			return EOF;
		}
		return ch;
	}

	return -1;
}

static void
unreadChar(OBJ inStream, char ch){
	
	if(inStream->u.any.tag == T_FILESTREAM){

		inStream->u.fileStream.peekChar = ch;
	}
	if(inStream->u.any.tag == T_STRINGSTREAM){
	
		inStream->u.stringStream.index--;	
		///printf("\nunreadChar: %c newIndex: %i",ch, inStream->u.stringStream.index);
	}
}

static char
skipWhiteSpace(OBJ inStream){

	char ch;

	do{
		ch = nextChar(inStream);
		//printf("skip:%i>", ch);
	} while (isWhiteSpace(ch));

	return ch;
}

int
thisIsTheEnd(OBJ inStream){
	/*
	 *	This is the end
	 *	Beautiful friend
	 *	This is the end
	 *	My only friend, the end
	 */
	int end = 0;
	char ch;

	do{
		//printf(CYN "\nendSearch: " RESET);
		ch = nextChar(inStream);
		if(ch == '\n' || ch == EOF){
			//printf(RED "<END>" RESET);
			end = 1;
			break;
		}
	} while (isWhiteSpace(ch));

	unreadChar(inStream, ch);
	return end;
}

static int
isDigit(char ch) {
	return (ch >= '0') && (ch <= '9');
}

static OBJ
readList(OBJ inStream){
	
	int ch;
	OBJ car, cdr;
	
	ch = skipWhiteSpace(inStream);
	if( ch == ')'){
		return js_nil;
	}
	unreadChar(inStream, ch);

	car = js_read(inStream);
	cdr = readList(inStream);
	return newCons(car, cdr);
}


OBJ
readNumber(OBJ inStream, char firstChar, int isNegative){
	//printf(YEL "\nreadNumber>" RESET);

	/*
 	* TO-DO refactor!	
 	*/	
	jscheme_int64 *iVal = NULL;
	char ch;
	OBJ retVal;

	// substract the ASCII value of '0' from char to get the actual value between 0 and 9.
	jscheme_int64 start = (jscheme_int64) firstChar - '0';
	if(isNegative) start *= -1;

	iVal = &start;
	while( isDigit( ch = nextChar(inStream) )){
		//iVal = iVal * 10 + ( (int)ch - '0');

		jscheme_int64 ch_val = (jscheme_int64) ch - '0';
		
		if(isNegative){
			int mul = __builtin_smull_overflow(*iVal, 10, iVal);
			int sub = __builtin_ssubl_overflow(*iVal, ch_val, iVal);
			if( mul || sub ){
				if(thisIsTheEnd(inStream)){
					prompt_on();
				}
				js_error("readNumber: integer underflow", newInteger(*iVal));
			}
		}else{
			int mul = __builtin_smull_overflow(*iVal, 10, iVal);
			int add = __builtin_saddl_overflow(*iVal, ch_val, iVal);
			if( mul || add ){
				if(thisIsTheEnd(inStream)){
					prompt_on();
				}
				js_error("readNumber: integer overflow", newInteger(*iVal));
			}
		
		}
	}
	unreadChar(inStream, ch);
	retVal = newInteger( *iVal );
	return retVal;
}

OBJ
readSymbol(OBJ inStream, char firstChar){
	//printf(YEL "\nreadSymbol>" RESET);

	char ch;
	char *buffer = NULL;
	int buffer_size = 64;
	int current_size = 0;
	//OBJ retVal;

	buffer = malloc(buffer_size);
	buffer[current_size++] = firstChar;

	ch = nextChar(inStream);
	while( !isWhiteSpace(ch) &&
		(ch != '(') &&
		(ch != ')') &&
		(ch != '"') &&
		(ch != EOF)){

		buffer[current_size++] = ch;

		if(current_size == buffer_size){
			int new_buffer_size = buffer_size * 2;
			buffer = realloc(buffer, new_buffer_size);
			buffer_size = new_buffer_size;
		}
		ch = nextChar(inStream);
	}
		
	unreadChar(inStream, ch);

	if(strcmp(buffer, "nil") == 0 ){
		free(buffer);
		return js_nil;
	}
	if(strcmp(buffer, "#t") == 0 ){
		free(buffer);
		return js_true;
	}
	if(strcmp(buffer, "#f") == 0 ){
		free(buffer);
		return js_false;
	}

	// realloc buffer to save memory and add string terminator.
	buffer = realloc(buffer, current_size + 1);
	buffer[current_size] = '\0';	
	
	return symbolTableGetOrAdd(buffer);	
	//retVal = newSymbol(buffer);
	//return retVal;
}

OBJ
readString(OBJ inStream){
	//printf(YEL "\nreadString>" RESET);

	char ch;
	OBJ retString;

	char *buffer = NULL;
	int buffer_size = 64;
	int current_size = 0;

	buffer = malloc(buffer_size);
	ch = nextChar(inStream);

	while( ch != '"' && ch != EOF ){
		if(ch == '\\'){
			ch = nextChar(inStream);
			switch(ch){
				case EOF:
					printf("Error: <unterminated string>");
					break;
				case 'n':
					ch = '\n';
					break;
				case 'r':
					ch = '\r';
					break;
				case 't':
					ch = '\t';
					break;
				default:
					break;
			}
		}

		buffer[current_size++] = ch;

		if(current_size == buffer_size){
			int new_buffer_size = buffer_size * 2;
			buffer = realloc(buffer, new_buffer_size);
			buffer_size = new_buffer_size;
		}
		ch = nextChar(inStream);
	}


	// realloc buffer to save memory and add string terminator.
	buffer = realloc(buffer, current_size + 1);
	buffer[current_size] = '\0';

	retString = newString(buffer);
	return retString;
}

OBJ
js_read(OBJ inStream){

	OBJ retVal;
	prompt_off();
	char ch = skipWhiteSpace(inStream);
	
	if(ch == '('){
		retVal = readList(inStream);
	}
	else if(ch =='\''){
		OBJ expr = js_read(inStream);

		// (quote (expr* (nil))) -> expr must be a cons
		return newCons(symbolTableGetOrAdd("quote"), 
				newCons(expr, js_nil));
	}
	else if(ch == '"'){
		retVal = readString(inStream);
	}
	else if(isDigit(ch)){
		retVal = readNumber(inStream, ch, 0);
	
	}else if(ch == '-'){
		/*
		 * TO-DO refactor: implement proper read ahead solution
		 */
		// simple read ahead to catch negative numbers
		char nextCh = nextChar(inStream);
		if(isDigit(nextCh)){
			retVal = readNumber(inStream, nextCh, 1);	
		}else{
			unreadChar(inStream, nextCh);
			retVal = readSymbol(inStream, ch);
		}
	}
#ifdef DEBUG
	else if(ch == '%'){
		ch = nextChar(inStream);
		OBJ debugOption = readSymbol(inStream, ch);
		switchDebugOptions( debugOption );
		retVal = js_void;
	}
#endif
	else {
		retVal = readSymbol(inStream, ch);
	}
	
	if(thisIsTheEnd(inStream)){
		prompt_on();
	};

	return retVal;
}

