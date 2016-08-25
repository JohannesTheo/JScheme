/*
 * eval stack
 */

extern OBJ *evalStack;
extern int spIndex; // spIndex = index if next unused slot
extern int stackLimit;
extern OBJ globalEnvironment;

static inline void
PUSH(OBJ o) {
#ifdef DEBUG	
	if(spIndex == stackLimit) {
		error("stack overflow", __FILE__, __LINE__);
	}
#endif
	evalStack[spIndex++] = o;
}

static inline OBJ
POP(){
#ifdef DEBUG
	if(spIndex == 0){
		error("stack underflow", __FILE__, __LINE__);
	}
#endif
	return evalStack[--spIndex];
}

static inline void
POPN(int n){
	spIndex -= n;
}

static inline OBJ
NTH_ARG(int numArgs, int index){
#ifdef DEBUG
	if(spIndex ==0){
		error("stack underflow", __FILE__, __LINE__);
	}
#endif
	return evalStack[spIndex - numArgs + index];
}
