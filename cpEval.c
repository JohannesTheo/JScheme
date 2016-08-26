#include "jscheme.h"

VOIDPTRFUNC
CP_js_eval(){

	OBJ env = globalEnvironment;
	OBJ expr = ARG(0);	

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
			error("<CP_evalCons unimpl.>", __FILE__, __LINE__);
			return NULL;
	}
}
