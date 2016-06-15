#include <stdio.h>
/*
 *	One header file to confuse them all ( and myself )
 */


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
	T_INTEGER,
	T_SYMBOL,
	T_STRING,
	T_FILESTREAM
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
};

struct jschemeInteger{
	enum tag tag;
	jscheme_int64 intVal;
};

struct jschemeObject{

	// More Types will be added
	union {
		struct jschemeInteger integer;
		struct jschemeSymbol symbol;
		struct jschemeString string;
		struct jschemeFileStream fileStream;
	} u;
};

/*
 * Forward declarations
 */

// memory
OBJ newInteger(jscheme_int64);
OBJ newSymbol(char *);
OBJ newString(char *);
OBJ newFileStream(FILE *);

//reader
OBJ js_read();

// print
void js_print(OBJ);

