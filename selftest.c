#include "jscheme.h"
#include <stdlib.h>
#include <string.h>

static OBJ
js_readFromString(char *buffer){
	
	OBJ stringStream = newStringStream(buffer);
	OBJ retVal = js_read(stringStream);
	free(stringStream);
	return retVal;
	
}

void
symbolTableFillTest(){

	char ch1, ch2, ch3;

	for(ch1 = 'a'; ch1 < 'z'; ch1++){
		for(ch2 = 'a'; ch2 < 'z'; ch2++){
			for(ch3 = 'a'; ch3 < 'z'; ch3++){
				char testSymbol[4];
				testSymbol[0] = ch1;
				testSymbol[1] = ch2;
				testSymbol[2] = ch3;
				testSymbol[3] = '\0';

				char* testSymbolCopy = malloc(4);
				strcpy(testSymbolCopy, testSymbol);
				symbolTableGetOrAdd(testSymbolCopy);
			}
		}
	}
}


void
globalEnvironmentFillTest(){

	char ch1, ch2, ch3;
	OBJ storeSymbol;
	OBJ searchSymbol, retrieveString;

	// fill env
	for(ch1 = 'a'; ch1 < 'z'; ch1++){
		for(ch2 = 'a'; ch2 < 'z'; ch2++){
			for(ch3 = 'a'; ch3 < 'z'; ch3++){
				char testSymbol[4];
				testSymbol[0] = ch1;
				testSymbol[1] = ch2;
				testSymbol[2] = ch3;
				testSymbol[3] = '\0';

				char *testSymbolCopy = malloc(4);
				strcpy(testSymbolCopy, testSymbol);
				
				storeSymbol = symbolTableGetOrAdd(testSymbolCopy);		
				OBJ theString = newString(testSymbolCopy);
				environmentPut(storeSymbol, theString);
			}
		}
	}

	// test entries
	for(ch1 = 'a'; ch1 < 'z'; ch1++){
		for(ch2 = 'a'; ch2 < 'z'; ch2++){
			for(ch3 = 'a'; ch3 < 'z'; ch3++){
				char testSymbol[4];
				testSymbol[0] = ch1;
				testSymbol[1] = ch2;
				testSymbol[2] = ch3;
				testSymbol[3] = '\0';

				char *testSymbolCopy = malloc(4);
				strcpy(testSymbolCopy, testSymbol);
				searchSymbol = symbolTableGetOrAdd(testSymbolCopy);		

				retrieveString = environmentGet(searchSymbol);
				
				ASSERT( retrieveString->u.any.tag == T_STRING , "bad tag in string");
				ASSERT( strcmp(STRINGVAL(retrieveString), testSymbolCopy) == 0 , "bad value in string");
			}
		}
	}

}

