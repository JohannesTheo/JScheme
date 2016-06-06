#include <stdio.h>
#include <stdlib.h>

int 
main() {

	printf("Welcome to (JS)cheme\n");

	for(;;){
		printf("JS> ");
		char input = getc(stdin);
		while(input != '\n'){
			printf("%c", input);
			input = getc(stdin);
		}
		printf("\n");
	}
	return 0;
}
