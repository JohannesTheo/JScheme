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
	T_FILESTREAM
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
		struct jschemeFileStream fileStream;
	} u;
};

/*
 * Forward declarations
 */

// memory
OBJ newInteger(jscheme_int64);
OBJ newFileStream(FILE *);

//reader
OBJ js_read();

// print
void js_print(OBJ);

