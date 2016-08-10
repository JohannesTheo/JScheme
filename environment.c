#include "jscheme.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#define INITIAL_ENVIRONMENTSIZE 511	// one below a power of 2

OBJ globalEnvironment;
static int globalEnvironmentSize = INITIAL_ENVIRONMENTSIZE;
static int globalEnvironmentFillSize = 0;

OBJ
newEnvironment(int numSlots, OBJ parentEnv){

	int size = offsetof(struct jsEnvironment, slots) + sizeof(JS_ENV_ENTRY)*numSlots;

	OBJ newEnv = (OBJ)(malloc( size));
	newEnv->u.environment.tag = T_LOCALENVIRONMENT;
	newEnv->u.environment.parentEnvironment = parentEnv;
	newEnv->u.environment.numSlots = numSlots;
	return newEnv;
}

OBJ
newGlobalEnvironment(int size){

	OBJ newEnv;

	newEnv = newEnvironment(size, NULL);
	newEnv->u.environment.tag = T_GLOBALENVIRONMENT;
	memset((void *)(&(newEnv->u.environment.slots)), 0, (sizeof(JS_ENV_ENTRY) * size));
	return newEnv;
}

void
initGlobalEnvironment(){
	globalEnvironment = newGlobalEnvironment(INITIAL_ENVIRONMENTSIZE);
}

void
globalEnvironmentRehash(){

	int oldSize = globalEnvironmentSize;
	int newSize = ( oldSize + 1 ) * 2 - 1; // one blow the next power of 2
	OBJ oldGlobalEnv = globalEnvironment;
	OBJ newGlobalEnv;
	int indexInOldTable;

	// allocate new global env
	newGlobalEnv = newGlobalEnvironment(newSize);

	for(indexInOldTable = 0; indexInOldTable < oldSize; indexInOldTable++){
	
		JS_ENV_ENTRY *oldEntry = &(oldGlobalEnv->u.environment.slots[indexInOldTable]);
		if(oldEntry->key != NULL){
		
			uint32_t newHash = (int)(oldEntry->key);
			int startIndex = newHash % newSize;
			int nextIndex = startIndex;

			// insert old entry with new "hash" in new env
			for(;;){
			
				OBJ try;
				try = newGlobalEnv->u.environment.slots[nextIndex].key;

				// case 1: slot is empty
				if(try == NULL){
					newGlobalEnv->u.environment.slots[nextIndex].key = oldEntry->key;
					newGlobalEnv->u.environment.slots[nextIndex].value = oldEntry->value;
					break; // inner fill loop
				}
				// case 2: slot not empty, hash collision -> probing
				nextIndex = ( nextIndex + 1) % newSize;
				if(nextIndex == startIndex){
					// error: no empty slot
					error("FATAL global environment error - rehash found no empty slot", __FILE__, __LINE__);
				}
			}		
		}
	}
	globalEnvironment = newGlobalEnv;
	globalEnvironmentSize = newSize;
	free(oldGlobalEnv);
}

void
globalEnvironmentPut(OBJ storedKey, OBJ storedValue){
	
	uint32_t h = (int) storedKey;
	int startIndex = h % globalEnvironmentSize;
	int nextIndex = startIndex;
	OBJ try;

	for(;;){
	
		try = globalEnvironment->u.environment.slots[nextIndex].key;

		// case 1: already there -> replace
		if(try == storedKey){

			globalEnvironment->u.environment.slots[nextIndex].value = storedValue;
			return;
		}
		// case 2: new -> insert
		if(try == NULL){
			
			globalEnvironment->u.environment.slots[nextIndex].key = storedKey;
			globalEnvironment->u.environment.slots[nextIndex].value = storedValue;
			globalEnvironmentFillSize++;

			if( globalEnvironmentFillSize > ( globalEnvironmentSize * 3 / 4)){
				globalEnvironmentRehash();
			}
			return;
		}
		// case 3: hash collision -> probing
		nextIndex = ( nextIndex + 1) % globalEnvironmentSize;
		if(nextIndex == startIndex){
			// error: no empty slots found 
			error("global environment error - no empty slot", __FILE__, __LINE__);
		}
	}
}

OBJ
globalEnvironmentGet(OBJ searchedKey){
	
	uint32_t h = (int) searchedKey;
	int startIndex = h % globalEnvironmentSize;
	int nextIndex = startIndex;

	OBJ try;

	for(;;){

		try = globalEnvironment->u.environment.slots[nextIndex].key;

		// case 1: found key -> return value
		if(try == searchedKey){
			return globalEnvironment->u.environment.slots[nextIndex].value;
		}
		// case 2: no key -> return NULL
		if(try == NULL){
			return NULL;
		}
		// case 3: hash collision -> probing
		nextIndex = (nextIndex + 1) % globalEnvironmentSize;
		if(nextIndex == startIndex){
			// error: all probed nothing found
			error("global environment error - no empty slot", __FILE__, __LINE__);
		}
	}
}

void
localEnvironmentPut(OBJ env, OBJ storedKey, OBJ storedValue){
	
	OBJ nextKeyInEnv;
	int numEnvSlots = env->u.environment.numSlots;

	for(int i = 0; i < numEnvSlots; i++){
		
		nextKeyInEnv = env->u.environment.slots[i].key;

		// case 1: key already there -> replace
		if( nextKeyInEnv == storedKey) {
			env->u.environment.slots[i].value = storedValue;
		}
		// case 2: empty slot -> insert key, value
		if( nextKeyInEnv == NULL){
			env->u.environment.slots[i].key = storedKey;
			env->u.environment.slots[i].value = storedValue;
		}
	}
	// case 3: no slot is empty -> env is full
	error("local environment error - no empty slot", __FILE__, __LINE__);
}

OBJ
localEnvironmentGet(OBJ env, OBJ searchedKey){
	
	/*
	 *	return the searched key from the closest env or NULL if not present ( from globalEnv )
	 */
	OBJ nextKeyInEnv;
	int numEnvSlots = env->u.environment.numSlots;

	for(int i = 0; i < numEnvSlots; i++){
		
		nextKeyInEnv = env->u.environment.slots[i].key;
		if( nextKeyInEnv == searchedKey){
			return env->u.environment.slots[i].value;
		}
	}

	OBJ parentEnv = env->u.environment.parentEnvironment;
	return environmentGet(parentEnv, searchedKey);
}

void
environmentPut(OBJ env, OBJ storedKey, OBJ storedValue){

	if( TAG(env) == T_GLOBALENVIRONMENT ){
		return globalEnvironmentPut(storedKey, storedValue);
	} 
	if( TAG(env) == T_LOCALENVIRONMENT ){
		return localEnvironmentPut(env, storedKey, storedValue);
	} 
	js_error("environment put: not an environment", env);
}

OBJ
environmentGet(OBJ env, OBJ searchedKey){

	if( TAG(env) == T_GLOBALENVIRONMENT ){
		return globalEnvironmentGet(searchedKey);
	} 
	if( TAG(env) == T_LOCALENVIRONMENT ){
		return localEnvironmentGet(env, searchedKey);
	} 
	js_error("environment get: not an environment", env);

	// NOT REACHED
	return js_nil;
}

