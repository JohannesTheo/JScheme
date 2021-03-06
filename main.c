
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
	
	globalEnvironmentPut(symbolTableGetOrAdd("include"), newBuiltinFunction("include", builtin_include));
	globalEnvironmentPut(symbolTableGetOrAdd(">"), newBuiltinFunction(">", builtin_gThanNrP));
	globalEnvironmentPut(symbolTableGetOrAdd("set-car!"), newBuiltinFunction("set-car!", builtin_set_car));
	globalEnvironmentPut(symbolTableGetOrAdd("set-cdr!"), newBuiltinFunction("set-cdr!", builtin_set_cdr));
	globalEnvironmentPut(symbolTableGetOrAdd("display"), newBuiltinFunction("display", builtin_display));
	globalEnvironmentPut(symbolTableGetOrAdd("write"), newBuiltinFunction("write", builtin_write));
	globalEnvironmentPut(symbolTableGetOrAdd("string=?"), newBuiltinFunction("string=?", builtin_eqStringP));
	
}
	
void
setupBuiltinSyntax()
{
	// builtin syntax
	globalEnvironmentPut(symbolTableGetOrAdd("define"), newBuiltinSyntax("define", builtin_define));
	globalEnvironmentPut(symbolTableGetOrAdd("if"), newBuiltinSyntax("if", builtin_if));
	globalEnvironmentPut(symbolTableGetOrAdd("lambda"), newBuiltinSyntax("lambda", builtin_lambda));
	globalEnvironmentPut(symbolTableGetOrAdd("quote"), newBuiltinSyntax("quote", builtin_quote));
	globalEnvironmentPut(symbolTableGetOrAdd("set!"), newBuiltinSyntax("set!", builtin_set));
}

void
CP_setupBuiltinSyntax()
{	
	// CP builtin syntax
	globalEnvironmentPut(symbolTableGetOrAdd("define"), CP_newBuiltinSyntax("define", (VOIDPTRFUNC)CP_builtin_define));
	globalEnvironmentPut(symbolTableGetOrAdd("if"), CP_newBuiltinSyntax("if", (VOIDPTRFUNC)CP_builtin_if));
	globalEnvironmentPut(symbolTableGetOrAdd("lambda"), CP_newBuiltinSyntax("lambda", (VOIDPTRFUNC)CP_builtin_lambda));
	globalEnvironmentPut(symbolTableGetOrAdd("quote"), CP_newBuiltinSyntax("quote", (VOIDPTRFUNC)CP_builtin_quote));
	globalEnvironmentPut(symbolTableGetOrAdd("set!"), CP_newBuiltinSyntax("set!", (VOIDPTRFUNC)CP_builtin_set));

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

	js_eof = (OBJ)(malloc( sizeof( struct jschemeAny)));
	js_eof->u.any.tag = T_EOF;

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
		if( result == js_eof) return;
		js_print(stdout, result, 1);			// P rint
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
	//printJStack(__FILE__,__LINE__,__FUNCTION__);
	DEBUGCODE(PRINT_STACK->state, printJStack(__FILE__,__LINE__,__FUNCTION__));
	VOIDPTRFUNC CP_jREPL();

	PUSH((OBJ)((INT)BP));
	PUSH((OBJ)((INT)AP));
	PUSH((OBJ)CP_jREPL);
	PUSH(NULL);	// last continuation
	AP = SP;
	BP = AP - 4;
	PUSH(input);
	VOIDPTRFUNC CP_jREPL();

	trampoline((VOIDPTRFUNC)CP_jREPL);

	return js_nil;
}

VOIDPTRFUNC
CP_jREPL(){
	
	//fprintf(stdout, "\nStack in CP_jREPL:\n");
	//printJStack(__FILE__,__LINE__,__FUNCTION__);
	DEBUGCODE(PRINT_STACK->state, printJStack(__FILE__,__LINE__,__FUNCTION__));

	OBJ inputStream = ARG(0);

	OBJ expr;
	VOIDPTRFUNC CP_jREPL2();

	if( SP > 5) prompt_off();
	if(prompt_enabled) printf(CYN "JS> " RESET);
	expr = js_read(inputStream);				// R ead
	if( expr == js_eof ) RETURN( js_void );
	CALL2( CP_js_eval,globalEnvironment, expr, CP_jREPL2);	// E val
}

VOIDPTRFUNC
CP_jREPL2(){
	
	//fprintf(stdout, "\nStack in CP_jREPL2:\n");
	//printJStack(__FILE__,__LINE__,__FUNCTION__);
	DEBUGCODE(PRINT_STACK->state, printJStack(__FILE__,__LINE__,__FUNCTION__));

	OBJ result = RETVAL;

#ifdef DEBUG
	if( (SP <= 5) || PRINT_INCLUDE->state){
		printIndent(indentLevelForInclude);
#else
	if( (SP <= 5)) {
#endif
		js_print(stdout, result, 1);		// P rint
		printf("\n");
	}
							// L oop
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
	setupBuiltinSyntax();
	selftest();
#endif
	
	printf("hello friend...\n");
	initGlobalEnvironment();
	setupInitialEnvironment();
	printf("Welcome to (JS)cheme\n");

	if( setjmp(whereEverythingWasFine) ){
#ifdef DEBUG
		indentLevel = 0;
		indentLevelForInclude = 0;
#endif
		// reset Stack index
		SP = 0;
		AP = 0;
		BP = 0;
		prompt_on();
		printf("back in wonderland\n");
	}	
	printf("...starting a REPL for you.\n");
	OBJ inputStream = newFileStream(stdin);
	
#ifdef DEBUG
	if(CONTINUATION_PASSING->state){
		CP_setupBuiltinSyntax();
		enterTrampoline1(inputStream);
	}else{
		setupBuiltinSyntax();
		jREPL(inputStream);
	}
#else 
	//jREPL(inputStream);
	CP_setupBuiltinSyntax();
	enterTrampoline1(inputStream);
#endif

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
	
#ifdef DEBUG
	if( (SP <= 5) || PRINT_INCLUDE->state){
		printIndent(indentLevelForInclude);
		fprintf(stdout, "Enter a magical trampoline.\n");
	}
#endif
	void *nextFunc;
	
	while( funcToCall != NULL ){
		nextFunc = funcToCall();
		funcToCall = (VOIDPTRFUNC)(nextFunc);
	}
#ifdef DEBUG
	if( (SP <= 5) || PRINT_INCLUDE->state){
		printIndent(indentLevelForInclude);
		fprintf(stdout, "Back in reality.\n");
	}
#endif
}
