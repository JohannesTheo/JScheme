#include "jscheme.h"

#ifdef DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int indentLevel = 0;
int oldIndentLevel = 0;
int PAUSE_INDENT_FLAG = 0;

struct debugOption *DETAILED_TYPES; 
struct debugOption *EVAL_TRACE;

static struct debugOption*
newDebugOption(char* name,char* identifier, int initialState){

	struct debugOption *newOption;
	newOption = (struct debugOption*)(malloc( sizeof( struct debugOption)));
	newOption->name = name;
	newOption->identifier = identifier;
	newOption->state = initialState;
	return newOption;
}

static char*
isOnOff(int option){
	
	if( option == 0) return RED"OFF"RESET;
	return GRN"ON"RESET;
}

static void
printDebugOptions(){

	fprintf(stdout, "\navailable debug options: \n");
	fprintf(stdout, "----------------------------\n");
	fprintf(stdout, "%s -> %s: %s\n", DETAILED_TYPES->identifier, DETAILED_TYPES->name, isOnOff(DETAILED_TYPES->state) );
	fprintf(stdout, "%s -> %s: %s\n", EVAL_TRACE->identifier, EVAL_TRACE->name, isOnOff(EVAL_TRACE->state) );
	fprintf(stdout, "----------------------------\n");
}

static void
toggleOption(struct debugOption *option) {

	if( option->state ){
	       	option->state = 0;
	}else{
		option->state = 1;
	}
}

static void
detailedTypes(){
	toggleOption(DETAILED_TYPES);
	printDebugOptions();
}

static void
evalTrace(){
	toggleOption(EVAL_TRACE);
	printDebugOptions();
}

void
initDebugOptions(){
	DETAILED_TYPES = newDebugOption( "DETAILED_TYPES","DT", 0);
	EVAL_TRACE = newDebugOption( "EVAL_TRACE","ET", 0);
}

void
switchDebugOptions( OBJ debugOption){

	if( !ISSYMBOL(debugOption) ){
		error("Not a js_symbol as debugOption", __FILE__, __LINE__);
	}

	char *option = SYMBOLVAL(debugOption);

	if( (strcmp(option,DETAILED_TYPES->identifier) == 0) ||	(strcmp(option, DETAILED_TYPES->name) == 0)){
		detailedTypes();
	}else if( (strcmp(option, EVAL_TRACE->identifier) == 0) || (strcmp(option, EVAL_TRACE->name) == 0)){
		evalTrace();
	}else{
		fprintf(stdout, "\nPlease use one of the");
		printDebugOptions();
	}			
}
#endif
