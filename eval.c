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
#ifdef DEBUG
// PRINT TRACE
if( EVAL_TRACE->state){
		printIndent(indentLevel);
		fprintf(stdout, MAG "ARGS FOR BUILTINFUNC\n" RESET);
}
#endif
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
			return evaluatedFunctionSlot->u.builtinSyntax.theCode(env, argList);
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
			int numLocals = evaluatedFunctionSlot->u.userDefinedFunction.numLocals;
			OBJ home = evaluatedFunctionSlot->u.userDefinedFunction.home;

			if( numFormalArgs != numArgs){
				js_function_error("(lambda): function expects %d args given %d", evaluatedFunctionSlot, numFormalArgs, numArgs);
			}

			// fill new environment
			OBJ newEnv = newEnvironment((numFormalArgs + numLocals), home);
			OBJ restFormalArgs = evaluatedFunctionSlot->u.userDefinedFunction.argList;
			restArgs = argList;
			//int slotIndex = 0;

#ifdef DEBUG
// PRINT TRACE
if( EVAL_TRACE->state){
			printIndent(indentLevel);
			fprintf(stdout, RED"EVAL UDF"RESET);
			fprintf(stdout, "\n");
			printIndent(indentLevel);
			fprintf(stdout, MAG "PREPARE NEW ENV" YEL " (%p)" RESET, newEnv);
			fprintf(stdout, " numFArgs: %d numLocals: %d\n", numFormalArgs, numLocals);
}
#endif
			while( restFormalArgs != js_nil){
			
				// expected formal arguments
				OBJ nextFormalArg = CAR(restFormalArgs);
				restFormalArgs = CDR(restFormalArgs);
				
				// given arguments
				OBJ unevaluatedArg = CAR(restArgs);
				restArgs = CDR(restArgs);
#ifdef DEBUG				
// PRINT TRACE
if( EVAL_TRACE->state){
				printIndent(indentLevel);
				fprintf(stdout, "binding " GRN); js_print(stdout, nextFormalArg);
				fprintf(stdout, RESET " to -> ");
				oldIndentLevel = indentLevel;
				indentLevel = 0;
				if( ISCONS(unevaluatedArg)) PAUSE_INDENT_FLAG = 1;
}
#endif
				
				// put evaluated arg in new env
				OBJ evaluatedArg = js_eval(env, unevaluatedArg);
				environmentPut(newEnv, nextFormalArg, evaluatedArg);
				//newEnv->u.environment.slots[slotIndex].key = nextFormalArg;
				//newEnv->u.environment.slots[slotIndex].value = evaluatedArg;
				//slotIndex++;	

#ifdef DEBUG				
// PRINT TRACE
if( EVAL_TRACE->state){
				indentLevel = oldIndentLevel;
				oldIndentLevel = 0;
}
#endif
			}
			// eval bodylist
			OBJ restBodyList = evaluatedFunctionSlot->u.userDefinedFunction.bodyList;	

#ifdef DEBUG
			int numBodies = length(restBodyList);
			int currentBody = 0;
#endif

			OBJ lastValue;
			while( restBodyList != js_nil) {

				// bodies to evaluate
				OBJ nextBody = CAR(restBodyList);
				restBodyList = CDR(restBodyList);
#ifdef DEBUG				
// PRINT TRACE
if( EVAL_TRACE->state){
				fprintf(stdout, "\n");
				printIndent(indentLevel);
				fprintf(stdout, MAG "EVAL BODY %d of %d\n" RESET, ++currentBody, numBodies);
}
#endif

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

#ifdef DEBUG
// PRINT TRACE
if( EVAL_TRACE->state){
	printIndent(indentLevel);
	if( TAG(env) == T_GLOBALENVIRONMENT){
		printf("eval in " CYN "GLOBAL" RESET " (%p): ", env);
	}else{
		printf("eval in " YEL "LOCAL" RESET " (%p): ", env);
	}
	js_print(stdout, expr);
	
	if(PAUSE_INDENT_FLAG){
		indentLevel += oldIndentLevel;
		PAUSE_INDENT_FLAG = 0;
	}
}
#endif

	OBJ value;
	switch(TAG(expr)){
		default:
			// T_INTEGER
			// T_NIL
			// T_TRUE
			// T_FALSE
			// T_STRING
			// T_FILESTREAM
			// T_STRINGSTREAM
#ifdef DEBUG
// PRINT TRACE
if( EVAL_TRACE->state){
			fprintf(stdout, " -> " GRN); 
			js_print(stdout, expr);
			fprintf(stdout,"\n" RESET);
}
#endif
			return expr;

		case T_SYMBOL:
			//printf("Address of symbol: %p\n", exp	
			value = environmentGet(env, expr);
			if(value == NULL){
				js_error("undefined variable", expr);
			}
#ifdef DEBUG
// PRINT TRACE
if( EVAL_TRACE->state){
		fprintf(stdout, " -> " GRN); 
		js_print(stdout, value);
		fprintf(stdout, "\n" RESET);
}
#endif
		
			return value;
		case T_CONS:
#ifdef DEBUG
// PRINT TRACE
if( EVAL_TRACE->state){
			indentLevel++;
			fprintf(stdout,"\n");

			if( (TAG(env) == T_LOCALENVIRONMENT) && 
			    (!ISCONS( CAR(expr))) &&
			    (!isDefine(expr)) &&
			    (!isLambda(expr))
			  )
			{
				
				fprintf(stdout,"\n");
				printIndent(indentLevel);
				fprintf(stdout,"ARGS in");
				printLocalEnv(env);
			}
}
				value = evalCons(env, expr);
			
if( EVAL_TRACE->state){
			printIndent(indentLevel);
			fprintf(stdout, "<< ");
			js_print(stdout, value);
			fprintf(stdout, "\n");
			indentLevel--;
}
#else
			value = evalCons(env, expr);
#endif
			return value;
			
	}
}
