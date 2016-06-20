#include "jscheme.h"
#include <stdlib.h>


static void
print_result(int passed){

	if(passed){
		printf(GRN "PASSED\n" RESET);
	}else{
		printf(RED "FAILED\n" RESET);
	}
}


static OBJ
js_readFromString(char *buffer){
	
	OBJ stringStream = newStringStream(buffer);
	return js_read(stringStream);
}

void
selftest(){

	printf("JS selftest\n\n");

	OBJ test = js_readFromString("1");
	print_result((test->u.integer.tag == T_INTEGER) && (test->u.integer.intVal ==1));

	test = js_readFromString("2");
	print_result((test->u.integer.tag == 3) && (test->u.integer.intVal ==1));
	
	test = js_readFromString("       1");
	print_result((test->u.integer.tag == 3) && (test->u.integer.intVal ==1));

	test = js_readFromString("1     ");
	print_result((test->u.integer.tag == 3) && (test->u.integer.intVal ==1));

	test = js_readFromString("  1     ");
	print_result((test->u.integer.tag == 3) && (test->u.integer.intVal ==1));
	
	test = js_readFromString("1\n");
	print_result((test->u.integer.tag == 3) && (test->u.integer.intVal ==1));

	test = js_readFromString("\n 1 \n");
	print_result((test->u.integer.tag == 3) && (test->u.integer.intVal ==1));

	test = js_readFromString("a");
	print_result((test->u.symbol.tag == 4) && (*(test->u.symbol.symbolVal) == 'a'));
	
	test = js_readFromString("\"a\"");
	print_result((test->u.string.tag == 5) && (*(test->u.string.stringVal) == 'a'));

	/*
	 *	Test strings with multiple JScheme members
	 */

	char* test_string = ("1 2 3");
	printf("\nTest-String: \"%s\"\n", test_string);
	OBJ multi_test_stream = newStringStream(test_string);

	do{	
		OBJ result = js_read(multi_test_stream);
		const char *tag = tag_lookup[result->u.any.tag];
		jscheme_int64 intVal = result->u.integer.intVal;
		int nextIndex = multi_test_stream->u.stringStream.index;
		printf(GRN "M-TEST>" RESET " tag: %s intVal: %ld nextIndex: %d\n", tag, intVal, nextIndex);
		
	} while(!thisIsTheEnd(multi_test_stream));

//	error("Test Error", __FILE__, __LINE__);
//	ASSERT(1,"TRUE");
//	ASSERT(0,"FALSE");
	printf("\n\nEnd of selftest\n\n");
}
