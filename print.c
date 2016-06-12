#include <stdio.h>
#include "jscheme.h"

void
js_print(OBJ o){

	enum tag kindOf = o->u.integer.tag;

	if( kindOf == T_INTEGER ) {
		printf("%c" , (int) o->u.integer.intVal);
	} else {
		printf("#<unimpl. print>");
	}
}
