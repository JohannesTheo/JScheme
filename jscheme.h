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

// fancy colors
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

/*
 * 	In JScheme everything will be a jschemeObject
 */

typedef struct jschemeObject *OBJ;

enum tag{
	T_NIL,
	T_TRUE,
	T_FALSE,
	T_INTEGER,
	T_SYMBOL,
	T_STRING,
	T_FILESTREAM,
	T_STRINGSTREAM,
	T_CONS,
};

extern const char* tag_lookup[];

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

struct jschemeStringStream{
	enum tag tag;
	char* buffer;
	int index;
};

struct jschemeInteger{
	enum tag tag;
	jscheme_int64 intVal;
};

struct jschemeCons{
	enum tag tag;
	OBJ car;
	OBJ cdr;
};

struct jschemeObject{

	// More Types will be added
	union {
		struct jschemeAny any;
		struct jschemeInteger integer;
		struct jschemeSymbol symbol;
		struct jschemeString string;
		struct jschemeFileStream fileStream;
		struct jschemeStringStream stringStream;
		struct jschemeCons cons;
	} u;
};

/*
 *	macros
 */
#ifdef DEBUG
#define ASSERT(expr,msg){ ((!(expr)) ? error(msg, __FILE__, __LINE__): 0); }
#else
#define ASSERT(expr,msg) 
#endif

#define TAG(o)		((o)->u.any.tag)
#define CAR(o)		((o)->u.cons.car)
#define CDR(o)		((o)->u.cons.cdr)
#define INTVAL(o)	((o)->u.integer.intVal)
#define SYMBOLVAL(o)	((o)->u.symbol.symbolVal)
#define STRINGVAL(o)	((o)->u.string.stringVal)

#define ISNIL(o)	(TAG(o) == T_NIL)
#define ISTRUE(o)	(TAG(o) == T_TRUE)
#define ISFALSE(o)	(TAG(o) == T_FALSE)
#define ISINTEGER(o)	(TAG(o) == T_INTEGER)
#define ISSYMBOL(o)	(TAG(o) == T_SYMBOL)
#define ISSTRING(o)	(TAG(o) == T_STRING)
#define ISCONS(o)	(TAG(o) == T_CONS)

/*
 * 	Forward declarations
 */

// well known objects
OBJ js_nil, js_true, js_false;

// memory
OBJ newInteger(jscheme_int64);
OBJ newSymbol(char *);
OBJ newString(char *);
OBJ newFileStream(FILE *);
OBJ newStringStream(char *);
OBJ newCons(OBJ, OBJ);

//reader
OBJ js_read();
int thisIsTheEnd();

// eval
OBJ js_eval();

// print
void js_print(FILE *, OBJ);

// main
void prompt_on();
void prompt_off();
void getMeOutOfHere();

//error
void error(char *, char *, int);
void js_error(char *, OBJ);

// selftest
void selftest();

// symboltable
void initSymbolTable();
OBJ symbolTableGetOrAdd(char* );

// environment
void initGlobalEnvironment();
void environmentPut(OBJ storedKey, OBJ storedValue);
OBJ environmentGet(OBJ searchedKey);

