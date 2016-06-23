#include <stdio.h>
#include <stdlib.h>
#include "jscheme.h"
#include <setjmp.h>

static int prompt_enabled = 1;
static jmp_buf whereEverythingWasFine;

void 
initializeWellKnownObjects(){

	js_nil = (OBJ)(malloc ( sizeof( struct jschemeAny)));
	js_nil->u.any.tag = T_NIL;
	
	js_true = (OBJ)(malloc( sizeof( struct jschemeAny)));
	js_true->u.any.tag = T_TRUE;
	
	js_false = (OBJ)(malloc( sizeof( struct jschemeAny)));
	js_false->u.any.tag = T_FALSE;
}


int
main() {

	initializeWellKnownObjects();
	selftest();

	printf("Welcome to (JS)cheme\n");

	setjmp(whereEverythingWasFine);
	printf("start REPL...\n");

	OBJ input = newFileStream(stdin);
	/*
	 * Jojo Scheme REPL 
	 */
	for(;;){
		
		OBJ expr, result;
//		printf("MAIN: %d :", prompt_enabled);
		if(prompt_enabled) printf(CYN "JS> " RESET);
	
		result = js_read(input);
		js_print(stdout, result);

		printf("\n");
	}
	return 0;
}

void
prompt_on(){
	prompt_enabled = 1;
}

void
prompt_off(){
	prompt_enabled = 0;
}

void
getMeOutOfHere(){
	longjmp(whereEverythingWasFine, 1);
}

