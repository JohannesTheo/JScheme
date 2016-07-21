#include "jscheme.h"

OBJ
js_eval(OBJ expr){

	switch(TAG(expr)){
		default:
			//T_INTEGER
			// ...
			// T_STRING
			return expr;

		case T_SYMBOL:
			printf("Address of symbol: %p\n", expr); 
			js_error("undefined variable", expr);
			return expr;
	}
}
