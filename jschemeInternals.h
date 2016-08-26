/*
 *	Forward declarations
 */

// memory
OBJ newInteger(jscheme_int64);
OBJ newSymbol(char *);
OBJ newString(char *);
OBJ newFileStream(FILE *);
OBJ newStringStream(char *);
OBJ newCons(OBJ, OBJ);
OBJ newBuiltinFunction(char *, OBJFUNC);
OBJ newBuiltinSyntax(char *, OBJFUNC);
OBJ newEnvironment(int numSlots, OBJ parentEnv);
OBJ newUserDefinedFunction(char *, OBJ argList, OBJ bodyList);
int length();

//reader
OBJ js_read();
int thisIsTheEnd();

// eval
OBJ js_eval();
void initJStack();

// print
void js_print(FILE *, OBJ);
void printEvalStack();
#ifdef DEBUG
void printLocalEnv(OBJ);
void printIndent(int);
#endif 

// main
void prompt_on();
void prompt_off();
void getMeOutOfHere();
void trampoline(VOIDPTRFUNC);

//error
void error(char *, char *, int);
void js_error(char *, OBJ);
void js_function_error(char*, OBJ, int, int);

// selftest
void selftest();

// symboltable
void initSymbolTable();
OBJ symbolTableGetOrAdd(char* );

// environment
void initGlobalEnvironment();
void environmentPut(OBJ env, OBJ storedKey, OBJ storedValue);
OBJ environmentGet(OBJ env, OBJ searchedKey);
void globalEnvironmentPut(OBJ storedKey, OBJ storedValue);
OBJ globalEnvironmentGet(OBJ searchedKey);

// builtinFuntions
OBJ builtin_plus();
OBJ builtin_minus();
OBJ builtin_times();
OBJ builtin_quotient();
OBJ builtin_eqP();
OBJ builtin_consP();
OBJ builtin_car();
OBJ builtin_cdr();
OBJ builtin_cons();

// builtinSyntax
int isDefine();
int isLambda();
OBJ builtin_define();
OBJ builtin_if();
OBJ builtin_lambda();
OBJ builtin_quote();

/*
 *	Continuation passing
 */
VOIDPTRFUNC CP_js_eval();

