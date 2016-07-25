#include "jscheme.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_ENVIRONMENTSIZE 511	// one below a power of 2


struct environmentEntry{
	OBJ key;	// must be a symbol
	OBJ value;	// can be any OBJ
};
typedef struct environmentEntry ENV_ENTRY;


static ENV_ENTRY *globalEnvironment;
static int globalEnvironmentSize = INITIAL_ENVIRONMENTSIZE;
static int globalEnvironmentFillSize = 0;


void
initGlobalEnvironment(){

	globalEnvironmentSize = INITIAL_ENVIRONMENTSIZE;
	globalEnvironment = (ENV_ENTRY *) malloc(sizeof(ENV_ENTRY) * globalEnvironmentSize);
	memset((void *) globalEnvironment, 0, (sizeof(ENV_ENTRY) * globalEnvironmentSize));
}

void
environmentPut(OBJ storedKey, OBJ storedValue){

	int h = (int) storedKey;
	int startIndex = h % globalEnvironmentSize;
	int nextIndex = startIndex;
	OBJ try;

	for(;;){
	
		try = globalEnvironment[nextIndex].key;
		
		// case 1: already there -> replace
		if(try == storedKey){

			globalEnvironment[nextIndex].value = storedValue;
			return;
		}
		// case 2: new -> insert
		if(try == NULL){
			
			globalEnvironment[nextIndex].key = storedKey;
			globalEnvironment[nextIndex].value = storedValue;
			globalEnvironmentFillSize++;

			if( globalEnvironmentFillSize > ( globalEnvironmentSize * 3 / 4)){
				// TODO rehash...
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
environmentGet(OBJ searchedKey){
	
	int h = (int) searchedKey;
	int startIndex = h % globalEnvironmentSize;
	int nextIndex = startIndex;

	OBJ try;

	for(;;){

		try = globalEnvironment[nextIndex].key;

		// case 1: found key -> return value
		if(try == searchedKey){
			return globalEnvironment[nextIndex].value;
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

