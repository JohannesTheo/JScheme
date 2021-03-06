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
// CP memory
OBJ CP_newBuiltinSyntax(char *, VOIDPTRFUNC);

//reader
OBJ js_read();
int thisIsTheEnd();

// eval
OBJ js_eval();
void initJStack();

// print
void js_print(FILE *, OBJ, int);
void printJStack();
#ifdef DEBUG
void printLocalEnv(OBJ);
void printIndent(int);
char* functionName(VOIDPTRFUNC);
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
void environmentSet(OBJ env, OBJ storedKey, OBJ storedValue);
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
OBJ builtin_include();
OBJ builtin_gThanNrP();
OBJ builtin_set_car();
OBJ builtin_set_cdr();
OBJ builtin_display();
OBJ builtin_write();
OBJ builtin_eqStringP();

// builtinSyntax
int isDefine();
int isLambda();
OBJ builtin_define();
OBJ builtin_if();
OBJ builtin_lambda();
OBJ builtin_quote();
OBJ builtin_set();
// CP bultinSyntax
VOIDPTRFUNC CP_builtin_define();
VOIDPTRFUNC CP_builtin_define2();
VOIDPTRFUNC CP_builtin_lambda();
VOIDPTRFUNC CP_builtin_quote();
VOIDPTRFUNC CP_builtin_if();
VOIDPTRFUNC CP_builtin_if2();
VOIDPTRFUNC CP_builtin_set();
VOIDPTRFUNC CP_builtin_set2();
/*
 *	Continuation passing
 */
void trampoline(VOIDPTRFUNC);
OBJ enterTrampoline1(OBJ);
VOIDPTRFUNC CP_js_eval();

