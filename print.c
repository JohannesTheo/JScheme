#include <stdio.h>
#include "jscheme.h"

void
js_print(OBJ o){

	enum tag kindOf = o->u.any.tag;

#ifdef DEBUG
	switch(kindOf){
		case T_NIL:
			printf(YEL"<JS nil> "RESET);
			break;
		case T_TRUE:
			printf(YEL"<JS true> "RESET);
			break;
		case T_FALSE:
			printf(YEL"<JS false> "RESET);
			break;
		case T_INTEGER:
			printf(YEL"<JS integer> "RESET);
			break;
		case T_SYMBOL:
			printf(YEL"<JS symbol> "RESET);
			break;
		case T_STRING:
			printf(YEL"<JS string> "RESET);
			break;
		default:
			printf(RED"<Debug error: tag not supported>\n"RESET);
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
