/*	
 *
 *	*****************************
 * 	*     The JScheme Stack     *
 *	*****************************
 *
 *	SP -> points to the first free slot on stack
 *	AP -> points to the first argument in the current stack frame
 *	BP -> points to the first element in the current stack frame
 *
 *	RETVAL -> used as return value register in continuation passing style
 *
 * 	
 * 	THE JSTACK LAYOUT:
 *
 * 	abstract layout of a stack frame:
 *	| ------------------------------- |
 *	| 11 :                            | <- SP
 *	| 10 :         LOCAL N            |
 *	| 10 :           ...              |
 *	| 10 :         LOCAL 1            |
 *	| 10 :          ARG N             |
 *	| 10 :           ....             |
 *	| 10 :          ARG 1             |
 *	|  4 :       environment          | <- AP
 *	|  3 :   continuation / return    |
 *	|  2 :         callee             |
 *	|  1 :        (save AP)           | 
 *	|  0 :        (saved BP)          | <- BP
 *	| ------------------------------- |
 *	
 *	***********************************************************************
 
 *	#comment:
 *
 *	Every CALL will create a NEW stack frame. RETURN will allways leave a stack frame.
 *	This can be a small overhead but it simplifies the usage pattern a lot! :)
 *	
 *	1 CALL -> RETURN most of the time
 *	2 TAILCALL when reusing a stack frame ( will drop LOCALS so pass them as args if needed )
 *	3 LOCALS can be created in the stack frame they are needed. Because following CALLs will 
 *	  create another stack frame LOCALS are safe in the callers stack frame.
 
 *	***********************************************************************
 *
 *	CALL: 
 *	- creates a new stack frame (save oldAP, oldBP and set BP to new stack frame)
 *	- pushes callee
 *	- pushes continuation/return address
 *	- pushes ARG1 to ARG N
 *		
 *	TAILCALL:
 *	- replace calle of current stack frame with new callee
 *	- reuse continuation/return of current stack frame
 *	- set ARG1 to ARG N
 *	- drops locals!
 *
 *	RETURN:
 *	- set RETVAL to the return value
 *	- calls the continuation of current stack frame
 *	- leaves the stack frame ( restores previous stack frame with oldBP and oldAP )
 *
 * 
 * 	# IF a function needs some locals it must create and manage them
 *
 *	CREATE_LOCALS N:
 *	- creates n locals initilaised with js_nil
 *
 *	LOCAL N:
 *	- returns the local n in the current stack frame
 *
 *	SET_LOCAL N O:
 *	- sets the local n to o in the current stack frame
 *
 *
 *	initial stack:
 *	----------------------------------
 *	|  0 :                           | <- SP <- AP <- BP
 *	----------------------------------
 *
 * 	in CP_jREPL:
 *	----------------------------------
 *	|  0 :                           | <- SP 
 *	|  4 :       #file stream        | <- AP
 *	|  3 :           NULL            | (continuation/return)
 *	|  2 :         CP_jREPL          |
 *	|  1 :            0              | (saved AP)
 *	|  0 :            0              | (saved BP) <- BP
 *	----------------------------------
 *	
 *	in CP_js_eval for: (define a 100)
 *	-> CALL( CP_js_eval, GLOBAL, expr, CP_jREPL2)
 *	----------------------------------
 *	| 11 :                            | <- SP
 *	| 10 :            cons            | -> (define a 100)
 *	|  9 :  GLOBAL: (0x7ff7fa003600)  | <- AP
 *	|  8 :          CP_jREPL2         | (continuation/return)
 *	|  7 :         CP_js_eval         |
 *	|  6 :              4             | (saved AP)
 *	|  5 :              0             | (saved BP) <- BP
 *	| ------------------------------- |
 *	|  4 :        #file stream        |
 *	|  3 :            NULL            |
 *	|  2 :          CP_jREPL          |
 *	|  1 :              0             |
 *	|  0 :              0             |
 *	----------------------------------
 *	
 *	...
 *	-> TAILCALL CP_evalCons 
 *	-> Create LOCALS for functionsSlot, restArgs, numArgs, etc...
 *	...
 *
 *	in CP_evalCons2
 *	----------------------------------
 *	| 17 :                            | <- SP
 *	| 16 :             nil            |
 *  	| 15 :             nil            |
 *  	| 14 :             nil            |
 *  	| 13 :              0             |			LOCAL 2: numArgs
 *  	| 12 :            cons            | -> (a 100)		LOCAL 1: restArgs
 *	| 11 :      builtin (define)      |			LOCAL 0: THE EVALUATED FUNCTION SLOT
 *	| 10 :            cons            | -> (define a 100)	THE ORIGINAL cons
 *	|  9 :  GLOBAL: (0x7fdb74007600)  | <- AP
 *	|  8 :          CP_jREPL2         | (continuation/return)
 *	|  7 :         CP_evalCons        |
 *	|  6 :              4             | (saved AP)
 *	|  5 :              0             | (saved BP) <- BP
 *	| ------------------------------- |
 *	|  4 :        #file stream        |
 *	|  3 :            NULL            |
 *	|  2 :          CP_jREPL          |
 *	|  1 :              0             |
 *	|  0 :              0             |
 *	----------------------------------
 *
 */

extern OBJ *jStack;
extern int SP;
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


	ASSERT( ((AP + 2 + n0based) < SP), "get local arg index error");
	ASSERT( ((AP + 2 + n0based) > 0), "arg index error");
	 
	return jStack[AP + 2 + n0based];
}

static inline void
SET_LOCAL(int n0based, OBJ o){

	
	ASSERT( ((AP + 2 + n0based) < SP), "set local arg index error");
	ASSERT( ((AP + 2 + n0based) > 0), "arg index error");
	
	jStack[AP + 2 + n0based] = o;
}

/*
 * Continuation helpers
 */

static inline int
savedAP(){
	
	ASSERT( ((BP + 1) < SP), "saved AP index error");
	ASSERT( ((BP + 1) > 0), "saved AP index error");
	return (int)jStack[BP+1];
}

static inline VOIDPTRFUNC
getContinuation(){
	ASSERT( ((BP + 3) < SP), "CONT index error");
	ASSERT( ((BP + 3) > 0), "CONT index error");
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

	DEBUGCODE(PRINT_STACK->state, fprintf(stdout, "C1: %s Cont: %s\n",  functionName((VOIDPTRFUNC)func), functionName((VOIDPTRFUNC)continuation)) );
	
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
	
	DEBUGCODE(PRINT_STACK->state, fprintf(stdout, "C2: %s Cont: %s\n",  functionName((VOIDPTRFUNC)func), functionName((VOIDPTRFUNC)continuation)) );
	
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
	DEBUGCODE(PRINT_STACK->state, fprintf(stdout, "TC1: %s\n", functionName((VOIDPTRFUNC)func)) );
	
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
	
	DEBUGCODE( PRINT_STACK->state, fprintf(stdout, "TC2: %s\n", functionName((VOIDPTRFUNC)func)) );

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

	DEBUGCODE(PRINT_STACK->state,	fprintf(stdout, "RETURN: ") );
	DEBUGCODE(PRINT_STACK->state,	js_print(stdout, retVal, 1) );
	DEBUGCODE(PRINT_STACK->state,	fprintf(stdout, " -> %s\n", functionName((VOIDPTRFUNC)retAddr)) );

	return retAddr;
}

#define RETURN(retVal)\
{ \
	return RETURN_helper(retVal);\
}


