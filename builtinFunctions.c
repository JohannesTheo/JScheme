#include "jscheme.h"

OBJ
builtin_plus(int numArgs){

	jscheme_int64 sum = 0;
	int i;

	for(i = 0; i < numArgs; i++){
		OBJ theArg  = POP();

		if( !ISINTEGER(theArg)){
			js_error("(+): non-integer argument", theArg);
		}
		// TO-DO prevent OVERFLOW
		sum = sum + INTVAL(theArg);
	}
	return newInteger(sum);
}


// TO-DO: implement builtins 
OBJ
builtin_minus(int numArgs){
	if(numArgs == 0){
		js_error("(-): at least one arg expected", js_nil);
	}

	error("unimpl. minus", __FILE__, __LINE__);
	return js_nil;
}

OBJ
builtin_times(){
	error("unimpl. times", __FILE__, __LINE__);
	return js_nil;
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
