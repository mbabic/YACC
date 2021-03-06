/*
 * Cmput 415 - Team YACC
 * Unit tests for the Type module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testType.h"

char *
test_areSameType()
{
	Symbol *s1 = setUpTypeSymbol();
	Symbol *s2 = setUpTypeSymbol();

	mu_assert("s1 and s2 are NOT the exact same type",
	    !areSameType(s1, s2));

	s1->kindPtr.TypeKind = s2->kindPtr.TypeKind;
	mu_assert("s1 and s2 are of the exact same type",
	    areSameType(s1, s2));
	return NULL;

}

char *
test_areOpCompatible()
{
	Symbol *s1 = setUpTypeSymbol();
	Symbol *s2 = setUpTypeSymbol();

	mu_assert("two integers are operation compatible",
	    areOpCompatible(s1, s2));
	return NULL;
}

char *
test_isOrdinal()
{
	mu_assert("isOrdinal() should return false for REAL_T",
		  isOrdinal(REAL_T) == 0);
	mu_assert("isOrdinal() should return true for CHAR_T",
		  isOrdinal(CHAR_T));
	mu_assert("isOrdinal() should return true for BOOLEAN_T",
		  isOrdinal(BOOLEAN_T));
	mu_assert("isOrdinal() should return true for INTEGER_T",
		  isOrdinal(INTEGER_T));
	mu_assert("isOrdinal() should return 0 for ARRAY_T",
		  !isOrdinal(ARRAY_T));
	return NULL;
}

char *
test_isSimpleType()
{
	mu_assert("INT is a simple type", INTEGER_T);
	return NULL;

}

char *
test_all_Type()
{
	mu_run_test(test_areOpCompatible);
	mu_run_test(test_areSameType);
	mu_run_test(test_isOrdinal);
	mu_run_test(test_isSimpleType);
	return NULL;
}
