/*
 * eval stack
 */

extern OBJ *jStack;
extern int SP; 			// spIndex = index of next unused slot
extern int AP;
extern int BP;
extern int stackLimit;
extern OBJ RETVAL;


static inline void
PUSH(OBJ o) {
#ifdef DEBUG	
	if(SP == stackLimit) {
		error("stack overflow", __FILE__, __LINE__);
	}
#endif
	jStack[SP++] = o;
}

static inline OBJ
POP(){
#ifdef DEBUG
	if(SP == 0){
		error("stack underflow", __FILE__, __LINE__);
	}
#endif
	return jStack[--SP];
}

static inline void
POPN(int n){
	SP -= n;
}

static inline OBJ
NTH_ARG(int numArgs, int index){
#ifdef DEBUG
	if(SP ==0){
		error("stack underflow", __FILE__, __LINE__);
	}
#endif
	return jStack[SP - numArgs + index];
}

static inline OBJ
ARG(int n0based ){

	/*
	 ASSERT( ((AP + n0based) >= SP), "arg index error");
	 ASSERT( ((AP + n0based) >= SP), "arg index error");
	 */
	return jStack[AP + n0based];
}

static inline int
savedAP(){
	return (int)jStack[BP+1];
}

/*
 * Continuation helpers
 */
static inline VOIDPTRFUNC
CALL1_helper(VOIDPTRFUNC func, OBJ arg1, VOIDPTRFUNC continuation){
	
	// prepare new stackFrame
	PUSH(((OBJ)(INT)BP));
	BP = SP - 1;
	PUSH(((OBJ)(INT)AP));
	PUSH((OBJ) func);
	AP = SP;

	// push args
	PUSH(arg1);
	PUSH((OBJ)continuation);
	return func;
}
#define CALL1(func, arg1, continuation)\
{ \
	return CALL1_helper((VOIDPTRFUNC)func, arg1, (VOIDPTRFUNC) continuation);\
}

static inline VOIDPTRFUNC
CALL2_helper(VOIDPTRFUNC func, OBJ arg1, OBJ arg2, VOIDPTRFUNC continuation){
	
	// prepare new stackFrame
	PUSH(((OBJ)(INT)BP));
	BP = SP - 1;
	PUSH(((OBJ)(INT)AP));
	PUSH((OBJ) func);
	AP = SP;

	// push args
	PUSH(arg1);
	PUSH(arg2);
	PUSH((OBJ)continuation);
	return func;
}
#define CALL2(func, arg1, arg2, continuation)\
{ \
	return CALL2_helper((VOIDPTRFUNC)func, arg1, arg2, (VOIDPTRFUNC) continuation);\
}

static inline VOIDPTRFUNC
TAILCALL1_helper(VOIDPTRFUNC func, OBJ arg1){
	
	VOIDPTRFUNC retAddr = (VOIDPTRFUNC)(POP());
	
	jStack[AP] = arg1;
	SP = AP + 1;
	PUSH((OBJ)retAddr);
	return func;
}
#define TAILCALL1(func, arg1)\
{ \
	return TAILCALL1_helper((VOIDPTRFUNC)func, arg1);\
}

static inline VOIDPTRFUNC
TAILCALL2_helper(VOIDPTRFUNC func, OBJ arg1, OBJ arg2){
	
	VOIDPTRFUNC retAddr = (VOIDPTRFUNC)(POP());
	
	jStack[AP] = arg1;
	jStack[AP+1] = arg2;

	SP = AP + 2;
	PUSH((OBJ)retAddr);
	return func;
}
#define TAILCALL2(func, arg1, arg2)\
{ \
	return TAILCALL2_helper((VOIDPTRFUNC)func, arg1, arg2);\
}

static inline VOIDPTRFUNC
RETURN_helper(OBJ retVal){

	RETVAL = retVal;
	VOIDPTRFUNC retAddr = (VOIDPTRFUNC)(POP());
	AP = savedAP();
	SP = BP + 1;
	BP = (int)(POP());

	fprintf(stdout, "RETURN: ");
	js_print(stdout, retVal);
	fprintf(stdout, " -> %s\n", functionName((VOIDPTRFUNC)retAddr));

	return retAddr;
}

#define RETURN(retVal)\
{ \
	return RETURN_helper(retVal);\
}




