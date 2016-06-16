#include <stdio.h>
/*
 *	One header file to confuse them all ( and myself )
 */

#define DEBUG

#ifdef __WIN32__
typedef int jscheme_int32;
typedef long long jscheme_int64;
#else
typedef int jscheme_int32;
typedef long jscheme_int64;
#endif

// In JScheme everything will be a jschemeObject
typedef struct jschemeObject *OBJ;

enum tag{
	T_NIL,
	T_TRUE,
	T_FALSE,
	T_INTEGER,
	T_SYMBOL,
	T_STRING,
	T_FILESTREAM
};

struct jschemeAny{
	enum tag tag;
};

struct jschemeString{
	enum tag tag;
	char *stringVal;
};

struct jschemeSymbol{
	enum tag tag;
	char *symbolVal;
};

struct jschemeFileStream{
	enum tag tag;
	FILE *file;
	int peekChar;
};

struct jschemeInteger{
	enum tag tag;
	jscheme_int64 intVal;
};

struct jschemeObject{

	// More Types will be added
	union {
		struct jschemeAny any;
		struct jschemeInteger integer;
		struct jschemeSymbol symbol;
		struct jschemeString string;
		struct jschemeFileStream fileStream;
	} u;
};

/*
 * Forward declarations
 */

// well known objects
OBJ js_nil, js_true, js_false;

// memory
OBJ newInteger(jscheme_int64);
OBJ newSymbol(char *);
OBJ newString(char *);
OBJ newFileStream(FILE *);

//reader
OBJ js_read();

// print
void js_print(OBJ);

// main prompt
void prompt_on();
void prompt_off();

