#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jscheme.h"

#ifdef DEBUG
void
printIndent(int indentLevel){
	
	for(int i = 0; i < indentLevel; i++){
		fprintf(stdout, INDENT);
	}
}

void
printLocalEnv(OBJ localEnv){

	OBJ key, value;
	fprintf(stdout, " -> %p\n", localEnv);
	printIndent(indentLevel);
	fprintf(stdout, "-----------------------------------\n");
	for(int i = 0; i < localEnv->u.environment.numSlots; i++){
			
		printIndent(indentLevel);
		fprintf(stdout, "%d : ", i);
		key  = localEnv->u.environment.slots[i].key;
		if(key != NULL){	
			value  = localEnv->u.environment.slots[i].value;
			fprintf(stdout, "key: " GRN);
			js_print(stdout, key);
			fprintf(stdout,RESET " value: " GRN);
			js_print(stdout, value);
			fprintf(stdout, RESET "\n");	
		} else {
			fprintf(stdout, "NULL\n");
		}
	}
	printIndent(indentLevel);
	fprintf(stdout, "-----------------------------------\n\n");
}

char* 
functionName(VOIDPTRFUNC addr){

	extern VOIDPTRFUNC CP_jREPL(), CP_jREPL2();
	extern VOIDPTRFUNC CP_js_eval();

	if(addr == (VOIDPTRFUNC)(CP_jREPL)) return "CP_jREPL";
	if(addr == (VOIDPTRFUNC)(CP_jREPL2)) return "CP_jREPL2";
	if(addr == (VOIDPTRFUNC)(CP_js_eval)) return "CP_js_eval";
	if(addr == NULL) return "NULL";

	static char buffer[64];
	sprintf(buffer, "(?func: %p)", addr);
	return buffer;
}

char*
objName(OBJ o){


	enum tag kindOf = o->u.any.tag;
	switch(kindOf){
		case T_NIL:
			return "nil";
		case T_TRUE:
			return "#t";
		case T_FALSE:
			return "#f";
		case T_INTEGER:
			{
			char* intBuffer = (char*) malloc(64);
			sprintf(intBuffer, "%lu", o->u.integer.intVal);
			return intBuffer;
			}
		case T_STRING:
			{
			char* string  = o->u.string.stringVal;
			int size = strlen(string);
			char* strBuffer = (char*) (malloc( (size * sizeof(char))+2));
			sprintf(strBuffer, "\"%s\"", string);
			return strBuffer;
			}
		case T_SYMBOL:
			return o->u.symbol.symbolVal;
		case T_CONS:
			return "cons";
		case T_BUILTINFUNCTION:
			{
			char* string  = o->u.builtinFunction.internalName;
			int size = strlen(string);
			char* bltBuffer = (char*) (malloc( (size * sizeof(char))+10));
			sprintf(bltBuffer, "builtin (%s)", string);
			return bltBuffer;
			}
		case T_BUILTINSYNTAX:
			{
			char* string  = o->u.builtinSyntax.internalName;
			int size = strlen(string);
			char* bltBuffer = (char*) (malloc( (size * sizeof(char))+10));
			sprintf(bltBuffer, "builtin (%s)", string);
			return bltBuffer;
			}
		case T_VOID:
			return "#void";
		case T_USERDEFINEDFUNCTION:
			return "lambda";
		case T_LOCALENVIRONMENT:
			{
			char* envBuffer = (char*) malloc(64);
			sprintf(envBuffer, "local Env: (%p)", o);
			return envBuffer;
			}
		case T_GLOBALENVIRONMENT:
			{
			char* envBuffer = (char*) malloc(64);
			sprintf(envBuffer, "GLOBAL ENV (%p)", o);
			return envBuffer;
			}
			return "#env global";
		case T_FILESTREAM:
			return "#file stream";
		case T_STRINGSTREAM:
			return "#string stream";
		default:
			return "<unimpl. (objName)>";
	}
}


void
printCentered(int width, char* value){

	int sLength = strlen(value);
	int before = (width - sLength) / 2;
	int after = width - before - sLength;

	fprintf(stdout, "%*s%s%*s|", before, " ", value, after, " ");
}

void
printJStack(char *file,int line, char* func){
	
	fprintf(stdout, "\nJS Stack in "YEL"%s()"RESET" [%s:%d]\n",func, file, line);
	fprintf(stdout, "----------------------------------\n");
	for(int sp = (SP); sp >= 0; sp--){

		fprintf(stdout, "| %d : ", sp);
		
		if( sp == SP-1 || sp == AP - 2 || ((sp == 0) && (AP > 0))){
			VOIDPTRFUNC func = (VOIDPTRFUNC)(jStack[sp]);
			char* funcName = functionName((VOIDPTRFUNC)func);
			printCentered(27, funcName);	
		}
		else if( sp == (AP-1) && (AP-1) > 0){
			static char intBuffer[64];
			sprintf(intBuffer, "%lu", (INT)jStack[sp]);
			printCentered(27, intBuffer);
		}
		else if( sp == SP){
			fprintf(stdout,"                           |");
		}
		else{
			OBJ stackElement = (OBJ) jStack[sp];
			if( stackElement == NULL ){
				printCentered(27, "NULL");
			}else{
				printCentered(27, objName(stackElement));
			}
		}
		
		if( sp == SP) fprintf(stdout, " <- SP");
		if( sp == AP) fprintf(stdout, " <- AP");
		if( sp == AP - 1) fprintf(stdout, " (saved AP)");
		
		fprintf(stdout, "\n");	
	}
	fprintf(stdout, "----------------------------------\n");
}
#endif

