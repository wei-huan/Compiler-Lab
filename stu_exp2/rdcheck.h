#ifndef __RDCHECK_H
#define __RDCHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include "man_lex.h"

//#define DEBUG_MODE	1
extern FILE* ana_file;
extern int nextToken;

bool CompUnit(void);
bool Decl(void);
bool ConstDecl(void);
bool ConstDef(void);
bool VarDecl(void);
bool VarDefSameLeftFactor(void);
bool VarDef(void);
bool FuncDef(void);
bool BType(void);
bool ConstInitVal(void);
bool ConstExp(void);
bool InitVal(void);
bool FuncType(void);
bool FuncFParams(void);
bool FuncFParam(void);
bool Block(void);
bool BlockItem(void);
bool Stmt(void);
bool Exp(void);
bool Cond(void);
bool LVal(void);
bool PrimaryExp(void);
bool Number(void);
bool UnaryExp(void);
bool UnaryOp(void);
bool FuncRParams(void);
bool MulExp(void);
bool AddExp(void);
bool RelExp(void);
bool EqExp(void);
bool LAndExp(void);
bool LOrExp(void);
bool MulExp_Extend(void);
bool AddExp_Extend(void);
bool RelExp_Extend(void);
bool EqExp_Extend(void);
bool LAndExp_Extend(void);
bool LOrExp_Extend(void);

#endif /*__RDCHECK_H*/
