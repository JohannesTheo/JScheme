#include <stdio.h>
#include "jscheme.h"


void
printIndent(int indentLevel){
	
	for(int i = 0; i < indentLevel; i++){
		fprintf(stdout, INDENT);
	}
}

void
printLocalEnv(int intendLevel, OBJ localEnv){

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

void
printEvalStack(){
	
	fprintf(stdout, "Eval Stack: \n");
	fprintf(stdout, "--------------\n");
	for(int i = (spIndex-1); i >= 0; i--){
		
		fprintf(stdout, "%d : ", i);
		js_print(stdout, NTH_ARG(spIndex,i) );
		fprintf(stdout, "\n");	
	}
	fprintf(stdout, "--------------\n");
}

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
		default:
			fprintf(outFile,RED"<Debug error: tag not supported>\n"RESET);
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
		default:
			fprintf(outFile,"<unimpl. (print)>");
			break;
	}
}
