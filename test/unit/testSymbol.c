/*
 * Cmput 415 - Team YACC
 * Unit tests for the Symbol module.
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testSymbol.h"

#define INTLOW_VAL 12
#define INTHIGH_VAL 23423

int yylineno;
int colno;

/*
 * TODO: make this function accept type_t arg so it can set up types
 * of different types.
 */
struct Symbol *
setUpTypeSymbol()
{
	char id[] = "testType";
	struct Symbol *typeSym = calloc(1, sizeof(struct Symbol));
	size_t len = strlen(id);
	if (!typeSym) {
		err(1, "Failed to allocate memory for test type symbol!");
		exit(1);
	}

	typeSym->name = calloc(1, (sizeof(char))*len);
	if (!typeSym->name) {
		err(1, "Failed to allocate memory for test type symbol name!");
		exit(1);
	}
	strcpy(typeSym->name, id);
	typeSym->kind = TYPE_KIND;
	typeSym->kindPtr.TypeKind = calloc(1, sizeof(Type));
	typeSym->kindPtr.TypeKind->typePtr.Integer = calloc(1, sizeof(struct Integer));
	if (!typeSym->kindPtr.TypeKind->typePtr.Integer) {
		err(1, 
		    "Failed to allocate memory for test type symbol typeptr!");
		exit(1);
	}
	typeSym->kindPtr.TypeKind->typePtr.Integer->value = INTLOW_VAL;
	return typeSym; 	
}

char *
test_newVariableSym()
{
	struct Symbol *typeSym = setUpTypeSymbol();
	struct Symbol *newVar = NULL;
	struct Symbol *varTypeSym = NULL;
	char id[] = "testVariable";
	int lvl = 100;

	newVar = newVariableSym(lvl, id, NULL);
	mu_assert("newVariable() should return NULL if NULL typeSym passed",
		!newVar);
	
	newVar = newVariableSym(lvl, NULL, typeSym);
	mu_assert("newVariable() should return NULL if NULL id passed",
		!newVar);

	typeSym->kind = VAR_KIND;
	newVar = newVariableSym(lvl, id, typeSym);
	mu_assert("newVariable() should return NULL if passed VAR_KIND symbol",
		!newVar);

	typeSym->kind = TYPE_KIND;

	newVar = newVariableSym(lvl, id, typeSym);
	varTypeSym = newVar->kindPtr.VarKind->typeSym;
	mu_assert("newVariable() should return symbol of kind VAR_KIND",
		newVar->kind == VAR_KIND);
	mu_assert("newVariable() should return symbol w/name \"testVariable\"",
		strcmp(newVar->name, id) == 0);
	mu_assert("newVariable() should be a variable with type defined by"
		  "the symbol expected",
		(varTypeSym == typeSym));
	return NULL;
}
/*
char *
test_newSubrangeSym() 
{
	struct Symbol *subrangeSym = NULL;
	struct Symbol *testSymLow = setUpTypeSymbol();
	struct Symbol *testSymHigh = setUpTypeSymbol();
	int lvl = 102;
	testSymHigh->typePtr.Integer->value = INTHIGH_VAL;
	testSymLow->kind = CONST_KIND;
	testSymHigh->kind = CONST_KIND;	
	
	subrangeSym = newSubrangeSym(lvl, testSymLow, NULL);
	mu_assert("newSubrangeSym() should return NULL if a symbol arg is NULL",
		  !subrangeSym);


	subrangeSym = newSubrangeSym(lvl, testSymHigh, testSymLow);
	mu_assert("newSubRangeSym() should return NULL if low >= high",
		  !subrangeSym);

	subrangeSym = newSubrangeSym(lvl, testSymLow, testSymHigh);
	if (!subrangeSym) printf("da fuck!?!?!?!\n");
	mu_assert("newSubRangeSym() should return expected subrange",
		  (subrangeSym->typePtr.Subrange->baseType == INTEGER_T) &&
		  (subrangeSym->typePtr.Subrange->baseTypePtr.Integer ==
		   testSymLow->typePtr.Integer) &&
		  (subrangeSym->typePtr.Subrange->low ==
		   testSymLow->typePtr.Integer->value) &&
		  (subrangeSym->typePtr.Subrange->high ==
		   testSymHigh->typePtr.Integer->value) && 
		  (subrangeSym->lvl == lvl) &&
		  (subrangeSym->kind == TYPE_KIND) &&
		  (!subrangeSym->name));

	testSymLow->type = ARRAY_T;
	subrangeSym = newSubrangeSym(lvl, testSymLow, testSymHigh);
	mu_assert("newSubrangeSym() should return null when symbol types \
		   mistmatch.", !subrangeSym);
	return NULL;
}
*/
char *
test_all_Symbol()
{
//	mu_run_test(test_newSubrangeSym);
	mu_run_test(test_newVariableSym);
	return NULL;
}