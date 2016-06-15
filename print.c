#include <stdio.h>
#include "jscheme.h"

void
js_print(OBJ o){

	enum tag kindOf = o->u.any.tag;

#ifdef DEBUG
	switch(kindOf){
		case T_NIL:
			printf("<JS nil> ");
			break;
		case T_TRUE:
			printf("<JS true> ");
			break;
		case T_FALSE:
			printf("<JS false> ");
			break;
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
		case T_NIL:
			printf("nil");
			break;
		case T_TRUE:
			printf("#t");
			break;
		case T_FALSE:
			printf("#f");
			break;
		case T_INTEGER:
			printf("%ld" , o->u.integer.intVal);
			break;
		case T_STRING:
			printf("\"%s\"", o->u.string.stringVal);
			break;
		case T_SYMBOL:
			printf("%s", o->u.symbol.symbolVal);
			break;
		default:
			printf("<unimpl. (print)>");
			break;
	}
}
