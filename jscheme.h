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

#ifndef POINTER_SIZE
#if defined( __x86_64__ )
#  define POINTER_SIZE 8
#else
#  define POINTER_SIZE 4
# endif
#endif

#if POINTER_SIZE == 4
#  define INT jscheme_int32
#else 
#if POINTER_SIZE == 8
#  define INT jscheme_int64
#else
#  error "please define pointer size"
# endif
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
	T_BUILTINFUNCTION,
	T_BUILTINSYNTAX,
	T_VOID,
	T_GLOBALENVIRONMENT,
	T_LOCALENVIRONMENT,
	T_USERDEFINEDFUNCTION,
};

extern const char* tag_lookup[];

extern OBJ globalEnvironment;

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

typedef OBJ (*OBJFUNC)();
typedef void* (*VOIDPTRFUNC)();

struct jsBuiltinFunction{
	enum tag tag;

	char *internalName;
	OBJFUNC theCode;
}; 

struct jsBuiltinSyntax{
	enum tag tag;

	char *internalName;
	OBJFUNC theCode;
};

struct CP_jsBuiltinSyntax{
	enum tag tag;

	char *internalName;
	VOIDPTRFUNC theCode;
};

struct jsUserDefinedFunction{
	enum tag tag;
	
	char *internalName;
	int numLocals;
	int numArgs;
	OBJ home;
	OBJ argList;
	OBJ bodyList;
};

struct jsEnvironmentEntry{
	OBJ key;	// must be a symbol
	OBJ value;	// any object
};
typedef struct jsEnvironmentEntry JS_ENV_ENTRY;

struct jsEnvironment{
	enum tag tag;
	int numSlots;
	OBJ parentEnvironment;
	JS_ENV_ENTRY slots[];
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
		struct jsBuiltinFunction builtinFunction;
		struct jsBuiltinSyntax builtinSyntax;
		struct CP_jsBuiltinSyntax CP_builtinSyntax;
		struct jsEnvironment environment;
		struct jsUserDefinedFunction userDefinedFunction;
	} u;
};

/*
 *	macros
 */
#ifdef DEBUG
#define ASSERT(expr,msg){ ((!(expr)) ? error(msg, __FILE__, __LINE__): 0); }
#else
#define ASSERT(expr,msg) {}
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
#define ISBUILTINF(o) 	(TAG(o) == T_BUILTINFUNCTION)
#define ISBUILTINS(o)	(TAG(o) == T_BUILTINSYNTAX)
#define ISVOID(o)	(TAG(o) == T_VOID)
#define ISUDF(o)	(TAG(o) == T_USERDEFINEDFUNCTION)

/*
 * well known objects
 */
OBJ js_nil, js_true, js_false, js_void;
OBJ js_sym_define, js_sym_lambda;

#ifdef DEBUG
/*
 * debug tracing
 */

#define INDENT "      "
extern int indentLevel;
extern int oldIndentLevel;
extern int PAUSE_INDENT_FLAG;

extern struct debugOption *DETAILED_TYPES;
extern struct debugOption *EVAL_TRACE; 

struct debugOption{
	char *name;
	char *identifier;
	int state;
};
void switchDebugOptions(OBJ);
void initDebugOptions();
#endif

#include "jschemeInternals.h"
#include "jschemeStack.h"
