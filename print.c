#include <stdio.h>
#include "jscheme.h"

void
js_print(OBJ o){

	enum tag kindOf = o->u.integer.tag;

#ifdef DEBUG
	switch(kindOf){
		case T_INTEGER:
			printf("<JS integer> ");
			break;
		case T_SYMBOL:
			printf("<JS symbol> ");
			break;
		case T_STRING:
			printf("<JS string> ");
			break;
		default:
			printf("<Debug error: tag not supported>\n");
	}
#endif

	switch(kindOf){
		case T_INTEGER:
			printf("%ld" , o->u.integer.intVal);
			break;
		case T_SYMBOL:
			printf("%s", o->u.symbol.symbolVal);
			break;
		case T_STRING:
			printf("\"%s\"", o->u.string.stringVal);
			break;
		default:
			printf("<unimpl. (print)>");
			break;
	}
}
