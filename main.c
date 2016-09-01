
#include <stdlib.h>
#include "jscheme.h"
#include <setjmp.h>

static int prompt_enabled = 1;
static jmp_buf whereEverythingWasFine;

void
setupInitialEnvironment(){	

	globalEnvironmentPut(symbolTableGetOrAdd("a"), newInteger(10));
	globalEnvironmentPut(symbolTableGetOrAdd("b"), newInteger(20));
	
	// builtin functions
	globalEnvironmentPut(symbolTableGetOrAdd("+"), newBuiltinFunction("+", builtin_plus));
	globalEnvironmentPut(symbolTableGetOrAdd("-"), newBuiltinFunction("-", builtin_minus));
	globalEnvironmentPut(symbolTableGetOrAdd("*"), newBuiltinFunction("*", builtin_times));
	globalEnvironmentPut(symbolTableGetOrAdd("/"), newBuiltinFunction("/", builtin_quotient));
	globalEnvironmentPut(symbolTableGetOrAdd("eq?"), newBuiltinFunction("eq?", builtin_eqP));
	globalEnvironmentPut(symbolTableGetOrAdd("cons?"), newBuiltinFunction("cons?", builtin_consP));
	globalEnvironmentPut(symbolTableGetOrAdd("car"), newBuiltinFunction("car", builtin_car));
	globalEnvironmentPut(symbolTableGetOrAdd("cdr"), newBuiltinFunction("cdr", builtin_cdr));
	globalEnvironmentPut(symbolTableGetOrAdd("cons"), newBuiltinFunction("cons", builtin_cons));

	// builtin syntax

	globalEnvironmentPut(symbolTableGetOrAdd("define"), newBuiltinSyntax("define", builtin_define));
	globalEnvironmentPut(symbolTableGetOrAdd("if"), newBuiltinSyntax("if", builtin_if));
	globalEnvironmentPut(symbolTableGetOrAdd("lambda"), newBuiltinSyntax("lambda", builtin_lambda));
	globalEnvironmentPut(symbolTableGetOrAdd("quote"), newBuiltinSyntax("quote", builtin_quote));
	
}

void
setupInitialEnvironmentCP(){
	
	// CP builtin syntax
	globalEnvironmentPut(symbolTableGetOrAdd("define"), CP_newBuiltinSyntax("define", (VOIDPTRFUNC)CP_builtin_define));
	//globalEnvironmentPut(symbolTableGetOrAdd("if"), newBuiltinSyntax("if", builtin_if));
	//globalEnvironmentPut(symbolTableGetOrAdd("lambda"), newBuiltinSyntax("lambda", builtin_lambda));
	//globalEnvironmentPut(symbolTableGetOrAdd("quote"), newBuiltinSyntax("quote", builtin_quote));

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

	js_sym_define = symbolTableGetOrAdd("define");
	js_sym_lambda = symbolTableGetOrAdd("lambda");
}

/*
 *	jREPL: normal read evaluate print loop
 */

void
jREPL(OBJ input){
	/*
	* Jojo Scheme REPL 
	*/
	for(;;){
		
		OBJ expr, result;
		if(prompt_enabled) printf(CYN "JS> " RESET);
	
		expr = js_read(input);				// R ead
		result = js_eval(globalEnvironment, expr);	// E valuate
		js_print(stdout, result);			// P rint
								// L oop
		printf("\n");
	}
}

/*
 *	CP_jREPL: read evaluate print loop using continuation passing
 */
OBJ
enterTrampoline1(OBJ input){
	
	//fprintf(stdout, "\nStack before Trampoline:\n");
	printJStack(__FILE__,__LINE__,__FUNCTION__);
	VOIDPTRFUNC CP_jREPL();

	PUSH((OBJ)CP_jREPL);
	PUSH(NULL);	// last continuation
	AP = SP;	
	PUSH(input);
	VOIDPTRFUNC CP_jREPL();

	trampoline((VOIDPTRFUNC)CP_jREPL);

	return js_nil;
}

VOIDPTRFUNC
CP_jREPL(){
	
	//fprintf(stdout, "\nStack in CP_jREPL:\n");
	printJStack(__FILE__,__LINE__,__FUNCTION__);

	fprintf(stdout, "jREPL\n");
	OBJ inputStream = ARG(0);

	OBJ expr;
	VOIDPTRFUNC CP_jREPL2();

	if(prompt_enabled) printf(CYN "JS> " RESET);
	expr = js_read(inputStream);				// R ead
	CALL2( CP_js_eval,globalEnvironment, expr, CP_jREPL2);	// E val
}

VOIDPTRFUNC
CP_jREPL2(){
	
	//fprintf(stdout, "\nStack in CP_jREPL2:\n");
	printJStack(__FILE__,__LINE__,__FUNCTION__);

	OBJ result = RETVAL;
	fprintf(stdout, "jREPL2\n");
	js_print(stdout, result);			// P rint
							// L oop
	printf("\n");

	OBJ inputStream = ARG(0);
	TAILCALL1((VOIDPTRFUNC)CP_jREPL, inputStream);	
}

int
main() {

	initSymbolTable();
	initializeWellKnownObjects();
	initJStack();
#ifdef DEBUG
	initDebugOptions();	
	initGlobalEnvironment();
	setupInitialEnvironment();
	selftest();
#endif
	initGlobalEnvironment();
	setupInitialEnvironment();

	setupInitialEnvironmentCP();

	printf("Welcome to (JS)cheme\n");

	if( setjmp(whereEverythingWasFine) ){
#ifdef DEBUG
		indentLevel = 0;
#endif
		// reset Stack index
		SP = 0;
		AP = 0;
		BP = 0;
		printf("back in wonderland\n");
	}	
	printf("start REPL...\n");
	OBJ inputStream = newFileStream(stdin);

	//jREPL(inputStream);
	enterTrampoline1(inputStream);

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

void
trampoline( VOIDPTRFUNC funcToCall){
	
	fprintf(stdout, "Enter trampoline...\n");
	void *nextFunc;
	
	while( funcToCall != NULL ){
		nextFunc = funcToCall();
		funcToCall = (VOIDPTRFUNC)(nextFunc);
	}

	fprintf(stdout, "Leave trampoline...\n");
}
