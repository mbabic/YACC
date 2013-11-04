
#include <stdio.h>
#include <stdlib.h>

#ifndef TYPE_H
#define TYPE_H

#include "Kind.h"

/*
 * Changed heavily to mirror the design suggested in Piotr's notes (see
 * CheckingAttributes.pdf of the site for details)
 */

/*
 * Notes/thoughts:
 *	-PARAM =?= Vvar with flag that tells us if it was
 *	 passed by reference or not???  Do we actually need to know the params
 *	 for a function ? ..... yes: for semantic checking at invocation
 *
 *
 *	-Does this class pass divine judgment upon the souls of the living
 * 	 and the dead? (smellin like a god class brah)
 */

/*
 * The types of a values which can be associated with various kinds.
 */
typedef enum {
	ARRAY_T,
	BOOLEAN_T,
	CHAR_T,
	INTEGER_T,
	FUNCTION_T,
	PROCEDURE_T,
	REAL_T,
	RECORD_T,	
	SCALAR_T,
	STRING_T,
	SUBRANGE_T,
} type_t;

typedef union type_union {
	struct Array * Array;
	struct Boolean * Boolean;
	struct Char * Char;
	struct Integer * Integer;
	struct Function *Function;
	struct Procedure *Procedure;
	struct Real * Real;
	struct Record * Record;
	struct Scalar * Scalar;
	struct String * String;
	struct Subrange * Subrange;
} Type;


/*
 * The kind values an entry in the symbol table can take.
 */
typedef enum {
	CONST_KIND,	/* constant value */
	FUNC_KIND,	/* function declaration */
	PROC_KIND,	/* procedure declaration */
	TYPE_KIND,	/* type declaration */
	VAR_KIND	/* variable declaration */
} kind_t;



struct Param {
	char *name;			/* TODO: name unnecessary, work it out
					 * of code */
	type_t type;	/* probably unnecessary now that we're pting to sym */
	struct Symbol *typeSymPtr;	/* note: PAL specifications requires
					 * that all procs/funcs have params
					 * have already defined types, so no
					 * anon types as param types */
};

/* Param Array defined here to avoid incestuous circular .h includes :( */
struct ParamArray {
	struct Param **data;
	unsigned int len;
	unsigned int nElements;
};


struct Array {
	struct Symbol *baseTypeSym;	/* pointer to struct of base type */
	struct Symbol *indexTypeSym;	/* pointer to struct of index type */
};

struct Boolean {
	int value;		/* value only needed when being pointed to
				 * by an object of kind CONST_KIND */
};

struct Char {
	char value;		/* value only needed when being pointed to
				 * by an object of kind CONST_KIND */
};

/*
 * TODO: need to check if function actually returns something (i.e., does it
 *	 reference its own id as l-val in an assignment operation?) ; we won't
 *	 have time to implement checking if there is a execution path
 * 	 which results in no return type being set (and thus setting warning/
 *	 error)
 */
struct Function {
	struct ParamArray *params;
	struct Symbol  *returnTypeSym;

};

struct Integer {
	int value;		/* value only needed when being pointed to
				 * by an object of kind CONST_KIND */
};

struct Procedure {
	struct ParamArray *params;
};


struct Real {
	double value;		/* value only needed when being pointed to
				 * by an object of kind CONST_KIND */
};


struct String {
	char * str;
	unsigned int strlen;
};
struct Subrange {
	int high;
	int low;
	struct Symbol *baseTypeSym; /* pointer to struct of type of subrange indices */
};

struct Scalar {
	struct Symbol *consts;	/* linked list of const syms in scalar 
				 * enumeration */
};



struct Record {
	/* each record implemented as its own symbol table */
};

/*
 * To be used when were are making an anonymous constant type (which will
 * be used to make a symbol).  We could not simply reuse the Type union
 * that is a union of pointers and would have required that we allocate
 * memory inside actions in the grammar file which I felt was a bad idea.
 */
typedef union anonymous_constant_value {
	struct Boolean Boolean;
	struct Char Char;
	struct Integer Integer;
	struct Real Real;
	struct String String;
} AnonConstVal;

typedef union kind_union {
	struct ConstantKind * ConstKind;
	struct FunctionKind * FuncKind;
	struct ProcedureKind * ProcKind;
	struct TypeKind * TypeKind;
	struct VariableKind * VarKind;
} Kind;

struct ConstantKind {
	struct Symbol *typeSym;
	AnonConstVal value;		
};

struct FunctionKind {
	struct Symbol *typeSym;
	struct ParamArray *params;
};

struct ProcedureKind {
	struct ParamArray *params;
};

struct TypeKind {
	Type typePtr;
	type_t type;
};

struct VariableKind {
	struct Symbol *typeSym;
};


/*
 * The object stored at a hash element in the symbol table.
 * Here we define the name of the entry, what kind it is (e.g., VAR, CONST,
 * PARAM, ... ), and a pointer to the particular type of kind it is. 
 * We also have type and typePtr here as well because EVERY entry will have
 * these fields.
 */
struct Symbol {
	char *name;	/* the name associated with the entry */
	kind_t kind;	/* the kind associated with this entry */
	Kind kindPtr;	/* kind specific description of symbol */
	int lvl;	/* the lexical level at which the entry is defined */	
};


/* Function declarations */
int isOrdinal(type_t);
struct Array *newArray(struct Symbol *, struct Symbol *);
struct Subrange *newSubrange(struct Symbol*, struct Symbol *);
Type newAnonConstType(AnonConstVal, type_t);
type_t getType(struct Symbol *);
void setTypePtr(Type *, Type, type_t);
void typeMemoryFailure();
#endif