#include <stdio.h>
#include <stdlib.h>
#include "jscheme.h"

int
main() {

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
