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
				js_error("(-): non-integer argument", theArg);
			}
			
			jscheme_int64 *difference = NULL;
			jscheme_int64 start = INTVAL(theArg);
			difference = &start;

			for(int i = 1; i < numArgs; i++){
				OBJ nextArg = NTH_ARG(numArgs, i);
				if( !ISINTEGER(nextArg)){
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
builtin_quotient(){
	error("unimpl. quotient", __FILE__, __LINE__);
	return js_nil;
}
OBJ
builtin_car(){
	error("unimpl. car", __FILE__, __LINE__);
	return js_nil;
}

OBJ
builtin_cdr(){
	error("unimpl. cdr", __FILE__, __LINE__);
	return js_nil;
}

OBJ
builtin_cons(){
	error("unimpl. cons", __FILE__, __LINE__);
	return js_nil;
}
