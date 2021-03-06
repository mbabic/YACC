#ifndef INIT_H
#define INIT_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Error.h"
#include "ErrorLL.h"
#include "Globals.h"
#include "Hash.h"
#include "PreDef.h"
#include "EmitUtils.h"
#include "EmitToFile.h"
#include "StmtLL.h"

/* Macros and const string variable declarations. */


/* Global variables. */
extern struct preDefTypeSymbols *preDefTypeSymbols;
extern struct hash *symbolTable;
extern unsigned long long whileLoopDepth;

/* Non-C99 compliant function prototypes. */


/* Function declarations. */
int initialize();
int deInitialize();

#endif
