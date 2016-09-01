#include "jscheme.h"

VOIDPTRFUNC
CP_js_eval(){

//	fprintf(stdout, "\nStack in CP_js_eval:\n");
	printJStack(__FILE__,__LINE__,__FUNCTION__);

	OBJ env = ARG(0);
	OBJ expr = ARG(1);	
	VOIDPTRFUNC CP_evalCons();

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
			RETURN(expr);

		case T_SYMBOL:
			//printf("Address of symbol: %p\n", exp	
			value = environmentGet(env, expr);
			if(value == NULL){
				js_error("undefined variable", expr);
			}
		
			RETURN(value);
		case T_CONS:
			TAILCALL2(CP_evalCons, env, expr);
	}
}

VOIDPTRFUNC
CP_evalCons(){
	

	OBJ env = ARG(0);
	OBJ expr = ARG(1);
	OBJ functionSlot = CAR(expr);
	VOIDPTRFUNC CP_evalCons2();

	/* 
	 * Prepare some LOCALs before evaluating the function slot
	 *
	 *	LOCAL(2): numArgs
	 *	LOCAL(1): restArgs
	 *	LOCAL(0): evaluated function slot
	 *
	 */
	CREATE_LOCALS(3);
	
	printJStack(__FILE__,__LINE__,__FUNCTION__);
	CALL2(CP_js_eval, env, functionSlot, CP_evalCons2);	
}

/*
 *	A function slot has been evaluated
 */
VOIDPTRFUNC
CP_evalCons2(){
	

	OBJ evaluatedFunctionSlot = RETVAL;
	OBJ env = ARG(0);
	OBJ expr = ARG(1);
	OBJ restArgs = CDR(CDR(expr));
	int numArgs = 0;

	SET_LOCAL(0, evaluatedFunctionSlot);
	SET_LOCAL(1, restArgs);
	SET_LOCAL(2, (OBJ)((INT) numArgs));
	
	VOIDPTRFUNC CP_evalCons3();

	printJStack(__FILE__,__LINE__,__FUNCTION__);
	switch( TAG(evaluatedFunctionSlot) ){

		case T_BUILTINFUNCTION:
		{
			//error("cur shit", __FILE__,__LINE__);
			OBJ firstArg = CAR(CDR(expr));
			CALL2(CP_js_eval, env, firstArg, CP_evalCons3); 
		
		/*	int numArgs = 0;
			//restArgs = argList;
			while( restArgs != js_nil) {
				OBJ theArg = CAR(restArgs);
				restArgs = CDR(restArgs);

				CALL2(CP_js_eval, env, theArg, CP_evalCons3);

				OBJ evaluatedArg = js_eval(env, theArg);
				PUSH(evaluatedArg);
				numArgs++;
			}
			// All args need to be on Stack...
			OBJ value = evaluatedFunctionSlot->u.builtinFunction.theCode(numArgs);
			RETURN(value);
			*/
		}

		case T_BUILTINSYNTAX:
		{
			error("T_BULTINSYNTAX unimpl.", __FILE__, __LINE__);
			//return evaluatedFunctionSlot->u.builtinSyntax.theCode(env, argList);
		}

		case T_USERDEFINEDFUNCTION:
		{
			error("T_USERDEFINEDFUNCTION unimpl.", __FILE__, __LINE__);
			/*
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

			while( restFormalArgs != js_nil){
			
				// expected formal arguments
				OBJ nextFormalArg = CAR(restFormalArgs);
				restFormalArgs = CDR(restFormalArgs);
				
				// given arguments
				OBJ unevaluatedArg = CAR(restArgs);
				restArgs = CDR(restArgs);
				
				// put evaluated arg in new env
				OBJ evaluatedArg = js_eval(env, unevaluatedArg);
				environmentPut(newEnv, nextFormalArg, evaluatedArg);
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
			*/
		}
		default:
			js_error("Eval: not a function: ", evaluatedFunctionSlot);
	}
	// NOT REACHED
	return js_nil;
	
}

VOIDPTRFUNC
CP_evalCons3(){
	

	OBJ evaluatedArg = RETVAL;
	PUSH(evaluatedArg);

	OBJ env = ARG(0);
	OBJ restArgs = LOCAL(1);
	int numArgs = (int) LOCAL(2);
	numArgs++;

	if( restArgs != js_nil){
	
		OBJ nextArg = CAR(restArgs);
		restArgs = CDR(restArgs);
		SET_LOCAL(1, restArgs);
		SET_LOCAL(2, (OBJ)((INT)numArgs));

		printJStack(__FILE__,__LINE__,__FUNCTION__);	
		CALL2(CP_js_eval, env, nextArg, CP_evalCons3);	
	}

	SET_LOCAL(2, (OBJ)((INT)numArgs));
	printJStack(__FILE__,__LINE__,__FUNCTION__);	

	OBJ function = LOCAL(0);

	OBJ value = function->u.builtinFunction.theCode(numArgs);
	RETURN(value);

	error("unimpl.", __FILE__, __LINE__);
	//OBJ function = 
	//OBJ value = 
	

	/*
	switch( TAG(evaluatedFunctionSlot) ){

		case T_BUILTINFUNCTION:
		{

			error("T_BULTINFUNCTION unimpl.", __FILE__, __LINE__);
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
			error("T_BULTINSYNTAX unimpl.", __FILE__, __LINE__);
			//return evaluatedFunctionSlot->u.builtinSyntax.theCode(env, argList);
		}

		}
		default:
			js_error("Eval: not a function: ", evaluatedFunctionSlot);
	}
	// NOT REACHED
	return js_nil;
	*/
}
