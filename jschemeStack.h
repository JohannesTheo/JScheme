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

static inline OBJ
LOCAL(int n0based ){

	/*
	 ASSERT( ((AP + n0based) >= SP), "arg index error");
	 ASSERT( ((AP + n0based) >= SP), "arg index error");
	 */
	return jStack[AP + 2 + n0based];
}

static inline void
SET_LOCAL(int n0based, OBJ o){

	/*
	 ASSERT( ((AP + n0based) >= SP), "arg index error");
	 ASSERT( ((AP + n0based) >= SP), "arg index error");
	 */
	jStack[AP + 2 + n0based] = o;
}

/*
 * Continuation helpers
 */

static inline int
savedAP(){
	return (int)jStack[BP+1];
}

static inline VOIDPTRFUNC
getContinuation(){
	return (VOIDPTRFUNC)jStack[BP+3];
}

static inline void
CREATE_LOCALS(int n){
	for(int i = 0; i < n; i++){
		PUSH(js_nil);
	}
}

static inline void
NEW_STACK_FRAME(){
	//printf(stdout, "ENTER %d\n", numLocals);
	// prepare new stackFrame
	PUSH(((OBJ)(INT)BP));
	BP = SP - 1;
	PUSH(((OBJ)(INT)AP));
	//for(int i = 0; i < numLocals; i++){
	//	PUSH(js_nil);
	//}
}

static inline void
LEAVE_STACK_FRAME(){
	//fprintf(stdout, "LEAVE\n");

	AP = savedAP();
	SP = BP+1;
	BP = (int)(POP());
}

static inline VOIDPTRFUNC
CALL1_helper(VOIDPTRFUNC func, OBJ arg1, VOIDPTRFUNC continuation){
	fprintf(stdout, "C1: %s Cont: %s\n",  functionName((VOIDPTRFUNC)func), functionName((VOIDPTRFUNC)continuation));
	NEW_STACK_FRAME();
	PUSH((OBJ) func);
	PUSH((OBJ)continuation);
	AP = SP;

	// push args
	PUSH(arg1);
	return func;
}
#define CALL1(func, arg1, continuation)\
{ \
	return CALL1_helper((VOIDPTRFUNC)func, arg1, (VOIDPTRFUNC) continuation);\
}

static inline VOIDPTRFUNC
CALL2_helper(VOIDPTRFUNC func, OBJ arg1, OBJ arg2, VOIDPTRFUNC continuation){
	fprintf(stdout, "C2: %s Cont: %s\n",  functionName((VOIDPTRFUNC)func), functionName((VOIDPTRFUNC)continuation));
	NEW_STACK_FRAME();	
	PUSH((OBJ) func);
	PUSH((OBJ)continuation);
	AP = SP;

	// push args
	PUSH(arg1);
	PUSH(arg2);
	return func;
}
#define CALL2(func, arg1, arg2, continuation)\
{ \
	return CALL2_helper((VOIDPTRFUNC)func, arg1, arg2, (VOIDPTRFUNC) continuation);\
}

static inline VOIDPTRFUNC
TAILCALL1_helper(VOIDPTRFUNC func, OBJ arg1){
	fprintf(stdout, "TC1: %s\n", functionName((VOIDPTRFUNC)func));
	
	VOIDPTRFUNC retAddr = (VOIDPTRFUNC)(getContinuation());
	
	jStack[AP-2] = (OBJ)func;	
	jStack[AP-1] = (OBJ)retAddr;
	jStack[AP] = arg1;
	SP = AP + 1;
	return func;
}
#define TAILCALL1(func, arg1)\
{ \
	return TAILCALL1_helper((VOIDPTRFUNC)func, arg1);\
}

static inline VOIDPTRFUNC
TAILCALL2_helper(VOIDPTRFUNC func, OBJ arg1, OBJ arg2){
	
	fprintf(stdout, "TC2: %s\n", functionName((VOIDPTRFUNC)func));

	VOIDPTRFUNC retAddr = (VOIDPTRFUNC)(getContinuation());

	jStack[AP-2] = (OBJ)func;	
	jStack[AP-1] = (OBJ)retAddr;
	jStack[AP] = arg1;
	jStack[AP+1] = arg2;

	SP = AP + 2;
	return func;
}
#define TAILCALL2(func, arg1, arg2)\
{ \
	return TAILCALL2_helper((VOIDPTRFUNC)func, arg1, arg2);\
}

static inline VOIDPTRFUNC
RETURN_helper(OBJ retVal){

	RETVAL = retVal;
	VOIDPTRFUNC retAddr = (VOIDPTRFUNC)(getContinuation());
	
	LEAVE_STACK_FRAME();
	//SP = AP - 1;
	//AP = savedAP();

	fprintf(stdout, "RETURN: ");
	js_print(stdout, retVal);
	fprintf(stdout, " -> %s\n", functionName((VOIDPTRFUNC)retAddr));

	return retAddr;
}

#define RETURN(retVal)\
{ \
	return RETURN_helper(retVal);\
}


