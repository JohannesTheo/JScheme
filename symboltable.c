#include "jscheme.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_SYMBOLTABLESIZE 511	// one below a power of 2

static OBJ *symbolTable;
static int symbolTableSize = INITIAL_SYMBOLTABLESIZE;
static int fillSize = 0;

void
initSymbolTable(){

	symbolTableSize = INITIAL_SYMBOLTABLESIZE;
	symbolTable = (OBJ * ) malloc((sizeof(OBJ) * symbolTableSize));
	memset( (void *)symbolTable, 0, (sizeof(OBJ) * symbolTableSize));
}

void
symbolTableRehash(){

	int oldSize = symbolTableSize;
	int newSize = ( oldSize + 1 ) * 2 - 1; // one below the next power of 2
	OBJ *oldSymbolTable = symbolTable;
	OBJ *newSymbolTable;
	int indexInOldTable;

	// allocate new symbolTable
	newSymbolTable = (OBJ *) malloc((sizeof(OBJ) * newSize));
	memset( (void *) newSymbolTable, 0, (sizeof(OBJ) * newSize));

	// rehash old entries and insert in new table
	for(indexInOldTable = 0; indexInOldTable < oldSize; indexInOldTable++){
	
		OBJ oldSymbol = oldSymbolTable[indexInOldTable];
		if( oldSymbol != NULL){
			
			uint32_t newh = FNV1a_string(SYMBOLVAL(oldSymbol));
			int startIndex = newh % newSize;
			int nextIndex = startIndex;
			OBJ try;

			// insert old OBJ with new hash in new table
			for(;;){

				try = newSymbolTable[nextIndex];

				// case 1: Slot empty
				if(try == NULL){
					newSymbolTable[nextIndex] = oldSymbol;
					break;
				}
				
				// case 2: Slot not empty - hash collision
				nextIndex = (nextIndex + 1) % newSize;
				if(nextIndex == startIndex){
					// error: no empty slots found in probing!
					error("FATAL symbol table error - rehash found no empty slot", __FILE__, __LINE__);
				}
			}		
		}
	}

	symbolTable = newSymbolTable;
	symbolTableSize = newSize;
	free(oldSymbolTable);
}

OBJ
symbolTableGetOrAdd(char* symbol){
	
	uint32_t h = FNV1a_string(symbol);
	int startIndex = h % symbolTableSize;
	int nextIndex = startIndex;
	OBJ try;

	for(;;){
	//	printf("sym: %s index: %d\n", symbol, nextIndex);

		try = symbolTable[nextIndex];

		// case 1: Slot empty
		if(try == NULL){
			OBJ newSym = newSymbol(symbol);
			symbolTable[nextIndex] = newSym;
			fillSize++;
			if (fillSize > (symbolTableSize * 3 / 4) ){
				symbolTableRehash();
			}
			return newSym;
		}
		
		// case 2: Slot not empty - found symbol
		if (strcmp(symbol, SYMBOLVAL(try)) == 0){
			return symbolTable[nextIndex];
		}
		
		// case 3: Slot not empty - hash collision
		nextIndex = (nextIndex + 1) % symbolTableSize;
		if(nextIndex == startIndex){
			// error: no empty slots found in probing!
			error("symbol table error - no empty slot", __FILE__, __LINE__);
		}
	}
}

