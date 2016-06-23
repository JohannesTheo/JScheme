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
			js_error("undefined variable", expr);
			return expr;
	}
}
