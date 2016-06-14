#include <stdio.h>
#include "jscheme.h"

void
js_print(OBJ o){

	enum tag kindOf = o->u.integer.tag;

	switch(kindOf){
		case T_INTEGER:
			printf("%ld" , o->u.integer.intVal);
			break;
		case T_SYMBOL:
			printf("%s", o->u.symbol.symbolVal);
			break;
		default:
			printf("#<unimpl. print>");
			break;
	}
}