static void
print_list_rest(FILE* outFile, OBJ o){

	OBJ theRest;
	js_print(outFile, CAR(o));	
	theRest = CDR(o);

	if(theRest == js_nil){
		fprintf(outFile, ")");
		return;
	}
	if( !ISCONS(theRest)){
		fprintf(outFile, " . ");
		js_print(outFile, theRest);
		fprintf(outFile, ")");
		return;
	}
	fprintf(outFile, " ");
	print_list_rest(outFile, theRest);
}

void
js_print(FILE* outFile,OBJ o){

	enum tag kindOf = o->u.any.tag;

#ifdef DEBUG

	if( DETAILED_TYPES->state ){
	switch(kindOf){
		case T_NIL:
			fprintf(outFile,YEL"<JS nil> "RESET);
			break;
		case T_TRUE:
			fprintf(outFile,YEL"<JS true> "RESET);
			break;
		case T_FALSE:
			fprintf(outFile,YEL"<JS false> "RESET);
			break;
		case T_INTEGER:
			fprintf(outFile,YEL"<JS integer> "RESET);
			break;
		case T_SYMBOL:
			fprintf(outFile,YEL"<JS symbol> "RESET);
			break;
		case T_STRING:
			fprintf(outFile,YEL"<JS string> "RESET);
			break;
		case T_CONS:
			fprintf(outFile,YEL"<JS cons> "RESET);
			break;
		case T_BUILTINFUNCTION:
			fprintf(outFile,YEL"<JS builtinFunction> "RESET);
			break;
		case T_BUILTINSYNTAX:
			fprintf(outFile,YEL"<JS builtinSyntax> "RESET);
			break;
		case T_VOID:
			fprintf(outFile,YEL"<JS void> "RESET);
			break;
		case T_USERDEFINEDFUNCTION:
			fprintf(outFile,YEL"<JS userDefinedFunction> "RESET);
			break;
		case T_LOCALENVIRONMENT:
			fprintf(outFile,YEL"<JS local environment> "RESET);
			break;
		case T_GLOBALENVIRONMENT:
			fprintf(outFile,YEL"<JS global environment> "RESET);
			break;	
		case T_FILESTREAM:
			fprintf(outFile,YEL"<JS file stream> "RESET);
			break;
		case T_STRINGSTREAM:
			fprintf(outFile,YEL"<JS string stream> "RESET);
			break;
		default:
			fprintf(outFile,RED"<Debug error: tag not supported>\n"RESET);
	}
	}
#endif

	switch(kindOf){
		case T_NIL:
			fprintf(outFile,"nil");
			break;
		case T_TRUE:
			fprintf(outFile,"#t");
			break;
		case T_FALSE:
			fprintf(outFile,"#f");
			break;
		case T_INTEGER:
			fprintf(outFile,"%ld" , o->u.integer.intVal);
			break;
		case T_STRING:
			fprintf(outFile,"\"%s\"", o->u.string.stringVal);
			break;
		case T_SYMBOL:
			fprintf(outFile,"%s", o->u.symbol.symbolVal);
			break;
		case T_CONS:
			fprintf(outFile, "(");
			print_list_rest(outFile, o);
			break;
		case T_BUILTINFUNCTION:
			fprintf(outFile, "<builtin procedure '%s'>", o->u.builtinFunction.internalName);
			break;
		case T_BUILTINSYNTAX:
			fprintf(outFile, "<builtin syntax %s>", o->u.builtinSyntax.internalName);
			break;
		case T_VOID:
			fprintf(outFile, "#void");
			break;
		case T_USERDEFINEDFUNCTION:
			fprintf(outFile, "(lambda ");
			js_print(outFile, o->u.userDefinedFunction.argList);
			fprintf(outFile, " ");
			print_list_rest(outFile, o->u.userDefinedFunction.bodyList);
			break;
		case T_LOCALENVIRONMENT:
			fprintf(outFile,"#env local");
			break;
		case T_GLOBALENVIRONMENT:
			fprintf(outFile,"#env global");
			break;
		case T_FILESTREAM:
			fprintf(outFile,"#file stream");
			break;
		case T_STRINGSTREAM:
			fprintf(outFile,"#string stream");
			break;
		default:
			fprintf(outFile,"<unimpl. (print)>");
			break;
	}
}
