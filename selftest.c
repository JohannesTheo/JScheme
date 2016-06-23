#include "jscheme.h"
#include <stdlib.h>
#include <string.h>

static OBJ
js_readFromString(char *buffer){
	
	OBJ stringStream = newStringStream(buffer);
	return js_read(stringStream);
}

void
selftest(){

//	printf("JS selftest\n\n");

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

	/*
	 *	END OF SELFTEST :)
	 */
	printf(GRN "selftest passed\n" RESET);
}
