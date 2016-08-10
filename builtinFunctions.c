#include "jscheme.h"

OBJ
builtin_plus(int numArgs){

	jscheme_int64  start = 0;
	jscheme_int64 *sum = NULL;
	sum = &start;
	int i;

	for(i = 0; i < numArgs; i++){
		OBJ theArg  = POP();

		if( !ISINTEGER(theArg)){
			POPN((numArgs - 1) - i);
			js_error("(+): non-integer argument", theArg);
		}
		
	       	if(__builtin_saddl_overflow( *sum, INTVAL(theArg), sum)){
			// clean evalStack
			POPN((numArgs - 1) - i);
			printf("spIndex: %d\n", spIndex);
			js_error("(+): integer overflow", newInteger(*sum));	
		};
	}
	printf("spIndex: %d\n", spIndex);
	return newInteger(*sum);
}

OBJ
builtin_minus(int numArgs){
	OBJ theArg;
	
	switch (numArgs){
		case 0:
			js_error("(-): at least one arg expected", js_nil);
			/* NOT REACHED */
		case 1:
			theArg = POP();
			if( !ISINTEGER(theArg)){
				js_error("(-): non-integer argument", theArg);
			}
			return newInteger( -INTVAL(theArg) );
		default:
			theArg = NTH_ARG(numArgs, 0);

			if( !ISINTEGER(theArg)){
				POPN(numArgs);
				js_error("(-): non-integer argument", theArg);
			}
			
			jscheme_int64 *difference = NULL;
			jscheme_int64 start = INTVAL(theArg);
			difference = &start;

			for(int i = 1; i < numArgs; i++){
				OBJ nextArg = NTH_ARG(numArgs, i);
				if( !ISINTEGER(nextArg)){
					POPN(numArgs);
					js_error("(-): non-integer argument", theArg);
				}
				if(__builtin_ssubl_overflow(*difference, INTVAL(nextArg), difference)){
					// clean stack
					POPN(numArgs);
					printf("spIndex: %d\n", spIndex);
					js_error("(-): integer overflow", newInteger(*difference));
				};
			}
			POPN(numArgs);
			printf("spIndex: %d\n", spIndex);
			return newInteger(*difference);
	}	
	/* NOT REACHED */
	return js_nil;
}

OBJ
builtin_times(int numArgs){
	jscheme_int64 *product= NULL;
	jscheme_int64 start = 1;
	product = &start;

	for(int i = 0; i < numArgs; i++){
		OBJ theArg = POP();
		
		if( !ISINTEGER(theArg)){
			POPN((numArgs - 1) - i);
			js_error("(*): non-integer argument", theArg);
		}
		if(__builtin_smull_overflow(*product,INTVAL(theArg),product)){
			// clean stack
			POPN((numArgs - 1) - i);
			printf("spIndex: %d\n", spIndex);
			js_error("(*): integer overflow", newInteger(*product));
		}
	}
	printf("spIndex: %d\n", spIndex);
	return newInteger(*product);
}

OBJ
builtin_quotient(int numArgs){
	
	printf(RED "WARNING:" RESET " division is implemented for integers only and will truncate fractions!\n");
	OBJ theArg;
	switch (numArgs){
		
		case 0:
			printf("spIndex: %d\n", spIndex);
			js_error("(/): at least one arg expected", js_nil);
			/* NOT REACHED */
		case 1:
			theArg = POP();
			if( !ISINTEGER(theArg)){
			printf("spIndex: %d\n", spIndex);
				js_error("(/): non-integer argument", theArg);
				/* NOT REACHED */
			}
			if( INTVAL(theArg) == 0){
			printf("spIndex: %d\n", spIndex);
				js_error("(/): division by zero", theArg);
				/* NOT REACHED */
			}
			printf("spIndex: %d\n", spIndex);
			return newInteger( 1 / INTVAL(theArg) );
		default:
			theArg = NTH_ARG(numArgs, 0);

			if( !ISINTEGER(theArg)){
				POPN(numArgs);
				printf("spIndex: %d\n", spIndex);
				js_error("(/): non-integer argument", theArg);
				/* NOT REACHED */
			}
			if( INTVAL(theArg) == 0){

				for(int i = 1; i < numArgs; i++){
					
					OBJ nextArg = NTH_ARG(numArgs, i);
					if( !ISINTEGER(nextArg) ){
						POPN(numArgs);
						printf("spIndex: %d\n", spIndex);
						js_error("(/): non-integer argument", theArg);
						/* NOT REACHED */
					}
					if( INTVAL(nextArg) == 0){
						POPN(numArgs);
						printf("spIndex: %d\n", spIndex);
						js_error("(/): division by zero", nextArg);
						/* NOT REACHED */
					}
				}
				POPN(numArgs);
				printf("spIndex: %d\n", spIndex);
				return newInteger(0);
			}
			
			jscheme_int64 quotient = INTVAL(theArg);
			for(int i = 1; i < numArgs; i++){
				
				OBJ nextArg = NTH_ARG(numArgs, i);
				if( !ISINTEGER(nextArg) ){
					POPN(numArgs);
					printf("spIndex: %d\n", spIndex);
					js_error("(/): non-integer argument", theArg);
					/* NOT REACHED */
				}
				if( INTVAL(nextArg) == 0){
					POPN(numArgs);
					printf("spIndex: %d\n", spIndex);
					js_error("(/): division by zero", nextArg);
					/* NOT REACHED */
				}

				quotient = quotient / INTVAL(nextArg);
			}
			POPN(numArgs);
			printf("spIndex: %d\n", spIndex);
			return newInteger(quotient);
	}
	/* NOT REACHED */
	return js_nil;
}

