#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Type.h"
#include "Symbol.h"

/*
 * TODO: finish implementing this function.
 */
struct Symbol *
newArraySym(int lvl, char *id, struct Symbol *baseTypeSym,
	 struct Symbol *indexTypeSym)
{
	struct Symbol *newArray = NULL;
	size_t len;
	if ((!baseTypeSym) || (!indexTypeSym)) {
		/* Error */
		return NULL;
	}
	if ((indexTypeSym->type != SCALAR_T) ||
	    (indexTypeSym->type != SUBRANGE_T)) {
		/* Error */
		return NULL;
	}

	newArray = calloc(1, sizeof(struct Symbol));
	if (!newArray) {
		err(1, "Failed to allocate memory for new array!");
		exit(1);
	}

	/* Have to explicitly check if id is set because we can have
	 * anonymous arrays (e.g., myArray : array[1..10] of array[1..10] of
	 * char )
	 */
	if (id) {
		strcpy(newArray->name, id);
	}
	
	return NULL;
}

/*
 * Creates a new variable struct to be added to the symbol table
 * given an identifier and an entry in the symbol table which is a type.
 */

struct Symbol *
newVariableSym(int lvl, char *id, struct Symbol* typeSym)
{
	struct Symbol *newVar = NULL;	/* new symbol to be created */
	size_t len;
	/*
	 * Before making any allocations, we assure that the given 
	 * symbol typeSym is in fact a type and that we can use it
	 * to create a new variable.
	 */
	if (!typeSym) {
		/* 
		 * ERROR: trying to create var for NULL symbol!
		 * --> probably using undefined type
		 * Call an error recording function.
		 */
		return NULL; 
	}

	if (typeSym->kind != TYPE_KIND) {
		/* ERROR: 
		 * Trying to create var using symbol other than a type.
		 * Call an error recording function.
		 */
		return NULL;	
	}

	if (!id) {
		/*Error: cannot have anonymous variable! */
		return NULL;
	}
		

	newVar = calloc(1, sizeof(struct Symbol));
	if (!newVar) {
		err(1, "Failed to allocate memory for new symbol!");
		exit(1);
	}
	
	/* Set variable name <- id */
	len = strlen(id);
	newVar->name = calloc(1, (sizeof(char))*len);
	if (!newVar->name) {
		err(1, "Failed to allocate memory for new symbol name!");
		exit(1);
	}	
	strcpy(newVar->name, id);

	newVar->kind = VAR_KIND;
	newVar->kindPtr = NULL;	/* var struct is empty for now */
	newVar->type = typeSym->type;
	newVar->typePtr = typeSym->typePtr;

	newVar->lvl = lvl;	
	return newVar;
}

/*
 * Creates a new parameter to be attached to a function. 
 * NOTE: each call to newParameter must be followed by a call to newVariable()
 *       in order to make the parameter available as a local variable in
 *       procedure/function definition body.
 * TODO: this function is UNTESTED
 */
struct Param *
newParameter(int lvl, char *id, struct Symbol *typeSym)
{
	struct Param *newParam = NULL;

	if (!typeSym) {
		/* Record error */
		return NULL;
	}
	if (typeSym->kind != TYPE_KIND) {
		/* Record error */
		return NULL;
	}

	if (!id) {
		/* Error: canot have anonymous parameters! */
		return NULL;
	}

	
	newParam = calloc(1, sizeof(struct Param));
	if (!newParam) {
		err(1, "Failed to allocate memory for new parameter!");
		exit(1);
	}

	strcpy(newParam->name, id);
	newParam->type = typeSym->kind;
	newParam->typePtr = typeSym->typePtr;	

	return newParam;
}

/*
 * Constructs an anonymous subrange symbol.  
 * TODO: split this into two functions: newSubrangeSym() and newSubrange()
*	 where each one does the obvious thing.
 */
struct Symbol *
newSubrangeSym(int lvl, struct Symbol *constSymLow, 
	        struct Symbol *constSymHigh)
{
	struct Symbol *newSubrange = NULL;
	int high, low;
	/* 
	 * We must assure that we are constructing a subrange
	 * from two ordinal types of the same type of kind const such that
	 * the value of constSymLow is less than the value of constSymHigh.
	 */
	if ((!constSymLow) || (!constSymHigh)) {
		/* Error */
		printf("1\n");
		return NULL;
	}

	if ((constSymLow->kind != CONST_KIND) ||
	    (constSymHigh->kind != CONST_KIND)) {
		/*Error: subranges indices not constants */
		printf("2\n");	
		return NULL;
	}

	if (constSymLow->type != constSymHigh->type) {
		/* Error:  Mismatched types for subrange indices */
		printf("3\n");
		return NULL;
	}

	if (!isOrdinal(constSymLow->type)) {
		/* 
		 * Error: trying to construct subrange from non ordinal
		 * types
		 */
		printf("4\n");
		return NULL;
	}

	/*
	 * Insure that values are bounded correctly (dependent on type ).
	 */
	switch (constSymLow->type) {
		case BOOLEAN_T:
		{
			low = constSymLow->typePtr.Boolean->value;
			high = constSymLow->typePtr.Boolean->value;	
			if (low >= high) {
				/* Error: lhs value not less than rhs value */
				return NULL;
			}
			break;
		}
		case CHAR_T:
		{
			low = constSymLow->typePtr.Char->value;
			high = constSymHigh->typePtr.Char->value;
			if (low >= high ) {
				/* Error: lhs value not less than rhs value */
				return NULL;
			}
			break;
		}
		case INTEGER_T:
		{
			low = constSymLow->typePtr.Integer->value;
			high = constSymHigh->typePtr.Integer->value;
			if (low >= high ) {
				/* Error: lhs value not less than rhs value */
				return NULL;
			}
			break;
		}
		default:
		{
			/* This shouldn't happen ... */
			return NULL;
		}
	}



	newSubrange = calloc(1, sizeof(struct Symbol));
	if (!newSubrange) {
		err(1, "Failed to allocate memory for new subrange symbol!");
		exit(1);
	}

	newSubrange->typePtr.Subrange = calloc(1, sizeof(struct Subrange));
	if (!newSubrange->typePtr.Subrange) {
		err(1, "Failed to allocate memory for new subrange!");
		exit(1);
	}
	
	newSubrange->kind = TYPE_KIND;
	newSubrange->kindPtr = NULL;
	newSubrange->typePtr.Subrange->low = low;
	newSubrange->typePtr.Subrange->high = high;
	newSubrange->type = SUBRANGE_T;
	newSubrange->typePtr.Subrange->baseType = constSymLow->type;
	newSubrange->name = NULL;
	newSubrange->lvl = lvl;
	/* Set base type pointer for new subrange */
	switch (newSubrange->typePtr.Subrange->baseType) {
		case BOOLEAN_T:
		{
			newSubrange->typePtr.Subrange->baseTypePtr.Boolean =
			    constSymLow->typePtr.Boolean;
			break;
		}
		case CHAR_T:
		{
			newSubrange->typePtr.Subrange->baseTypePtr.Char =
			    constSymLow->typePtr.Char;
			break;
		}
		case INTEGER_T:
		{
			newSubrange->typePtr.Subrange->baseTypePtr.Integer =
			    constSymLow->typePtr.Integer;
			break;
		}
		default:
			break;
	}
	
	return newSubrange;
}


