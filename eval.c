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

		case T_USERDEFINEDFUNCTION:
		{
			// 1. check if the udfs expected numArgs is equal to the given args
			// 2. put the given args in a new env and bind them to the expected formal args
			// 3. eval every body in bodylist in the new env
			// 4. return last bodies value
			//
			// given:
			// 	evaluatedFunctionslot -> theUDF
			// 	argList -> the non formal args passed to the udf

			// this is can be an overhead since we dont expect that case to happen often
			// -> refactor to checking args number while building env
			int numFormalArgs = evaluatedFunctionSlot->u.userDefinedFunction.numArgs;
			int numArgs = length(argList);

			if( numFormalArgs != numArgs){
				js_function_error("(lambda): function expects %d args given %d", evaluatedFunctionSlot, numFormalArgs, numArgs);
			}

			// fill new environment
			OBJ newEnv = newEnvironment(numFormalArgs, globalEnvironment);
			OBJ restFormalArgs = evaluatedFunctionSlot->u.userDefinedFunction.argList;
			restArgs = argList;
			//int slotIndex = 0;

			while( restFormalArgs != js_nil){
			
				// expected formal arguments
				OBJ nextFormalArg = CAR(restFormalArgs);
				restFormalArgs = CDR(restFormalArgs);
				
				// given arguments
				OBJ unevaluatedArg = CAR(restArgs);
				restArgs = CDR(restArgs);
				
				// put evaluated arg in new env
				OBJ evaluatedArg = js_eval(env, unevaluatedArg);
				environmentPut(env, nextFormalArg, evaluatedArg);
				//newEnv->u.environment.slots[slotIndex].key = nextFormalArg;
				//newEnv->u.environment.slots[slotIndex].value = evaluatedArg;
				//slotIndex++;	
			}

			// eval bodylist
			OBJ restBodyList = evaluatedFunctionSlot->u.userDefinedFunction.bodyList;
			OBJ lastValue;
			while( restBodyList != js_nil) {

				// bodies to evaluate
				OBJ nextBody = CAR(restBodyList);
				restBodyList = CDR(restBodyList);

				lastValue = js_eval(newEnv, nextBody);
			}
			return lastValue;
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
			
			OBJ value = environmentGet(env, expr);
			if(value == NULL){
				js_error("undefined variable", expr);
			}
			return value;
		case T_CONS:
			return evalCons(env, expr);
	}
}
