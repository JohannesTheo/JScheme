#include <stdlib.h>
#include "jscheme.h"

OBJ
newInteger(jscheme_int64 iVal) {

	struct jschemeInteger *theJSInt;

	theJSInt = (struct jschemeInteger*)(malloc( sizeof( struct jschemeInteger)));
	theJSInt->tag = T_INTEGER;
	theJSInt->intVal = iVal;
	return (OBJ) theJSInt;
}

