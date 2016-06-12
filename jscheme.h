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
};

struct jschemeInteger{
	enum tag tag;
	jscheme_int64 intVal;
};

struct jschemeObject{

	// More JScheme Type will be added
	union {
		struct jschemeInteger integer;
	} u;
};

/*
 * Forward devlarations
 */

// memory
OBJ newInteger(jscheme_int64);

//reader
OBJ js_read();
OBJ readNumber(char);

// print
void js_print(OBJ);

