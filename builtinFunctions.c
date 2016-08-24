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
//	printf("spIndex: %d\n", spIndex);
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
//			printf("spIndex: %d\n", spIndex);
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
//	printf("spIndex: %d\n", spIndex);
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
//				printf("spIndex: %d\n", spIndex);
				js_error("(/): non-integer argument", theArg);
				/* NOT REACHED */
			}
			if( INTVAL(theArg) == 0){

				for(int i = 1; i < numArgs; i++){
					
					OBJ nextArg = NTH_ARG(numArgs, i);
					if( !ISINTEGER(nextArg) ){
						POPN(numArgs);
//						printf("spIndex: %d\n", spIndex);
						js_error("(/): non-integer argument", theArg);
						/* NOT REACHED */
					}
					if( INTVAL(nextArg) == 0){
						POPN(numArgs);
//						printf("spIndex: %d\n", spIndex);
						js_error("(/): division by zero", nextArg);
						/* NOT REACHED */
					}
				}
				POPN(numArgs);
//				printf("spIndex: %d\n", spIndex);
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
//			printf("spIndex: %d\n", spIndex);
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

int
isLambda(OBJ expr){

	if( ISCONS(expr) && (CAR(expr) == js_sym_lambda) ){
			return 1;
	}
	return 0;
}

int
isDefine(OBJ expr){

	if( ISCONS(expr) && (CAR(expr) == js_sym_define) ){
			return 1;
	}
	return 0;
}

static int
count_defines(OBJ bodyList){

	int count = 0;
	while( bodyList != js_nil){
		if( isDefine( CAR( bodyList))){
			count++;
		}
		bodyList = CDR(bodyList);
	}
	return count;
}

OBJ
builtin_define(OBJ env, OBJ argList){

	if( !ISCONS(argList) ){
		js_error("(define): expects at least  2 arguments", js_nil);
	}
	
	OBJ arg1 = CAR(argList);
	argList = CDR(argList);

	if( !ISCONS(argList) ){
		js_error("(define): expects at least  2 arguments", js_nil);
	}

	// case 1: define SYMBOL -> (define symbol expression)
	if( ISSYMBOL(arg1)) {
		OBJ arg2 = CAR(argList);
		argList = CDR(argList);
		if( argList != js_nil ){
			js_error("(define): this form expects exactly 2 arguments", js_nil);
		}
		OBJ value = js_eval(env, arg2);
		environmentPut(env, arg1, value);

#ifdef DEBUG
// PRINT TRACE
if( EVAL_TRACE->state) {
		printIndent(indentLevel);
		fprintf(stdout, RED"DEFINE "RESET);
		js_print(stdout, arg1);       
		fprintf(stdout, " -> ");
		js_print(stdout, value);

		if( TAG(env) == T_GLOBALENVIRONMENT ){

			fprintf(stdout," in " CYN "GLOBAL" RESET " (%p)\n", env);
		} 
		if( TAG(env) == T_LOCALENVIRONMENT ){
			fprintf(stdout," in " YEL "LOCAL" RESET " (%p)\n", env);
		}
}
#endif

		return js_void;
	}
	// case 2: define CONS ( function ) -> (define (name args*) (body*) )
	if( ISCONS(arg1)){
		
		OBJ name = CAR(arg1);
		if( ISSYMBOL(name) ){
			OBJ formalArgList = CDR(arg1);
			OBJ bodyList = argList;
			OBJ newUDF;

			newUDF = newUserDefinedFunction("anonymous lambda", formalArgList, bodyList);
			newUDF->u.userDefinedFunction.numLocals = count_defines(bodyList);
			newUDF->u.userDefinedFunction.home = env;
			environmentPut(env, name, newUDF);

#ifdef DEBUG
// PRINT TRACE
if( EVAL_TRACE->state ){
			printIndent(indentLevel);
			fprintf(stdout, RED"DEFINE "RESET);
			js_print(stdout, name);       
			fprintf(stdout, " -> ");
			js_print(stdout, newUDF);

			if( TAG(env) == T_GLOBALENVIRONMENT ){

				fprintf(stdout," in " CYN "GLOBAL" RESET " (%p)\n", env);
			} 
			if( TAG(env) == T_LOCALENVIRONMENT ){
				fprintf(stdout," in " YEL "LOCAL" RESET " (%p)\n", env);
			}
}
#endif
			return js_void;
		}
	}
	error("define form unimplemented", __FILE__, __LINE__);

	// NOT REACHED
	return js_nil;
}

OBJ
builtin_if(OBJ env, OBJ argList){
	OBJ condExpr, ifExpr, elseExpr, condValue;
	
	int numArgs = length(argList);
	if( numArgs == 2){
		// case 1: else-less if -> (if cond <expr>)
		elseExpr = js_nil;

	} else if( numArgs == 3){
		// case 2: regular if -> (if cond <ifExpr> <elseExpr>)
		elseExpr = CAR( CDR( CDR( argList )));
	} else {
		js_error("(if): expects 2 or 3 arguments", js_nil);
	}

	condExpr = CAR(argList); 
	ifExpr = CAR( CDR(argList));

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
builtin_lambda(OBJ env, OBJ argList){
	
	if( !ISCONS(argList) ){
		js_error("(lambda): expects at least 2 arguments", js_nil);
	}

	OBJ lambdaArgList = CAR(argList);
	if( ! (lambdaArgList == js_nil || ISCONS(lambdaArgList) )){
		js_error("(lambda): invalid argument list", lambdaArgList);
	}
	OBJ bodyList = CDR(argList);
	
	OBJ newUDF = newUserDefinedFunction( "anonymous lambda", lambdaArgList, bodyList);
	newUDF->u.userDefinedFunction.numLocals = count_defines(bodyList);
	newUDF->u.userDefinedFunction.home = env;
			
	return newUDF;
}

OBJ
builtin_quote(OBJ env, OBJ argList){

	if( (!ISCONS(argList)) || ( CDR(argList) != js_nil) ){
		js_error("(quote): expects exactly 1 argument", js_nil);
	}
	return CAR(argList);
}