OBJ
builtin_eqP(int numArgs){
	if(numArgs != 2){
		POPN(numArgs);
		js_error("(eq?): expects 2 arguments", js_nil);
	}

	OBJ arg2 = POP();
	OBJ arg1 = POP();
	
	// case 1: same jscheme OBJ 
	if( arg1 == arg2) return js_true;
	// case 2: same INTEGER value
	if(ISINTEGER(arg1)){
		if(ISINTEGER(arg2)){
			if( INTVAL(arg1) == INTVAL(arg2)){
				return js_true;
			}
		}
	}
	// TO-DO: Strings, cons, ...
	return js_false;
}

OBJ
builtin_consP(int numArgs){
	if(numArgs != 1){
		POPN(numArgs);
		js_error("(cons?): expects 1 argument", js_nil);
	}
	OBJ theArg = POP();
	return( ISCONS(theArg) ? js_true : js_false);
}


OBJ
builtin_car(int numArgs){
	if(numArgs != 1){
		POPN(numArgs);
		js_error("(car): expects 1 argument", js_nil);
	}
	OBJ theArg = POP();
	if(!ISCONS(theArg)){
		js_error("(car): non-cons argument", theArg);
	}
	return CAR(theArg);
}

OBJ
builtin_cdr(int numArgs){
	if(numArgs != 1){
		POPN(numArgs);
		js_error("(cdr): expects 1 argument", js_nil);
	}
	OBJ theArg = POP();
	if(!ISCONS(theArg)){
		js_error("(cdr): non-cons argument", theArg);
	}
	return CDR(theArg);
}

OBJ
builtin_cons(int numArgs){
	if(numArgs != 2){
		POPN(numArgs);
		js_error("(cons): expects 2 arguments", js_nil);
	}
	OBJ arg2 = POP();
	OBJ arg1 = POP();

	return newCons(arg1, arg2);
}

/*
 * syntax
 */

OBJ
builtin_define(int numArgs, OBJ env, OBJ ignoredArgList){
	
	if( numArgs < 2){
		POPN(numArgs);
		js_error("(define): expects at least  2 arguments", js_nil);
	}
	OBJ arg1 = NTH_ARG(numArgs, 0);	// evaluated
	OBJ arg2 = NTH_ARG(numArgs, 1);	// not evaluated

	// case 1: define SYMBOL -> (define symbol expression)
	if( ISSYMBOL(arg1)) {
		if( numArgs != 2){
			POPN(numArgs);
			js_error("(define): this form expects exactly 2 arguments", js_nil);
		}
		environmentPut(env, arg1, js_eval(globalEnvironment, arg2));
		POPN(2);
		return js_void;
	}

	POPN(numArgs);
	error("define form unimplemented", __FILE__, __LINE__);

	// NOT REACHED
	return js_nil;
}

OBJ
builtin_if(int numArgs, OBJ env, OBJ ignoredArgList){
	OBJ condExpr, ifExpr, elseExpr, condValue;
	
	if( numArgs == 2){
		// case 1: else-less if -> (if cond <expr>)
		elseExpr = js_nil;

	} else if( numArgs == 3){
		// case 2: regular if -> (if cond <ifExpr> <elseExpr>)
		elseExpr = NTH_ARG(numArgs, 2);
	} else {
		POPN(numArgs);
		js_error("(if): expects 2 or 3 arguments", js_nil);
	}

	condExpr = NTH_ARG(numArgs, 0);
	ifExpr = NTH_ARG(numArgs, 1);
	POPN(numArgs);

	condValue = js_eval(env, condExpr);
	if (condValue == js_true){
		return (js_eval(env, ifExpr));
	}
	if (condValue == js_false){
		return (js_eval(env,elseExpr));
	}

	// TO-DO implement #t #f rules for all kind of OBJs
	js_error("(if): non-boolean condition value", condValue);
	// NOT REACHED
	return js_nil;
}

OBJ
builtin_lambda(int numArgs, OBJ env, OBJ lambdaArgList){
	
	printEvalStack();
	if( numArgs < 2){
		POPN(numArgs);
		js_error("(lambda): expects at least 2 arguments", js_nil);
	}

	OBJ argList, bodyList;

	argList = NTH_ARG(numArgs,0);
	if( ! (argList == js_nil || ISCONS(argList) )){
		POPN(numArgs);
		js_error("(lambda): invalid argument list", argList);
	}
	/*
	 * all bodies are on the stack BUT we need them as list -> use CDR(lambdaArgList) instead!
	 */
	bodyList = CDR(lambdaArgList);
	
	POPN(numArgs);
	return newUserDefinedFunction( "anonymous lambda", argList, bodyList);
}
