#include "jscheme.h"
#include <stdlib.h>

#define INITIAL_STACK_SIZE 128

OBJ *evalStack;
int spIndex = 0;
int stackLimit;

void
initEvalStack(){

	stackLimit = INITIAL_STACK_SIZE;
	evalStack = (OBJ *)(malloc( sizeof(OBJ) * INITIAL_STACK_SIZE));
	spIndex = 0;
}

static OBJ
evalCons(OBJ env, OBJ expr){
	
	OBJ functionSlot = CAR(expr);
	OBJ evaluatedFunctionSlot = js_eval(env, functionSlot);
	OBJ argList = CDR(expr);
	OBJ restArgs;

	switch( TAG(evaluatedFunctionSlot) ){

		case T_BUILTINFUNCTION:
		{

			int numArgs = 0;
			restArgs = argList;
			while( restArgs != js_nil) {
				OBJ theArg = CAR(restArgs);
				OBJ evaluatedArg;
				restArgs = CDR(restArgs);

				evaluatedArg = js_eval(env, theArg);
				PUSH(evaluatedArg);
				numArgs++;
			}
			return evaluatedFunctionSlot->u.builtinFunction.theCode(numArgs);
		}

		case T_BUILTINSYNTAX:
		{
			int numArgs = 0;
			restArgs = argList;
			while( restArgs != js_nil) {
				OBJ theArg = CAR(restArgs);
				restArgs = CDR(restArgs);

				PUSH(theArg);	// NOT evaluated
				numArgs++;
			}
			return evaluatedFunctionSlot->u.builtinSyntax.theCode(numArgs, env, argList);

		}
		default:
			js_error("Eval: not a function: ", evaluatedFunctionSlot);
	}
	// NOT REACHED
	return js_nil;
}

OBJ
js_eval(OBJ env, OBJ expr){

	switch(TAG(expr)){
		default:
			// T_INTEGER
			// T_NIL
			// T_TRUE
			// T_FALSE
			// T_STRING
			// T_FILESTREAM
			// T_STRINGSTREAM
			return expr;

		case T_SYMBOL:
			printf("Address of symbol: %p\n", expr); 
			
			OBJ value = environmentGet(expr);
			if(value == NULL){
				js_error("undefined variable", expr);
			}
			return value;
		case T_CONS:
			return evalCons(env, expr);
	}
}
