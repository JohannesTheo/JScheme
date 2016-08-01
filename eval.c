#include "jscheme.h"

OBJ
js_eval(OBJ expr){

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
	}
}
