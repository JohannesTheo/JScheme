/*
 * eval stack
 */

extern OBJ *jStack;
extern int SP 			// spIndex = index of next unused slot
extern int AP
extern int stackLimit;
extern OBJ globalEnvironment;

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
	if(spIndex == 0){
		error("stack underflow", __FILE__, __LINE__);
	}
#endif
	return jStack[--SP];
}

static inline void
POPN(int n){
	spIndex -= n;
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
