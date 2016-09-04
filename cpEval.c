#include "jscheme.h"

VOIDPTRFUNC
CP_js_eval(){

	printJStack(__FILE__,__LINE__,__FUNCTION__);

	OBJ env = ARG(0);
	OBJ expr = ARG(1);	
	VOIDPTRFUNC CP_evalCons();

	ASSERT(ISENV(env), "bad env");
	OBJ value;
	switch(TAG(expr)){
		default:
			// ALL TYPES BUT T_SYMBOL && T_CONS
			RETURN(expr);

		case T_SYMBOL:
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

	ASSERT(ISENV(env), "bad env");
	/* 
	 * Prepare some LOCALs before evaluating the function slot
	 *	
	 *	LOCAL(5): restBodyList	 (UDF)
	 *	LOCAL(4): newEnv	 (UDF)
	 *	LOCAL(3): restFormalArgs (UDF)
	 *	LOCAL(2): numArgs
	 *	LOCAL(1): restArgs
	 *	LOCAL(0): evaluated function slot
	 *
	 */
	CREATE_LOCALS(6);
	
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
	OBJ restArgs = CDR(expr);
	int numArgs = 0;
	
	ASSERT(ISENV(env), "bad env");
	SET_LOCAL(0, evaluatedFunctionSlot);
	SET_LOCAL(1, restArgs);
	SET_LOCAL(2, (OBJ)((INT) numArgs));
	
	VOIDPTRFUNC CP_evalCons3();

	printJStack(__FILE__,__LINE__,__FUNCTION__);
	switch( TAG(evaluatedFunctionSlot) ){

		case T_BUILTINFUNCTION:
		{
			if( restArgs != js_nil){
				OBJ firstArg = CAR(restArgs);
				CALL2(CP_js_eval, env, firstArg, CP_evalCons3); 
			}else{
				OBJ value = evaluatedFunctionSlot->u.builtinFunction.theCode(numArgs);
				RETURN(value);
			}
		}
		case T_BUILTINSYNTAX:
		{
			TAILCALL2(evaluatedFunctionSlot->u.CP_builtinSyntax.theCode, env, restArgs);
		}
		case T_USERDEFINEDFUNCTION:
		{
			
			// 1. check if the udfs expected numArgs is equal to the given args
			// 2. put the given args in a new env and bind them to the expected formal args
			// 3. eval every body in bodylist in the new env
			// 4. return last bodies value
			
			// this is can be an overhead since we dont expect that case to happen often
			// -> refactor to checking args number while building env
			int numFormalArgs = evaluatedFunctionSlot->u.userDefinedFunction.numArgs;
			int numArgs = length(restArgs);

			if( numFormalArgs != numArgs){
				js_function_error("(lambda): function expects %d args given %d", evaluatedFunctionSlot, numFormalArgs, numArgs);
			}

			// fill new environment
			int numLocals = evaluatedFunctionSlot->u.userDefinedFunction.numLocals;
			OBJ home = evaluatedFunctionSlot->u.userDefinedFunction.home;

			OBJ newEnv = newEnvironment((numFormalArgs + numLocals), home);
			OBJ restFormalArgs = evaluatedFunctionSlot->u.userDefinedFunction.argList;
			//restArgs = argList;
			//int slotIndex = 0;
			
			OBJ restBodyList = evaluatedFunctionSlot->u.userDefinedFunction.bodyList;	
			
			// save restFormalArgs and newEnv in LOCALS
			SET_LOCAL(3, restFormalArgs);
			SET_LOCAL(4, newEnv);
			SET_LOCAL(5, restBodyList);

			ASSERT(ISENV(newEnv), "bad env");
			if( restFormalArgs != js_nil){
			
				// given arguments
				OBJ unevaluatedArg = CAR(restArgs);
				restArgs = CDR(restArgs);
				SET_LOCAL(1, restArgs);
				
				// put evaluated arg in new env
				VOIDPTRFUNC CP_evalCons4();
				CALL2(CP_js_eval, env, unevaluatedArg, CP_evalCons4);
			}
			
			/*
			 *  eval bodies
			 */
			
			// case: BodyList is nil -> RETURN nil.
			if( restBodyList == js_nil ) RETURN(js_nil);
			
			// case: Last Body in BodyList -> EVAL as TAILCALL
			if( CDR(restBodyList) == js_nil){
				OBJ lastBody = CAR(restBodyList);
				TAILCALL2(CP_js_eval, newEnv, lastBody);
			}
			// case: Not the last Body -> Loop eval with CP_evalCons5.
			OBJ nextBody = CAR(restBodyList);
			restBodyList = CDR(restBodyList);
			SET_LOCAL(5, restBodyList);
			
			VOIDPTRFUNC CP_evalCons5();	
			CALL2(CP_js_eval, newEnv, nextBody, CP_evalCons5);
		}
		default:
			js_error("Eval: not a function: ", evaluatedFunctionSlot);
	}
	// NOT REACHED
	return NULL;
	
}

/*
 *	An argument for a builtinFunction has been evaluated
 */
VOIDPTRFUNC
CP_evalCons3(){
	
	OBJ evaluatedArg = RETVAL;
	PUSH(evaluatedArg);

	OBJ env = ARG(0);
	OBJ restArgs = CDR(LOCAL(1));
	SET_LOCAL(1, restArgs);
	
	ASSERT(ISENV(env), "bad env");
	int numArgs = (int) LOCAL(2);
	numArgs++;
	SET_LOCAL(2, (OBJ)((INT)numArgs));

	if( restArgs != js_nil){
	
		OBJ nextArg = CAR(restArgs);
		printJStack(__FILE__,__LINE__,__FUNCTION__);	
		CALL2(CP_js_eval, env, nextArg, CP_evalCons3);	
	}

	printJStack(__FILE__,__LINE__,__FUNCTION__);	
	OBJ function = LOCAL(0);
	OBJ value = function->u.builtinFunction.theCode(numArgs);
	RETURN(value);
}

/*
 *	An argument for a user defined function has been evaluated
 */
VOIDPTRFUNC
CP_evalCons4(){
	
	OBJ evaluatedArg = RETVAL;
	OBJ restFormalArgs = LOCAL(3);
	OBJ newEnv = LOCAL(4);
	OBJ nextFormalArg = CAR(restFormalArgs);	
	
	ASSERT(ISENV(newEnv), "bad env");
	// put evaluatedArg in newEnv
	environmentPut(newEnv, nextFormalArg, evaluatedArg);

	restFormalArgs = CDR(restFormalArgs);
	SET_LOCAL(3, restFormalArgs);

	// case 1: Still some formalArgs to go...
	if( restFormalArgs != js_nil){
		printJStack(__FILE__,__LINE__,__FUNCTION__);
		OBJ env = ARG(0);
		ASSERT(ISENV(env), "bad env");
		OBJ restArgs = LOCAL(1);
	
		OBJ unevaluatedArg = CAR(restArgs);
		restArgs = CDR(restArgs);
		SET_LOCAL(1, restArgs);

		CALL2(CP_js_eval, env, unevaluatedArg, CP_evalCons4); 
	}
	
	printJStack(__FILE__,__LINE__,__FUNCTION__);
	// case 2: newEnv is ready, eval bodies now...
	OBJ restBodyList = LOCAL(5);

	// case: BodyList is nil -> RETURN nil.
	if( restBodyList == js_nil ) RETURN(js_nil);
	
	// case: Last Body in BodyList -> EVAL as TAILCALL
	if( CDR(restBodyList) == js_nil){
		OBJ lastBody = CAR(restBodyList);
		TAILCALL2(CP_js_eval, newEnv, lastBody);
	}
	// case: Not the last Body -> Loop eval with CP_evalCons5.
	OBJ nextBody = CAR(restBodyList);
	restBodyList = CDR(restBodyList);
	SET_LOCAL(5, restBodyList);
	
	VOIDPTRFUNC CP_evalCons5();
	CALL2(CP_js_eval, newEnv, nextBody, CP_evalCons5);
}

/*
 *	A body of a user defined function has been evaluated
 */
VOIDPTRFUNC
CP_evalCons5(){
	
	printJStack(__FILE__,__LINE__,__FUNCTION__);
	OBJ newEnv = LOCAL(4);
	OBJ restBodyList = LOCAL(5);

	ASSERT(ISENV(newEnv), "bad env");
	// case: Last Body in BodyList -> EVAL as TAILCALL
	if( CDR(restBodyList) == js_nil){
		OBJ lastBody = CAR(restBodyList);
		TAILCALL2(CP_js_eval, newEnv, lastBody);
	}
	// case: Not the last Body -> Loop eval with CP_evalCons5.
	OBJ nextBody = CAR(restBodyList);
	restBodyList = CDR(restBodyList);
	SET_LOCAL(5, restBodyList);

	CALL2(CP_js_eval, newEnv, nextBody, CP_evalCons5);
}