void
selftest(){

//	printf("JS selftest\n\n");

	/*
	 *	symbol table tests
	 */
	{
		OBJ sym1 = symbolTableGetOrAdd("hello");
		ASSERT( ISSYMBOL(sym1), "bad tag in symbol");

		OBJ sym2 = symbolTableGetOrAdd("world");
		ASSERT( ISSYMBOL(sym2), "bad tag in symbol");

		OBJ sym3 = symbolTableGetOrAdd("hello");
		ASSERT( ISSYMBOL(sym3), "bad tag in symbol");

		ASSERT( (sym1 == sym3), "bad symbol table behaviour");
		ASSERT( (sym1 != sym2), "bad symbol table behaviour");
		
		symbolTableFillTest();

		printf("SymTable: " GRN "OK\n" RESET);
	}

	/*
	 * environment tests
	 */
	{
		OBJ hello_key = symbolTableGetOrAdd("hello");
		OBJ world_key = symbolTableGetOrAdd("world");
		OBJ val_AFFE;
		OBJ val_BEBE;
		OBJ val_retrieved;

		val_retrieved = environmentGet(world_key);
		
		// test - no entry for key in environment
		ASSERT(val_retrieved == NULL, "Oo - already knows about the world");

		// insert 2 key, value pairs
		val_AFFE = newInteger(0xAFFE);
		environmentPut(hello_key, val_AFFE);	
		val_BEBE = newInteger(0xBEBE);
		environmentPut(world_key, val_BEBE);

		// test - values of the 2 keys
		val_retrieved = environmentGet(hello_key);
		ASSERT(val_retrieved == val_AFFE, "global environment problem");
		val_retrieved = environmentGet(world_key);
		ASSERT(val_retrieved == val_BEBE, "global environment problem");

		globalEnvironmentFillTest();

		printf("Env: " GRN "OK\n" RESET);
	}

	/*
	 *	reader tests
	 */

	// integer

	OBJ t = js_readFromString("1");
	ASSERT( ISINTEGER(t), "bad tag in integer");
	ASSERT( (INTVAL(t) == 1), "bad value in integer");

	t = js_readFromString(" 12");
	ASSERT( ISINTEGER(t), "bad tag in integer");
	ASSERT( (INTVAL(t) == 12), "bad value in integer");

	t = js_readFromString("23 ");
	ASSERT( ISINTEGER(t), "bad tag in integer");
	ASSERT( (INTVAL(t) == 23), "bad value in integer");
	
	t = js_readFromString(" 123456789 ");
	ASSERT( ISINTEGER(t), "bad tag in integer");
	ASSERT( (INTVAL(t) == 123456789), "bad value in integer");
	
	t = js_readFromString("\n99");
	ASSERT( ISINTEGER(t), "bad tag in integer");
	ASSERT( (INTVAL(t) == 99), "bad value in integer");
	
	t = js_readFromString("99\n");
	ASSERT( ISINTEGER(t), "bad tag in integer");
	ASSERT( (INTVAL(t) == 99), "bad value in integer");
	
	t = js_readFromString("\n99\n");
	ASSERT( ISINTEGER(t), "bad tag in integer");
	ASSERT( (INTVAL(t) == 99), "bad value in integer");
	
	t = js_readFromString("\t\n\t  42\t");
	ASSERT( ISINTEGER(t), "bad tag in integer");
	ASSERT( (INTVAL(t) == 42), "bad value in integer");
	
	t = js_readFromString("     \n \t        12       \n\t                       ");
	ASSERT( ISINTEGER(t), "bad tag in integer");
	ASSERT( (INTVAL(t) == 12), "bad value in integer");
	
	t = js_readFromString("      100            ");
	ASSERT( ISINTEGER(t), "bad tag in integer");
	ASSERT( (INTVAL(t) == 100), "bad value in integer");
	
	// symbol

	t = js_readFromString("a");
	ASSERT( ISSYMBOL(t), "bad tag in symbol");
	ASSERT( (strcmp(SYMBOLVAL(t), "a") == 0), "bad value in symbol");
	
	t = js_readFromString(" abc ");
	ASSERT( ISSYMBOL(t), "bad tag in symbol");
	ASSERT( (strcmp(SYMBOLVAL(t), "abc") == 0), "bad value in symbol");
	
	t = js_readFromString("\n  a\n");
	ASSERT( ISSYMBOL(t), "bad tag in symbol");
	ASSERT( (strcmp(SYMBOLVAL(t), "a") == 0), "bad value in symbol");
	
	t = js_readFromString("  a123\"string\"   ");
	ASSERT( ISSYMBOL(t), "bad tag in symbol");
	ASSERT( (strcmp(SYMBOLVAL(t), "a123") == 0), "bad value in symbol");
		
	// string

	t = js_readFromString("\"a\"");
	ASSERT( ISSTRING(t), "bad tag in string");
	ASSERT( (strcmp(STRINGVAL(t), "a") == 0), "bad value in string");

	t = js_readFromString(" \t \n  \"abc\"     \t     \n  ");
	ASSERT( ISSTRING(t), "bad tag in string");
	ASSERT( (strcmp(STRINGVAL(t), "abc") == 0), "bad value in string");
	
	t = js_readFromString("\"abc\\nd\\t\"");
	ASSERT( ISSTRING(t), "bad tag in string");
	ASSERT( (strcmp(STRINGVAL(t), "abc\nd\t") == 0), "bad value in string");
	
	t = js_readFromString("\"abc123\"");
	ASSERT( ISSTRING(t), "bad tag in string");
	ASSERT( (strcmp(STRINGVAL(t), "abc123") == 0), "bad value in string");

	// cons
	t = js_readFromString("()");
	ASSERT( ISCONS(t) == 0, "nil read failed");
	ASSERT( ISNIL(t), "nil read failed");

	t = js_readFromString("    (   )");
	ASSERT( ISNIL(t), "nil read failed");

	t = js_readFromString("(     )");
	ASSERT( ISNIL(t), "nil read failed");

	t = js_readFromString("(1)");
	ASSERT( ISCONS(t), "list read failed");
	ASSERT( ISINTEGER( CAR(t)), "list read failed");
	ASSERT( ISNIL( CDR(t)), "list read failed");
	ASSERT( INTVAL( CAR(t)) == 1, "list read failed");

	t = js_readFromString("(1 2)");
	ASSERT( ISCONS(t), "list read failed");
	ASSERT( ISINTEGER( CAR(t)), "list read failed");
	ASSERT( ISCONS( CDR(t)), "list read failed");
	ASSERT( ISINTEGER( CAR( CDR(t))), "list read failed");
	ASSERT( ISNIL( CDR( CDR(t))), "list read failed");

	t = js_readFromString("(a 1 (  b \"string\" ) 2)");
	// CAR: a CDR: 1 ( b "string" ) 2 )
	ASSERT( ISCONS(t), "list read failed");
	ASSERT( ISSYMBOL( CAR(t)), "list read failed");
	ASSERT( (strcmp( SYMBOLVAL( CAR(t)), "a") == 0), "list read failed");
	ASSERT( ISCONS( CDR(t)), "list read failed");
	
	// CAR: 1 CDR: ( b "string" ) 2 )
	ASSERT( ISINTEGER( CAR( CDR(t))), "list read failed");
	ASSERT( INTVAL( CAR( CDR(t))) == 1, "list read failed");
	ASSERT( ISCONS( CDR( CDR(t))), "list read failed");
	
	// CAR: ( b "string" ) CDR: 2 ) 
	// NESTED: CAR: b CDR: "string" )
	ASSERT( ISCONS( CAR( CDR( CDR(t)))), "list read failed");
	ASSERT( ISSYMBOL( CAR( CAR( CDR( CDR(t))))), "list read failed");
	ASSERT( (strcmp( SYMBOLVAL( CAR( CAR( CDR( CDR(t))))), "b") == 0), "list read failed");
	ASSERT( ISCONS( CDR( CAR( CDR( CDR(t))))), "list read failed");

	// NESTED: CAR: "STRING" CDR: )
	ASSERT( ISSTRING( CAR( CDR( CAR( CDR( CDR(t)))))), "list read failed");
	ASSERT( (strcmp( STRINGVAL( (CAR( CDR( CAR( CDR( CDR(t))))))), "string") == 0), "list read failed");
	ASSERT( ISNIL( CDR( CDR( CAR( CDR( CDR(t)))))), "list read failed");
	
	// CAR: 2 CDR: )
	ASSERT( ISINTEGER( CAR( CDR( CDR( CDR(t))))), "list read failed");
	ASSERT( INTVAL( CAR( CDR( CDR( CDR(t))))) == 2, "list read failed");
	ASSERT( ISNIL( CDR( CDR( CDR( CDR(t))))), "list read failed");
	
	// negative test
	// TODO


	/*
	 *	test multiple strings with multiple JScheme members
	 */ 
	OBJ t2 = NULL;
	OBJ t_stream;
	char* test_string;
	int t_count;

	// 1. multiple integers in a row

	test_string = ("1 1     1\t      \t1\t");
	//printf("\nTest-String: \"%s\"\n", test_string);
	t_stream = newStringStream(test_string);
	t_count = 0;

	do{	
		t2 = js_read(t_stream);
		ASSERT( ISINTEGER(t2), "bad tag in integer");
		ASSERT( INTVAL(t2) == 1, "bad value in integer");
		t_count ++;
		
	} while(!thisIsTheEnd(t_stream));
	ASSERT( (t_count == 4), "bad number of members in multi-read test");
	free(t_stream);

	// 2. multiple symbols

	test_string = ("a a     a\t      \ta\t");
	//printf("\nTest-String: \"%s\"\n", test_string);
	t_stream = newStringStream(test_string);
	t_count = 0;

	do{	
		t2 = js_read(t_stream);
		ASSERT( ISSYMBOL(t2), "bad tag in symbol");
		ASSERT( (strcmp(SYMBOLVAL(t2), "a") == 0), "bad value in symbol");
		t_count ++;
		
	} while(!thisIsTheEnd(t_stream));
	ASSERT( (t_count == 4), "bad number of members in multi-read test");
	free(t_stream);

	// 3. multiple strings
	
	test_string = ("\"a\"    \"a\"\t\"a\"\"a\"");
	//printf("\nTest-String: \"%s\"\n", test_string);
	t_stream = newStringStream(test_string);
	t_count = 0;

	do{	
		t2 = js_read(t_stream);
		ASSERT( ISSTRING(t2), "bad tag in string");
		ASSERT( (strcmp(STRINGVAL(t2),"a") == 0), "bad value in string");
		t_count ++;
		
	} while(!thisIsTheEnd(t_stream));
	ASSERT( (t_count == 4), "bad number of members in multi-read test");
	free(t_stream);

	/*
	 *	test of mixed members in a row
	 */
	
	// 1. integer, string and sybmols mix with  white spaces

	test_string = (" 1 \"a\" b   c  ");
	t_stream = newStringStream(test_string);
	t_count = 0;

	do {	
		t2 = js_read(t_stream);
		
		switch(t_count){
			case 0:
				ASSERT( ISINTEGER(t2), "bad tag in integer");
				ASSERT( INTVAL(t2) == 1, "bad value in integer");
				break;
			case 1:
				ASSERT( ISSTRING(t2), "bad tag in string");
				ASSERT( (strcmp(STRINGVAL(t2), "a") == 0), "bad value in string");
				break;
			case 2:
				ASSERT( ISSYMBOL(t2), "bad tag in symbol");
				ASSERT( (strcmp(SYMBOLVAL(t2), "b") == 0) , "bad value in symbol");
				break;
			case 3:
				ASSERT( ISSYMBOL(t2), "bad tag in symbol");
				ASSERT( (strcmp(SYMBOLVAL(t2), "c") == 0), "bad value in symbol");
				break;
			default:
				break;
		}
		t_count ++;
		
	} while(!thisIsTheEnd(t_stream));
	ASSERT( (t_count == 4), "bad number of members in multi-read test");
	free(t_stream);

	// 2. integer, string and sybmols mix without whitespace

	test_string = ("1\"a\"b");
	t_stream = newStringStream(test_string);
	t_count = 0;

	do {	
		t2 = js_read(t_stream);
		
		switch(t_count){
			case 0:
				ASSERT( ISINTEGER(t2), "bad tag in integer");
				ASSERT( INTVAL(t2) == 1, "bad value in integer");
				break;
			case 1:
				ASSERT( ISSTRING(t2), "bad tag in string");
				ASSERT( (strcmp(STRINGVAL(t2), "a") == 0), "bad value in string");
				break;
			case 2:
				ASSERT( ISSYMBOL(t2), "bad tag in symbol");
				ASSERT( (strcmp(SYMBOLVAL(t2), "b") == 0) , "bad value in symbol");
				break;
			default:
				break;
		}
		t_count ++;
		
	} while(!thisIsTheEnd(t_stream));
	ASSERT( (t_count == 3), "bad number of members in multi-read test");
	free(t_stream);

	// 3. integer, string and sybmols mix without whitespace 2

	test_string = ("b\"a\"1");
	t_stream = newStringStream(test_string);
	t_count = 0;

	do {	
		t2 = js_read(t_stream);
		
		switch(t_count){
			case 0:
				ASSERT( ISSYMBOL(t2), "bad tag in symbol");
				ASSERT( (strcmp(SYMBOLVAL(t2), "b") == 0) , "bad value in symbol");
				break;
			case 1:
				ASSERT( ISSTRING(t2), "bad tag in string");
				ASSERT( (strcmp(STRINGVAL(t2), "a") == 0), "bad value in string");
				break;
			case 2:
				ASSERT( ISINTEGER(t2), "bad tag in integer");
				ASSERT( INTVAL(t2) == 1, "bad value in integer");
				break;
			default:
				break;
		}
		t_count ++;
		
	} while(!thisIsTheEnd(t_stream));
	ASSERT( (t_count == 3), "bad number of members in multi-read test");
	free(t_stream);


	/*
	 *	END OF SELFTEST :)
	 */
	printf(GRN "selftest passed\n" RESET);
}
