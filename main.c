#include <stdio.h>
#include <stdlib.h>
#include "jscheme.h"
#include <setjmp.h>

static int prompt_enabled = 1;
static jmp_buf whereEverythingWasFine;


void
setupInitialEnvironment(){	

	environmentPut(symbolTableGetOrAdd("a"), newInteger(10));
	environmentPut(symbolTableGetOrAdd("b"), newInteger(20));
	
	// builtin functions
	environmentPut(symbolTableGetOrAdd("+"), newBuiltinFunction("+", builtin_plus));
	environmentPut(symbolTableGetOrAdd("-"), newBuiltinFunction("-", builtin_minus));
	environmentPut(symbolTableGetOrAdd("*"), newBuiltinFunction("*", builtin_times));
	environmentPut(symbolTableGetOrAdd("/"), newBuiltinFunction("/", builtin_quotient));
	environmentPut(symbolTableGetOrAdd("eq?"), newBuiltinFunction("eq?", builtin_eqP));
	environmentPut(symbolTableGetOrAdd("cons?"), newBuiltinFunction("cons?", builtin_consP));
	environmentPut(symbolTableGetOrAdd("car"), newBuiltinFunction("car", builtin_car));
	environmentPut(symbolTableGetOrAdd("cdr"), newBuiltinFunction("cdr", builtin_cdr));
	environmentPut(symbolTableGetOrAdd("cons"), newBuiltinFunction("cons", builtin_cons));
	
	// builtin syntax
	environmentPut(symbolTableGetOrAdd("define"), newBuiltinSyntax("define", builtin_define));

}

void 
initializeWellKnownObjects(){

	js_nil = (OBJ)(malloc ( sizeof( struct jschemeAny)));
	js_nil->u.any.tag = T_NIL;
	
	js_true = (OBJ)(malloc( sizeof( struct jschemeAny)));
	js_true->u.any.tag = T_TRUE;
	
	js_false = (OBJ)(malloc( sizeof( struct jschemeAny)));
	js_false->u.any.tag = T_FALSE;

	js_void = (OBJ)(malloc( sizeof( struct jschemeAny)));
	js_void->u.any.tag = T_VOID;
}


int
main() {

	initializeWellKnownObjects();
	initEvalStack();
	initSymbolTable();
	initGlobalEnvironment();
	selftest();
	initGlobalEnvironment();

	setupInitialEnvironment();

	printf("Welcome to (JS)cheme\n");

	if( setjmp(whereEverythingWasFine) ){
		printf("back in wonderland\n");
	}	
	printf("start REPL...\n");

	OBJ input = newFileStream(stdin);
	/*
	 * Jojo Scheme REPL 
	 */
	for(;;){
		
		OBJ expr, result;
		if(prompt_enabled) printf(CYN "JS> " RESET);
	
		expr = js_read(input);		// R ead
		result = js_eval(expr);		// E valuate
		js_print(stdout, result);	// P rint
						// L oop

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

