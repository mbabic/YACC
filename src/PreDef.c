#include "PreDef.h"
#include "Type.h"
#include "Kind.h"
#include "Symbol.h"
#include "Hash.h"

Symbol *getPreDefBool(struct preDefTypeSymbols *preDefTypeSymbols) {
	return preDefTypeSymbols->boolean;
}


Symbol *getPreDefChar(struct preDefTypeSymbols *preDefTypeSymbols) {
	return preDefTypeSymbols->chars;
}


Symbol *getPreDefInt(struct preDefTypeSymbols *preDefTypeSymbols) {
	return preDefTypeSymbols->integer;
}


Symbol *getPreDefReal(struct preDefTypeSymbols *preDefTypeSymbols) {
	return preDefTypeSymbols->real;
}


struct Symbol *createPreDefType(char *name, type_t type) {
	struct Symbol *symbol = malloc(sizeof(struct Symbol));

	setSymbolName(symbol, name);
	symbol->kind = TYPE_KIND;

	allocateKindPtr(symbol);
	symbol->kindPtr.TypeKind->type = type;

	if ( strcmp(name, "boolean") == 0 ) {
		getTypePtr(symbol)->Boolean = malloc(sizeof(struct Boolean));
	}
	else if ( strcmp(name, "char") == 0 ) {
		getTypePtr(symbol)->Char = malloc(sizeof(struct Char));
	}
	else if ( strcmp(name, "integer") == 0 ) {
		getTypePtr(symbol)->Integer = malloc(sizeof(struct Integer));
	}
	else if ( strcmp(name, "real") == 0 ) {
		getTypePtr(symbol)->Real = malloc(sizeof(struct Real));
	}
	else {
		err(2, "Could not determine type asked in pre-def.");
		exit(EXIT_FAILURE);
	}

	symbol->lvl = getCurrentLexLevel(symbolTable);
	symbol->typeOriginator = 1;
	symbol->next = NULL;

	return symbol;
}


struct preDefTypeSymbols *initializePreDefTypes() {
	struct preDefTypeSymbols *preDefs = malloc(sizeof(struct preDefTypeSymbols));	
	preDefs->boolean = createPreDefType(BOOLEAN_KEY, TYPE_KIND);
	preDefs->chars = createPreDefType(CHAR_KEY, TYPE_KIND);
	preDefs->integer = createPreDefType(INTEGER_KEY, TYPE_KIND);
	preDefs->real = createPreDefType(REAL_KEY, TYPE_KIND);

	createHashElement(symbolTable, BOOLEAN_KEY, preDefs->boolean);
	createHashElement(symbolTable, CHAR_KEY, preDefs->chars);
	createHashElement(symbolTable, INTEGER_KEY, preDefs->integer);
	createHashElement(symbolTable, REAL_KEY, preDefs->real);

	return preDefs;
}


struct Symbol *createPreDefProc(char *name) {
	struct Symbol *symbol;

	symbol = calloc(1, sizeof(struct Symbol));
	if (symbol == NULL) {
		err(1, "Failed to allocate memory for symbol name!");
		exit(EXIT_FAILURE);
	}

	setSymbolName(symbol, name);
	symbol->kind = PROC_KIND;

	allocateKindPtr(symbol);
	symbol->kindPtr.ProcKind->params = NULL;

	symbol->lvl = getCurrentLexLevel(symbolTable);
	symbol->typeOriginator = 0;
	symbol->next = NULL;

	return symbol;
}


int initializePreDefProc() {
	struct Symbol *symbol;

	symbol = createPreDefProc(WRITE);
	createHashElement(symbolTable, WRITE, symbol);

	symbol = createPreDefProc(WRITELN);
	createHashElement(symbolTable, WRITELN, symbol);

	symbol = createPreDefProc(READ);
	createHashElement(symbolTable, READ, symbol);

	symbol = createPreDefProc(READLN);
	createHashElement(symbolTable, READLN, symbol);

	return 0;
}
