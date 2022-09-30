#ifndef __RDPARSER_H
#define __RDPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include "man_lex.h"

#define DEBUG_MODE	1
typedef struct _ast ast;
typedef struct _ast *past;
struct _ast{
	int ivalue;
	char ident[100];
	char* dataType;
	char* nodeType;
	past left;
	past right;
};

extern FILE* ana_file;
extern int nextToken;

past CompUnit(void);
past Decl(void);
past ConstDecl(void);
past ConstDef(void);
past VarDecl(void);
past VarDefSameLeftFactor(void);
past VarDef(void);
past FuncDef(void);
past BType(void);
past ConstInitVal(void);
past ConstExp(void);
past InitVal(void);
past FuncType(void);
past FuncFParams(void);
past FuncFParam(void);
past Block(void);
past BlockItem(void);
past Stmt(void);
past Exp(void);
past Cond(void);
past LVal(void);
past PrimaryExp(void);
past Number(void);
past UnaryExp(void);
past UnaryOp(void);
past FuncRParams(void);
past MulExp(void);
past AddExp(void);
past RelExp(void);
past EqExp(void);
past LAndExp(void);
past LOrExp(void);
past MulExp_Extend(void);
past AddExp_Extend(void);
past RelExp_Extend(void);
past EqExp_Extend(void);
past LAndExp_Extend(void);
past LOrExp_Extend(void);

#endif /*__RDPARSER_H*/
