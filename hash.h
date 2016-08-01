/*
 *	first simple hashing implementation FNV1a for the symbol table
 *	TO-DO: refactor! 
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

const static uint32_t FNV_OFFSET_32 = 0x811C9DC5;	// 2166136261;
const static uint32_t FNV_PRIME_32 = 0x01000193; 	//   16777619; 
const static int HLIMIT = 4;				// hash only the n first byte

static inline uint32_t
FNV1a_string(char *string){

	uint32_t hash = FNV_OFFSET_32;
	int index = 0;
	
	while(index < HLIMIT){
		
		if(string[index] == '\0'){
			break;
		}
		hash = (hash * FNV_PRIME_32) ^ string[index++];
//		printf("hash after %d: %u\n", (index - 1), hash);
	}

	return hash;
}
