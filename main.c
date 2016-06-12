#include <stdio.h>
#include <stdlib.h>
#include "jscheme.h"

int
main() {

	printf("Welcome to (JS)cheme\n");

	/*
	 * Jojo Scheme REPL 
	 */
	for(;;){
		
		printf("JS> ");
	
		OBJ input = js_read();			
		js_print(input);
		
		printf("\n");
	}
	return 0;
}
