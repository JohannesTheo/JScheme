#include <stdio.h>
#include <stdlib.h>
#include "jscheme.h"

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

	printf("Welcome to (JS)cheme\n");

	OBJ input = newFileStream(stdin);
	/*
	 * Jojo Scheme REPL 
	 */
	for(;;){
		
		printf("JS> ");
	
		OBJ result = js_read(input);			
		js_print(result);
		
		printf("\n");
	}
	return 0;
}
