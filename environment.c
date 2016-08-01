#include "jscheme.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

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
globalEnvironmentRehash(){

	int oldSize = globalEnvironmentSize;
	int newSize = ( oldSize + 1 ) * 2 - 1; // one blow the next power of 2
	ENV_ENTRY *oldGlobalEnvironment = globalEnvironment;
	ENV_ENTRY *newGlobalEnvironment;
	int indexInOldTable;

	// allocate new global env
	newGlobalEnvironment = (ENV_ENTRY *) malloc(sizeof(ENV_ENTRY) * newSize);
	memset((void *) newGlobalEnvironment, 0, (sizeof(ENV_ENTRY) * newSize));

	for(indexInOldTable = 0; indexInOldTable < oldSize; indexInOldTable++){
	
		ENV_ENTRY *oldEntry = &(oldGlobalEnvironment[indexInOldTable]);
		if(oldEntry->key != NULL){
		
			uint32_t newHash = (int)(oldEntry->key);
			int startIndex = newHash % newSize;
			int nextIndex = startIndex;

			// insert old entry with new "hash" in new env
			for(;;){
			
				OBJ try;
				try = newGlobalEnvironment[nextIndex].key;

				// case 1: slot is empty
				if(try == NULL){
					newGlobalEnvironment[nextIndex].key = oldEntry->key;
					newGlobalEnvironment[nextIndex].value = oldEntry->value;
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
	globalEnvironment = newGlobalEnvironment;
	globalEnvironmentSize = newSize;
	free(oldGlobalEnvironment);
}

void
environmentPut(OBJ storedKey, OBJ storedValue){
	
	uint32_t h = (int) storedKey;
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
environmentGet(OBJ searchedKey){
	
	uint32_t h = (int) searchedKey;
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

