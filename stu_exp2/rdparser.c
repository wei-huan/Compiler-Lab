#include "rdparser.h"

static int tok = 0;
void advance(){
	tok = lex_ana();
}

int save_recall_pos(){
	int recall_pos;
	
	if(last_get == 0)
		recall_pos = ftell(ana_file) - 2;
	else
		recall_pos = ftell(ana_file) - 1;

	if(recall_pos == -2)
		recall_pos = 0;

#ifdef DEBUG_MODE
	printf("recall_pos: %d\n", recall_pos);
#endif /*DEBUG_MODE*/

	return recall_pos;
}

void recall(int last_pos){
	int idx2 = ftell(ana_file);
	int recall_len = idx2 - last_pos;

	fseek(ana_file, (-1 * recall_len), SEEK_CUR);
	first_get = 1;

#ifdef DEBUG_MODE
	printf("回溯新位置: %ld\n", ftell(ana_file));
#endif /*DEBUG_MODE*/

	getChar();
}

past newAstNode(){
	past node = malloc(sizeof(ast));
	if(node == NULL){
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));
	return node;
}

past newType(char* typename){
	past type = newAstNode();
	type->nodeType = "Type";
	type->dataType = typename;
	return type;
}

past newNum(int value){
	past num = newAstNode();
	num->nodeType = "IntValue";
	num->ivalue = value;
	return num;
}

// past newVariable_List(past variable, past variable_list){
// 	past newvariable_list = newAstNode();
// 	newvariable_list
// }

past newVar(char* name, int dimension, past dimension_list){
	past var = newAstNode();
	strcpy(var->ident, name);
	var->nodeType = "Varible";
	var->ivalue = dimension;
	var->left = dimension_list;
	return var;
}

past newDimension_List(past dimension, past dimension_list){
	past newdimension_list = newAstNode();
	newdimension_list->nodeType = "Dimension_List";
	newdimension_list->left = dimension;
	newdimension_list->right = dimension_list;
	return newdimension_list;
}

// past newDimension(int length){
// 	past dimension = newAstNode();
// 	dimension->nodeType = "Dimension";
// 	dimension->ivalue = length;
// 	return dimension;
// }

past newOpcode(char* op, past left, past right){
	past opcode = newAstNode();
	opcode->nodeType = op;
	opcode->left = left;
	opcode->right = right;
	return opcode;
}

past newFuncDef_or_Decl_list(past funcdef_or_decl, past funcdef_or_decl_list){
	past f_or_d_list = newAstNode();
	f_or_d_list->nodeType = "FuncDef_or_Decl_list";
	f_or_d_list->left = funcdef_or_decl;
	f_or_d_list->right = funcdef_or_decl_list;
	return f_or_d_list;
}

past newFuncDef(past funcfparams_list, past block, char* ident, char* rettype){
	past funcdef = newAstNode();
	strcpy(funcdef->ident, ident);
	funcdef->nodeType = "FuncDef";
	funcdef->dataType = rettype;
	funcdef->left = funcfparams_list;
	funcdef->right = block;
	return funcdef;
}

past newFuncFParam(char* type, char* ident, int dimension, past dimension_list){
	past funcfparam = newAstNode();
	funcfparam->nodeType = type;
	funcfparam->ivalue = dimension;
	funcfparam->left = dimension_list;
	strcpy(funcfparam->ident, ident);
	return funcfparam;
}

past newFuncFParams(past funcparam, past funcparams){
	past newfuncparams = newAstNode();
	newfuncparams->left = funcparam;
	newfuncparams->right = funcparams;
	newfuncparams->nodeType = "FuncParams";
	return newfuncparams;
}

past newBlock_list(past decl_or_stmt, past block_list){
	past newblock_list = newAstNode();
	newblock_list->nodeType = "Block_List";
	newblock_list->left = decl_or_stmt;
	newblock_list->right = block_list;
	return newblock_list;
}

past newBlock(past block_list){
	past block = newAstNode();
	block->nodeType = "Block";
	block->right = block_list;
	return block;
}

past newInitVal(past exp_or_initval, past initvals){
	past newinitval = newAstNode();
	newinitval->left = exp_or_initval;
	newinitval->right = initvals;

	if(initvals == NULL)
		newinitval->nodeType = "InitVal";
	else
		newinitval->nodeType = "InitVals";

	return newinitval;
}

past newConstDef_List(past constdef, past constdef_list){
	past newconstdef_list = newAstNode();
	newconstdef_list->nodeType = "ConstDefList";
	newconstdef_list->left = constdef;
	newconstdef_list->right = constdef_list;
	return constdef_list;
}

past newConstDecl(past type, past constdef_list){
	past constdecl = newAstNode();
	constdecl->nodeType = "ConstDecl";
	constdecl->left = type;
	constdecl->right = constdef_list;
	return constdecl;
}

past newConstInitVal_List(past constinitval, past constinitval_list){
	past newconstinitval_list = newAstNode();
	newconstinitval_list->nodeType = "ConstInitVal_List";
	newconstinitval_list->left = constinitval;
	newconstinitval_list->right = constinitval_list;
	return newconstinitval_list;
}

past newVarDecl(past type, past vardef_list){
	past vardecl = newAstNode();
	vardecl->nodeType = "VarDecl";
	vardecl->left = type;
	vardecl->right = vardef_list;
	return vardecl;
}

past newVarDef_List(past vardef, past vardef_list){
	past newvardef_list = newAstNode();
	newvardef_list->nodeType = "VarDef_List";
	newvardef_list->left = vardef;
	newvardef_list->right = vardef_list;
	return newvardef_list;
}

past newFunc(char* funcname, past funcrparams_list){
	past func = newAstNode();
	strcpy(func->ident, funcname);
	func->nodeType = "Func";
	func->left = funcrparams_list;
	return func;
}

past newFuncRParams_List(past funcrparam, past funcrparams_list){
	past newfuncrparams_list = newAstNode();
	newfuncrparams_list->nodeType = "FuncRParams_List";
	newfuncrparams_list->left = funcrparam;
	newfuncrparams_list->right = funcrparams_list;
	return newfuncrparams_list;
}

past newWhileStmt(past cond, past stmt){
	past whilestmt = newAstNode();
	whilestmt->nodeType = "WhileStmt";
	whilestmt->left = cond;
	whilestmt->right = stmt;
	return whilestmt;
}

past newContinueStmt(){
	past continuestmt = newAstNode();
	continuestmt->nodeType = "ContinueStmt";
	return continuestmt;
}

past newReturnStmt(past exp){
	past returnstmt = newAstNode();
	returnstmt->nodeType = "ReturnStmt";
	returnstmt->left = exp;
	return returnstmt;
}

past newBreakStmt(){
	past breakstmt = newAstNode();
	breakstmt->nodeType = "BreakStmt";
	return breakstmt;
}

past newSelectStmt(past ifstmt, past elsestmt){
	past selectstmt = newAstNode();
	selectstmt->nodeType = "SelectStmt";
	selectstmt->left = ifstmt;
	selectstmt->right = elsestmt;
	return selectstmt;
}

past newIfStmt(past cond, past stmt){
	past ifstmt = newAstNode();
	ifstmt->nodeType = "IfStmt";
	ifstmt->left = cond;
	ifstmt->right = stmt;
	return ifstmt;
}

past newElseStmt(){
	past elsestmt = newAstNode();
	elsestmt->nodeType = "ElseStmt";
	return elsestmt;
}

past newNullStmt(){
	past nullstmt = newAstNode();
	nullstmt->nodeType = "NullStmt";
	return nullstmt;
}

past CompUnit(){
	int recall_pos;
	past funcdef_or_decl_list = NULL;

	while(1){
		recall_pos = save_recall_pos();
		advance();

		if(tok == tok_EOF){
#ifdef DEBUG_MODE
			printf("CompUnit out1\n");
#endif /*DEBUG_MODE*/
			return funcdef_or_decl_list;
		}
		else {
			recall(recall_pos);
			past funcdef = FuncDef();

			if(funcdef == NULL){
				recall(recall_pos);
				past decl = Decl();

				if(decl == NULL){
#ifdef DEBUG_MODE
					printf("CompUnit out2\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
				else{
					funcdef_or_decl_list = newFuncDef_or_Decl_list(decl, funcdef_or_decl_list);
#ifdef DEBUG_MODE
					printf("成功, 完成一轮CompUnit分析\n");
#endif /*DEBUG_MODE*/
				}
			}
			else{
				past funcdef_or_decl_list = newFuncDef_or_Decl_list(funcdef, funcdef_or_decl_list);
#ifdef DEBUG_MODE
				printf("成功, 完成一轮CompUnit分析\n");
#endif /*DEBUG_MODE*/
			}
		}
	}
}

past Decl(){
#ifdef DEBUG_MODE
	printf("Decl in\n");
#endif /*DEBUG_MODE*/
	int recall_pos = save_recall_pos(recall_pos);

	past constdecl = ConstDecl();
	if(constdecl != NULL){
		
#ifdef DEBUG_MODE
		printf("Decl out1 true\n");
#endif /*DEBUG_MODE*/
		return constdecl;
	}
	else{
		recall(recall_pos);

		past vardecl = VarDecl();
		if(vardecl != NULL){

#ifdef DEBUG_MODE
			printf("Decl out2 true\n");
#endif /*DEBUG_MODE*/
			return vardecl;
		}
		else{
#ifdef DEBUG_MODE
			printf("Decl out3 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
}

past ConstDecl(){
	past constdef_list = NULL;
#ifdef DEBUG_MODE
	printf("ConstDecl in\n");
#endif /*DEBUG_MODE*/

	advance();
	if(tok == tok_CONST){
		
		past btype = BType();
		if(btype != NULL){

			past constdef = ConstDef();
			if(constdef != NULL){

				constdef_list = newConstDef_List(constdef, constdef_list);

				advance();
				if(tok == tok_COMMA){
					while (1){

						past constdef = ConstDef();
						if(constdef != NULL){

							constdef_list = newConstDef_List(constdef, constdef_list);
							advance();

							if(tok != tok_COMMA)
								break;
						}
						else{
#ifdef DEBUG_MODE
							printf("ConstDecl out1 false\n");
#endif /*DEBUG_MODE*/
							return NULL;
						}
					}

					if(tok == tok_SEMICOLON){
						past constdecl = newConstDecl(btype, constdef_list);
#ifdef DEBUG_MODE
						printf("ConstDecl out2 true\n");
#endif /*DEBUG_MODE*/
						return constdecl;
					}
					else{
#ifdef DEBUG_MODE
						printf("ConstDecl out3 false\n");
#endif /*DEBUG_MODE*/
						return NULL;
					}
					
				}
				else if(tok == tok_SEMICOLON){
					past constdecl = newConstDecl(btype, constdef);
#ifdef DEBUG_MODE
					printf("ConstDecl out4 true\n");
#endif /*DEBUG_MODE*/
					return constdecl;
				}
				else{
#ifdef DEBUG_MODE
					printf("ConstDecl out5 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else{
#ifdef DEBUG_MODE
				printf("ConstDecl out6 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}

		}
		else{
#ifdef DEBUG_MODE
			printf("ConstDecl out7 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("ConstDecl out8 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past ConstDef(){
#ifdef DEBUG_MODE
	printf("ConstDef in\n");
#endif /*DEBUG_MODE*/
	advance();

	if(tok == tok_ID){
		char id_str[100];
		strcpy(id_str, lex_str);

		advance();

		if(tok == tok_ASSIGN){

			past const_variable = newVar(id_str, 0, NULL);
			past constinitval = ConstInitVal();
			if(constinitval != NULL){

				past constdef_assign = newOpcode("=", const_variable, constinitval);
#ifdef DEBUG_MODE
				printf("ConstDef out1 true\n");
#endif /*DEBUG_MODE*/
				return constdef_assign;
			}
			else{
#ifdef DEBUG_MODE
				printf("ConstDef out2 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
		else if(tok == tok_LSQUARE){

			past constexp = ConstExp();
			if(constexp != NULL){
				advance();

				if(tok == tok_RSQUARE){
					past dimension_list = newDimension_List(constexp, NULL);
					past const_variable = newVar(id_str, 1, dimension_list);
					advance();

					if(tok == tok_ASSIGN){

						past constinitval = ConstInitVal();

						if(constinitval != NULL){
							past constdef_assign = newOpcode("=", const_variable, constdef_assign);
#ifdef DEBUG_MODE
							printf("ConstDef out3 true\n");
#endif /*DEBUG_MODE*/
							return constdef_assign;
						}
						else{
#ifdef DEBUG_MODE
							printf("ConstDef out4 false\n");
#endif /*DEBUG_MODE*/
							return NULL;
						}
					}
					else{
#ifdef DEBUG_MODE
						printf("ConstDef out5 false\n");
#endif /*DEBUG_MODE*/
						return NULL;
					}
				}
				else{
#ifdef DEBUG_MODE
					printf("ConstDef out6 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else{
#ifdef DEBUG_MODE
				printf("ConstDef out7 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("ConstDef out8 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("ConstDef out9 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past BType(){
#ifdef DEBUG_MODE
	printf("BType in\n");
#endif /*DEBUG_MODE*/
	advance();

	if(tok == tok_INT){
#ifdef DEBUG_MODE
		printf("BType out1 true\n");
#endif /*DEBUG_MODE*/
		past type = newType(lex_str);
		return type;
	}
	else{
#ifdef DEBUG_MODE
		printf("BType out2 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past VarDecl(){
#ifdef DEBUG_MODE
	printf("VarDecl in\n");
#endif /*DEBUG_MODE*/
	past vardef_list = NULL;
	int last_pos = save_recall_pos();
	
	past btype = BType();
	if(btype != NULL){

		past vardef = VarDef();
		if(vardef != NULL){

			advance();
			
			if(tok == tok_SEMICOLON){
				past vardecl = newVarDecl(btype, vardef);
#ifdef DEBUG_MODE
				printf("VarDecl out1 true\n");
#endif /*DEBUG_MODE*/
				return vardecl;
			}
			else{
				while(1){
					if(tok == tok_COMMA){
						vardef_list = newVarDef_List(vardef, vardef_list);

						vardef = VarDef();
						if(vardef != NULL)
							advance();
						else{
#ifdef DEBUG_MODE
							printf("VarDecl out2 false\n");
#endif /*DEBUG_MODE*/
							return NULL;
						}
					}
					else
						break;
				}

				if(tok == tok_SEMICOLON){
					past vardecl = newVarDecl(btype, vardef);
#ifdef DEBUG_MODE
					printf("VarDecl out4 true\n");
#endif /*DEBUG_MODE*/
					return vardecl;
				}
				else{
#ifdef DEBUG_MODE
					printf("VarDecl out5 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
				
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("VarDecl out6 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("VarDecl out7 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past VarDef(){
	int dimension = 0;
	past dimension_list = NULL;
#ifdef DEBUG_MODE
	printf("VarDef in\n");
#endif /*DEBUG_MODE*/

	if(tok == tok_ID){
		char id_str[100];
		strcpy(id_str, lex_str);

		int recall_pos = save_recall_pos();
		advance();

		if(tok == tok_ASSIGN){

			past var = newVar(id_str, 0, NULL);
			past initval = InitVal();

			if(initval != NULL){

				past opcode = newOpcode("=", var, initval);
#ifdef DEBUG_MODE
				printf("VarDef out1 true\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
			else{
#ifdef DEBUG_MODE
				printf("VarDef out2 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
		else if(tok == tok_LSQUARE){
			while(1){
				if(tok == tok_LSQUARE){

					past constexp = ConstExp();
					if(constexp != NULL){
						
						advance();

						if(tok == tok_RSQUARE){
							dimension++;
							dimension_list = newDimension_List(constexp, dimension_list);
							recall_pos = save_recall_pos();
							advance();
						}
						else{
#ifdef DEBUG_MODE
							printf("VarDef out3 false\n");
#endif /*DEBUG_MODE*/
							return NULL;
						}
					}
					else{
#ifdef DEBUG_MODE
						printf("VarDef out4 false\n");
#endif /*DEBUG_MODE*/
						return NULL;
					}
				}
				else
					break;
			}

			past var = newVar(id_str, dimension, dimension_list);

			if(tok == tok_ASSIGN){

				past initval = InitVal();
				if(initval != NULL){

					past opcode = newOpcode("=", var, initval);
#ifdef DEBUG_MODE
					printf("VarDef out5 true\n");
#endif /*DEBUG_MODE*/
					return opcode;
				}
				else{
#ifdef DEBUG_MODE
					printf("VarDef out6 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else{
				recall(recall_pos);
#ifdef DEBUG_MODE
				printf("VarDef out7 true\n");
#endif /*DEBUG_MODE*/
				return var;
			}
		}
		else{
			recall(recall_pos);
			past var = newVar(id_str, 0, NULL);
#ifdef DEBUG_MODE
			printf("VarDef out8 true\n");
#endif /*DEBUG_MODE*/
			return var;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("VarDef out9 false\n");
#endif /*DEBUG_MODE*/
		return NULL;		
	}
}


past FuncDef(){
#ifdef DEBUG_MODE
	printf("FuncDef in\n");
#endif /*DEBUG_MODE*/
	int last_pos;

	past type = FuncType();
	if(type != NULL){
		advance();

		if(tok == tok_ID){
			char id_str[100];
			strcpy((char *)id_str, lex_str);

			advance();
			if(tok == tok_LPAR){

				last_pos = save_recall_pos(); 
				advance();

				if(tok == tok_RPAR){

					past block = Block();
					if(block != NULL){

						past funcdef = newFuncDef(NULL, block, id_str, type->dataType);
#ifdef DEBUG_MODE
						printf("FuncDef out1 true\n");
#endif /*DEBUG_MODE*/
						return funcdef;
					}
					else{
#ifdef DEBUG_MODE
						printf("FuncDef out2 false\n");
#endif /*DEBUG_MODE*/
						return NULL;
					}
				}
				else{
					recall(last_pos);

					past funcfarams = FuncFParams();

					if(funcfarams != NULL){
						advance();

						if(tok == tok_RPAR){

							past block = Block();
							if(block != NULL){
								
								past funcdef = newFuncDef(funcfarams, block, id_str, type->dataType);
#ifdef DEBUG_MODE
								printf("FuncDef out3 true\n");
#endif /*DEBUG_MODE*/
								return funcdef;
							}
							else{
#ifdef DEBUG_MODE
								printf("FuncDef out4 false\n");
#endif /*DEBUG_MODE*/
								return NULL;
							}
						}
						else{
#ifdef DEBUG_MODE
							printf("FuncDef out5 false\n");
#endif /*DEBUG_MODE*/
							return NULL;
						}
					}
					else{
#ifdef DEBUG_MODE
						printf("FuncDef out6 false\n");
#endif /*DEBUG_MODE*/
						return NULL;
					}
				}
				
			}
			else{
#ifdef DEBUG_MODE
				printf("FuncDef out7 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("FuncDef out8 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("FuncDef out9 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past ConstInitVal(){
#ifdef DEBUG_MODE
	printf("ConstInitVal in\n");
#endif /*DEBUG_MODE*/
	past constinitval_list = NULL;
	int recall_pos = save_recall_pos();
	advance();

	if(tok == tok_LBRACKET){
		recall_pos = save_recall_pos();
		advance();

		if(tok == tok_RBRACKET){
#ifdef DEBUG_MODE
			printf("ConstInitVal out1 true\n");
#endif /*DEBUG_MODE*/
			return NULL;			
		}

		recall(recall_pos);

		while(1){
			past constinitval = ConstInitVal(); 
			if(constinitval != NULL){
				advance();

				if(tok == tok_COMMA){
					constinitval_list = newConstInitVal_List(constinitval, constinitval_list);
					continue;
				}
				else if(tok == tok_RBRACKET){
#ifdef DEBUG_MODE
					printf("ConstInitVal out1 true\n");
#endif /*DEBUG_MODE*/
					return constinitval_list;
				}
				else{
#ifdef DEBUG_MODE
					printf("ConstInitVal out2 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else{
#ifdef DEBUG_MODE
				printf("ConstInitVal out3 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
	}
	else{
		recall(recall_pos);

		past constexp = ConstExp();
		if(constexp != NULL){
#ifdef DEBUG_MODE
			printf("ConstInitVal out4 true\n");
#endif /*DEBUG_MODE*/
			return constexp;
		}
		else{
#ifdef DEBUG_MODE
			printf("ConstInitVal out5 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
}

past FuncType(){
#ifdef DEBUG_MODE
	printf("FuncType in\n");
#endif /*DEBUG_MODE*/
	advance();

	if(tok == tok_VOID || tok == tok_INT){
		past type = newType(lex_str);
#ifdef DEBUG_MODE
		printf("FuncType out1 true\n");
#endif /*DEBUG_MODE*/
		return type;
	}
	else{
#ifdef DEBUG_MODE
		printf("FuncType out2 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past FuncFParams(){
#ifdef DEBUG_MODE
	printf("FuncFParams in\n");
#endif /*DEBUG_MODE*/
	int recall_pos;

	past funcfparams = NULL;

	while(1){
		past funcfparam = FuncFParam();

		if(funcfparam != NULL){
			past funcfparams = newFuncFParams(funcfparam, funcfparams);

			recall_pos = save_recall_pos();
			advance();

			if(tok != tok_COMMA){
				recall(recall_pos);

#ifdef DEBUG_MODE
				printf("FuncFParams out1 true\n");
#endif /*DEBUG_MODE*/
				return funcfparams;
			}
		}
		else{

#ifdef DEBUG_MODE
			printf("FuncFParams out2 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
}

past FuncFParam(){
#ifdef DEBUG_MODE
	printf("FuncFParam in\n");
#endif /*DEBUG_MODE*/
	int dimension = 0;
	past dimension_list = NULL;
	int recall_pos;

	if(BType() != NULL){
		char type_str[100];
		strcpy(type_str, lex_str);

		advance();

		if(tok == tok_ID){
			char id_str[100];
			strcpy(id_str, lex_str);

			recall_pos = save_recall_pos();
			advance();

			if(tok == tok_LSQUARE){
				advance();

				if(tok == tok_RSQUARE){

					dimension++;
					dimension_list = newDimension_List(NULL, NULL);

					while(1){
						recall_pos = save_recall_pos();
						advance();

						if(tok == tok_LSQUARE){
							past exp = Exp();

							if(exp != NULL){
								advance();

								if(tok == tok_RSQUARE){
									continue;
									dimension++;
									dimension_list = newDimension_List(exp, dimension_list);
								}
								else{
#ifdef DEBUG_MODE
									printf("FuncFParam out1 false\n");
#endif /*DEBUG_MODE*/
									return NULL;
								}
							}
							else{
#ifdef DEBUG_MODE
								printf("FuncFParam out2 false\n");
#endif /*DEBUG_MODE*/
								return NULL;
							}
						}
						else{
							recall(recall_pos);
							past funcfparam = newFuncFParam(type_str, id_str, dimension, dimension_list);
#ifdef DEBUG_MODE
							printf("FuncFParam out3 true\n");
#endif /*DEBUG_MODE*/
							return funcfparam;
						}
					}
				}
				else{
#ifdef DEBUG_MODE
					printf("FuncFParam out4 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else{
				recall(recall_pos);
				past funcfparam = newFuncFParam(type_str, id_str, 0, NULL);

#ifdef DEBUG_MODE
				printf("FuncFParam out5 true\n");
#endif /*DEBUG_MODE*/
				return funcfparam;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("FuncFParam out6 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("FuncFParam out7 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past Block(){
#ifdef DEBUG_MODE
	printf("Block in\n");
#endif /*DEBUG_MODE*/
	int recall_pos;
	past block = NULL;
	past block_list = NULL;

	advance();

	if(tok == tok_LBRACKET){
		recall_pos = save_recall_pos();

		advance();

		if(tok == tok_RBRACKET){
			block = newBlock(NULL);
#ifdef DEBUG_MODE
			printf("Block out1 true\n");
#endif /*DEBUG_MODE*/
			return block;
		}
		else{
			recall(recall_pos);

			while(1){
				
				past blockitem = BlockItem();
				if(blockitem != NULL){

					recall_pos = save_recall_pos();
					advance();

					if(tok == tok_RBRACKET){
						past block = newBlock(block_list);
#ifdef DEBUG_MODE
						printf("Block out2 true\n");
#endif /*DEBUG_MODE*/
						return block;
					}
					else{
						block_list = newBlock_list(blockitem, block_list);
						recall(recall_pos);
					}
				}
				else{
#ifdef DEBUG_MODE
					printf("Block out3 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}

			}
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("Block out4 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past BlockItem(){
#ifdef DEBUG_MODE
	printf("BlockItem in\n");
#endif /*DEBUG_MODE*/
	int recall_pos = save_recall_pos();

	past stmt = Stmt();
	if(stmt == NULL){
		recall(recall_pos);

		past decl = Decl();
		if(decl != NULL){
#ifdef DEBUG_MODE
			printf("BlockItem out1 true\n");
#endif /*DEBUG_MODE*/
			return decl;
		}
		else{
#ifdef DEBUG_MODE
			printf("BlockItem out2 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("BlockItem out3 true\n");
#endif /*DEBUG_MODE*/
		return stmt;
	}
}

past Stmt(){
#ifdef DEBUG_MODE
	printf("Stmt in\n");
#endif /*DEBUG_MODE*/
	
	int recall_pos = save_recall_pos();
	advance();

	if(tok == tok_IF){
		advance();

		if(tok == tok_LPAR){

			past cond = Cond();
			if(cond != NULL){
				advance();

				if(tok == tok_RPAR){

					past stmt = Stmt();
					if(stmt != NULL){

						past ifstmt = newIfStmt(cond, stmt);

						recall_pos = save_recall_pos();
						advance();

						if(tok == tok_ELSE){

							past else_stmt = Stmt();
							if(else_stmt != NULL){

								past selectstmt = newSelectStmt(ifstmt, else_stmt);

#ifdef DEBUG_MODE
								printf("Stmt out1 true\n");
#endif /*DEBUG_MODE*/
								return selectstmt;
							}
							else{
#ifdef DEBUG_MODE
								printf("Stmt out2 false\n");
#endif /*DEBUG_MODE*/
								return NULL;
							}
						}
						else{
							recall(recall_pos);
#ifdef DEBUG_MODE
							printf("Stmt out3 true\n");
#endif /*DEBUG_MODE*/
							return ifstmt;
						}
					}
					else{
#ifdef DEBUG_MODE
						printf("Stmt out4 false\n");
#endif /*DEBUG_MODE*/
						return NULL;
					}
				}
				else{
#ifdef DEBUG_MODE
					printf("Stmt out5 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else{
#ifdef DEBUG_MODE
				printf("Stmt out6 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("Stmt out7 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else if(tok == tok_WHILE){
		advance();

		if(tok == tok_LPAR){

			past cond = Cond();
			if(cond != NULL){

				advance();
				if(tok == tok_RPAR){

					past stmt = Stmt();
					if(stmt != NULL){
						past while_stmt = newWhileStmt(cond, stmt);
#ifdef DEBUG_MODE
						printf("Stmt out8 true\n");
#endif /*DEBUG_MODE*/
						return while_stmt;
					}
					else{
#ifdef DEBUG_MODE
						printf("Stmt out9 false\n");
#endif /*DEBUG_MODE*/
						return NULL;
					}
				}
				else{
#ifdef DEBUG_MODE
					printf("Stmt out10 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else{
#ifdef DEBUG_MODE
				printf("Stmt out11 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("Stmt out12 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else if(tok == tok_BREAK){
		advance();

		if(tok == tok_SEMICOLON){
			past breakstmt = newBreakStmt();
#ifdef DEBUG_MODE
			printf("Stmt out13 true\n");
#endif /*DEBUG_MODE*/
			return breakstmt;
		}
		else{
#ifdef DEBUG_MODE
			printf("Stmt out14 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else if(tok == tok_CONTINUE){
		advance();

		if(tok == tok_SEMICOLON){
			past continuestmt = newContinueStmt();
#ifdef DEBUG_MODE
			printf("Stmt out15 true\n");
#endif /*DEBUG_MODE*/
			return continuestmt;
		}
		else{
#ifdef DEBUG_MODE
			printf("Stmt out16 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}

	}
	else if(tok == tok_RETURN){
		recall_pos = save_recall_pos();
		advance();

		if(tok == tok_SEMICOLON){
			past returnstmt = newReturnStmt(NULL);
#ifdef DEBUG_MODE
			printf("Stmt out17 true\n");
#endif /*DEBUG_MODE*/
			return returnstmt;
		}

		recall(recall_pos);

		past exp = Exp();
		if(exp != NULL){
			advance();

			if(tok == tok_SEMICOLON){
				past returnstmt = newReturnStmt(exp);
#ifdef DEBUG_MODE
				printf("Stmt out17 true\n");
#endif /*DEBUG_MODE*/
				return returnstmt;
			}
			else{
#ifdef DEBUG_MODE
				printf("Stmt out18 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("Stmt out19 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}		
	}
	else if(tok == tok_SEMICOLON){
		past nullstmt = newNullStmt();
#ifdef DEBUG_MODE
		printf("Stmt out20 true\n");
#endif /*DEBUG_MODE*/
		return nullstmt;
	}
	else{
		recall(recall_pos);
		recall_pos = save_recall_pos();

		past lval = LVal();
		if(lval != NULL){
			advance();

			if(tok == tok_ASSIGN){

				past exp = Exp();
				if(exp != NULL){

					advance();
					if(tok == tok_SEMICOLON){
						past assign_stmt = newOpcode("=", lval, exp);
#ifdef DEBUG_MODE
						printf("Stmt out21 true\n");
#endif /*DEBUG_MODE*/
						return assign_stmt;
					}
					else{
#ifdef DEBUG_MODE
						printf("Stmt out22 false\n");
#endif /*DEBUG_MODE*/
						return NULL;
					}
				}
				else{
#ifdef DEBUG_MODE
					printf("Stmt out23 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else{
				recall(recall_pos);

				past exp = Exp();
				if(exp != NULL){
					advance();

					if(tok == tok_SEMICOLON){
#ifdef DEBUG_MODE
						printf("Stmt out24 true\n");
#endif /*DEBUG_MODE*/
						return exp;
					}
					else{
#ifdef DEBUG_MODE
						printf("Stmt out25 false\n");
#endif /*DEBUG_MODE*/
						return NULL;
					}
				}
				else{
#ifdef DEBUG_MODE
					printf("Stmt out26 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
		}
		else{
			recall(recall_pos);

			past exp = Exp();
			if(exp != NULL){
				advance();

				if(tok == tok_SEMICOLON){
#ifdef DEBUG_MODE
					printf("Stmt out27 true\n");
#endif /*DEBUG_MODE*/
					return exp;
				}
				else{
#ifdef DEBUG_MODE
					printf("Stmt out28 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else {
				recall(recall_pos);

				past block = Block();
				if(block != NULL){
#ifdef DEBUG_MODE
					printf("Stmt out29 true\n");
#endif /*DEBUG_MODE*/
					return block;
				}
				else{
#ifdef DEBUG_MODE
					printf("Stmt out30 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
		}
	}
}

past Exp(){
#ifdef DEBUG_MODE
	printf("Exp in\n");
#endif /*DEBUG_MODE*/

	past exp = AddExp();

	if(exp != NULL){
#ifdef DEBUG_MODE
		printf("Exp out1 true\n");
#endif /*DEBUG_MODE*/
		return exp;
	}
	else{
#ifdef DEBUG_MODE
		printf("Exp out2 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past Cond(){
#ifdef DEBUG_MODE
	printf("Cond in\n");
#endif /*DEBUG_MODE*/

	past lorexp = LOrExp();
	if(lorexp != NULL){
#ifdef DEBUG_MODE
		printf("Cond out1 true\n");
#endif /*DEBUG_MODE*/
		return lorexp;
	}
	else{
#ifdef DEBUG_MODE
		printf("Cond out2 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past LVal(){
#ifdef DEBUG_MODE
	printf("LVal in\n");
#endif /*DEBUG_MODE*/

	past dimension_list = NULL;
	int recall_pos = save_recall_pos();

	advance();
	if(tok == tok_ID){

		char id_str[100];
		strcpy(id_str, lex_str);

		while (1){
			recall_pos = save_recall_pos();
			advance();

			if(tok == tok_LSQUARE){
				past exp = Exp();

				if(exp != NULL){
					advance();

					if(tok == tok_RSQUARE){
						dimension_list = newDimension_List(exp, dimension_list);

						continue;
					}
					else{
#ifdef DEBUG_MODE
						printf("LVal out1 false\n");
#endif /*DEBUG_MODE*/
						return NULL;
					}
				}
				else{
#ifdef DEBUG_MODE
					printf("LVal out2 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else{
				recall(recall_pos);

				past var = newVar(id_str, 0, dimension_list);
#ifdef DEBUG_MODE
				printf("LVal out3 true\n");
#endif /*DEBUG_MODE*/
				return var;
			}
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("LVal out4 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past PrimaryExp(){
#ifdef DEBUG_MODE
	printf("PrimaryExp in\n");
#endif /*DEBUG_MODE*/

	int recall_pos = save_recall_pos();
	advance();

	if(tok == tok_LPAR){

		past exp = Exp();
		if(exp != NULL){
			advance();

			if(tok == tok_RPAR){
#ifdef DEBUG_MODE
				printf("PrimaryExp out1 true\n");
#endif /*DEBUG_MODE*/
				return exp;
			}
			else{
#ifdef DEBUG_MODE
				printf("PrimaryExp out2 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("PrimaryExp out3 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
		recall(recall_pos);

		past num = Number();
		if(num != NULL){
#ifdef DEBUG_MODE
			printf("PrimaryExp out4 true\n");
#endif /*DEBUG_MODE*/
			return num;
		}
		else{
			recall(recall_pos);

			past lval = LVal();
			if(lval != NULL){
#ifdef DEBUG_MODE
				printf("PrimaryExp out5 true\n");
#endif /*DEBUG_MODE*/
				return lval;
			}
			else{
#ifdef DEBUG_MODE
				printf("PrimaryExp out6 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
	}
}

past Number(){
#ifdef DEBUG_MODE
	printf("Number in\n");
#endif /*DEBUG_MODE*/

	advance();

	if(tok == tok_INTEGER){
		past num = newNum(atoi(lex_str));
#ifdef DEBUG_MODE
		printf("Number out1 true\n");
#endif /*DEBUG_MODE*/
		return num;
	}
	else{
#ifdef DEBUG_MODE
		printf("Number out2 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past UnaryExp(){
#ifdef DEBUG_MODE
	printf("UnaryExp in\n");
#endif /*DEBUG_MODE*/
	int recall_pos = save_recall_pos();
	advance();

	if(tok == tok_ID){
		char id_str[100];
		strcpy(id_str, lex_str);

		advance();

		if(tok == tok_LPAR){

			recall_pos = save_recall_pos();
			advance();

			if(tok == tok_RPAR){
				past func = newFunc(id_str, NULL);
#ifdef DEBUG_MODE
				printf("UnaryExp out1 true\n");
#endif /*DEBUG_MODE*/
				return func;
			}
			else{
				recall(recall_pos);

				past funcrparams = FuncRParams();
				if(funcrparams != NULL){

					advance();
					
					if(tok == tok_RPAR){
						past func = newFunc(id_str, funcrparams);
#ifdef DEBUG_MODE
						printf("UnaryExp out2 true\n");
#endif /*DEBUG_MODE*/
						return func;
					}
					else{
#ifdef DEBUG_MODE
						printf("UnaryExp out3 false\n");
#endif /*DEBUG_MODE*/
						return NULL;	
					}
				}
				else{
#ifdef DEBUG_MODE
					printf("UnaryExp out4 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			} 
		}
		else{
			recall(recall_pos);

			past primaryexp = PrimaryExp();
			if(primaryexp != NULL){
#ifdef DEBUG_MODE
				printf("UnaryExp out5 true\n");
#endif /*DEBUG_MODE*/
				return primaryexp;
			}
			else{
#ifdef DEBUG_MODE
				printf("UnaryExp out6 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
	}
	else {
		recall(recall_pos);

		past primaryexp = PrimaryExp();
		if(primaryexp != NULL){
#ifdef DEBUG_MODE
			printf("UnaryExp out7 true\n");
#endif /*DEBUG_MODE*/
			return primaryexp;
		}
		else {
			recall(recall_pos);

			past opcode = UnaryOp();
			if(opcode != NULL){

				past unaryexp = UnaryExp();
				if(unaryexp != NULL){
					opcode->right = unaryexp;
#ifdef DEBUG_MODE
					printf("UnaryExp out8 true\n");
#endif /*DEBUG_MODE*/
					return opcode;
				}
				else{
#ifdef DEBUG_MODE
					printf("UnaryExp out9 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else{
#ifdef DEBUG_MODE
				printf("UnaryExp out10 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}	
		}
	}
}

past UnaryOp(){
#ifdef DEBUG_MODE
	printf("UnaryOp in\n");
#endif /*DEBUG_MODE*/

	advance();
	if(tok == tok_ADD || tok == tok_SUB || tok == tok_NOT){
		past opcode = newOpcode(lex_str, NULL, NULL);
#ifdef DEBUG_MODE
		printf("UnaryOp out1 true\n");
#endif /*DEBUG_MODE*/
		return opcode;
	}
	else{
#ifdef DEBUG_MODE
		printf("UnaryOp out2 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past FuncRParams(){
#ifdef DEBUG_MODE
	printf("FuncRParams in\n");
#endif /*DEBUG_MODE*/
	past funcrparams_list = NULL;
	int recall_pos;

	past exp = Exp();
	if(exp != NULL){

		recall_pos = save_recall_pos();
		advance();

		while (1){
			if(tok == tok_COMMA){

				funcrparams_list = newFuncRParams_List(exp, funcrparams_list);

				exp = Exp();
				if(exp != NULL){
					recall_pos = save_recall_pos();
					advance();
				}
				else{
#ifdef DEBUG_MODE
					printf("FuncRParams out1 false\n");
#endif /*DEBUG_MODE*/
					return NULL;
				}
			}
			else{
				recall(recall_pos);
				funcrparams_list= newFuncRParams_List(exp, funcrparams_list);
#ifdef DEBUG_MODE
				printf("FuncRParams out2 true\n");
#endif /*DEBUG_MODE*/
				return funcrparams_list;
			}
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("FuncRParams out3 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past MulExp(){
#ifdef DEBUG_MODE
	printf("MulExp in\n");
#endif /*DEBUG_MODE*/

	past unaryexp = UnaryExp();
	if(unaryexp != NULL){

		int recall_pos = save_recall_pos();
		
		advance();
		if(tok == tok_SEMICOLON){
			recall(recall_pos);
#ifdef DEBUG_MODE
			printf("MulExp out1 true\n");
#endif /*DEBUG_MODE*/

			return unaryexp;
		}

		recall(recall_pos);

		recall_pos = save_recall_pos();

		past mulexp_extend = MulExp_Extend();
		if(mulexp_extend != NULL){
			mulexp_extend->left = unaryexp;
#ifdef DEBUG_MODE
			printf("MulExp out2 true\n");
#endif /*DEBUG_MODE*/
			return mulexp_extend;
		}
		else{
			recall(recall_pos);
#ifdef DEBUG_MODE
			printf("MulExp out3 false\n");
#endif /*DEBUG_MODE*/
			return unaryexp;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("MulExp out4 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past AddExp(){
#ifdef DEBUG_MODE
	printf("AddExp in\n");
#endif /*DEBUG_MODE*/

	int recall_pos = save_recall_pos();

	past mulexp = MulExp();
	if(mulexp != NULL){

		recall_pos = save_recall_pos();
		advance();

		if(tok == tok_SEMICOLON){
			recall(recall_pos);
#ifdef DEBUG_MODE
			printf("AddExp out1 true\n");
#endif /*DEBUG_MODE*/
			return mulexp;
		}
		
		recall(recall_pos);

		int recall_pos = save_recall_pos();

		past addexp_extend = AddExp_Extend();
		if(addexp_extend != NULL){
			addexp_extend->left = mulexp;
#ifdef DEBUG_MODE
			printf("AddExp out2 true\n");
#endif /*DEBUG_MODE*/
			return addexp_extend;
		}
		else{
			recall(recall_pos);
#ifdef DEBUG_MODE
			printf("AddExp out3 false\n");
#endif /*DEBUG_MODE*/
			return mulexp;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("AddExp out4 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past RelExp(){
#ifdef DEBUG_MODE
	printf("RelExp in\n");
#endif /*DEBUG_MODE*/

	past addexp = AddExp();
	if(addexp != NULL){

		int recall_pos = save_recall_pos();

		past relexp_extend = RelExp_Extend();
		if(relexp_extend != NULL){

			relexp_extend->left = addexp;
			return relexp_extend;
		}
		else{
			recall(recall_pos);
			return addexp;
		}
	}
	else
		return NULL;
}

past EqExp(){
#ifdef DEBUG_MODE
	printf("EqExp in\n");
#endif /*DEBUG_MODE*/

	past relexp = RelExp();
	if(relexp != NULL){

		int recall_pos = save_recall_pos();

		past eqexp_extend = EqExp_Extend();
		if(eqexp_extend != NULL){

			eqexp_extend->left = relexp;
		}
		else{
			recall(recall_pos);
			return relexp;
		}
	}
	else
		return NULL;
}

past LAndExp(){
#ifdef DEBUG_MODE
	printf("LAndExp in\n");
#endif /*DEBUG_MODE*/

	past eqexp = EqExp();
	if(eqexp != NULL){

		int recall_pos = save_recall_pos();

		past landexp_extend = LAndExp_Extend();
		if(landexp_extend != NULL){
			landexp_extend->left = eqexp;
		}
		else{
			recall(recall_pos);
			return eqexp;
		}
	}
	else
		return NULL;
}

past LOrExp(){
#ifdef DEBUG_MODE
	printf("LOrExp in\n");
#endif /*DEBUG_MODE*/

	past landexp = LAndExp();
	if(landexp != NULL){

		int recall_pos = save_recall_pos();

		past lorexp_extend = LOrExp_Extend();
		if(lorexp_extend != NULL){
			lorexp_extend->left = landexp;
		}
		else{
			recall(recall_pos);
			return landexp;
		}
	}
	else
		return NULL;
}

past MulExp_Extend(void){
#ifdef DEBUG_MODE
	printf("MulExp_Extend in\n");
#endif /*DEBUG_MODE*/

	int recall_pos = save_recall_pos();
	advance();

	if(tok == tok_MUL || tok == tok_DIV || tok == tok_MODULO){
		char opcode_str[5];
		strcpy(opcode_str, lex_str);

		past unaryexp = UnaryExp();
		if(unaryexp != NULL){

			recall_pos = save_recall_pos();
			past mulexp_extend = MulExp_Extend();

			if(mulexp_extend != NULL){
				
				mulexp_extend->left = unaryexp;
				past opcode = newOpcode(opcode_str, NULL, mulexp_extend);
#ifdef DEBUG_MODE
				printf("MulExp_Extend out1 true\n");
#endif /*DEBUG_MODE*/

				return opcode;
			}
			else{
				recall(recall_pos);
				past opcode = newOpcode(opcode_str, NULL, unaryexp);
#ifdef DEBUG_MODE
				printf("MulExp_Extend out2 false\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("MulExp_Extend out3 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("MulExp_Extend out4 true\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past AddExp_Extend(void){
#ifdef DEBUG_MODE
	printf("AddExp_Extend in\n");
#endif /*DEBUG_MODE*/

	int recall_pos = save_recall_pos();

	advance();
	if(tok == tok_ADD || tok == tok_SUB){
		char opcode_str[5];
		strcpy(opcode_str, lex_str);

		past mulexp = MulExp();
		if(mulexp != NULL){

			recall_pos = save_recall_pos();
			past addexp_extend = AddExp_Extend();

			if(addexp_extend != NULL){
				addexp_extend->left = mulexp;
				past opcode = newOpcode(opcode_str, NULL, addexp_extend);
#ifdef DEBUG_MODE
				printf("AddExp_Extend out1 true\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
			else{
				recall(recall_pos);
				past opcode = newOpcode(opcode_str, NULL, mulexp);
#ifdef DEBUG_MODE
				printf("AddExp_Extend out2 false\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("AddExp_Extend out3 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("AddExp_Extend out4 true\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past RelExp_Extend(void){
#ifdef DEBUG_MODE
	printf("RelExp_Extend in\n");
#endif /*DEBUG_MODE*/

	int recall_pos = save_recall_pos();
	advance();

	if(tok == tok_LESS || tok == tok_GREAT || tok == tok_LESSEQ || tok == tok_GREATEQ){
		char opcode_str[5];
		strcpy(opcode_str, lex_str);

		past addexp = AddExp();
		if(addexp != NULL){

			recall_pos = save_recall_pos();
			past relexp_extend = RelExp_Extend();

			if(relexp_extend != NULL){

				relexp_extend->left = addexp;
				past opcode = newOpcode(opcode_str, NULL, relexp_extend);
#ifdef DEBUG_MODE
				printf("RelExp_Extend out1 true\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
			else{
				recall(recall_pos);
				past opcode = newOpcode(opcode_str, NULL, addexp);
#ifdef DEBUG_MODE
				printf("RelExp_Extend out2 false\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("RelExp_Extend out3 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
		recall(recall_pos);
#ifdef DEBUG_MODE
		printf("RelExp_Extend out4 true\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past EqExp_Extend(void){
#ifdef DEBUG_MODE
	printf("EqExp_Extend in\n");
#endif /*DEBUG_MODE*/

	int recall_pos = save_recall_pos();
	advance();
		
	if(tok == tok_EQ || tok == tok_NOTEQ){
		char opcode_str[5];
		strcpy(opcode_str, lex_str);

		past relexp = RelExp();
		if(relexp != NULL){
			
			recall_pos = save_recall_pos();
			past eqexp_extend = EqExp_Extend();

			if(eqexp_extend != NULL){

				eqexp_extend->left = relexp;
				past opcode = newOpcode(opcode_str, NULL, eqexp_extend);
#ifdef DEBUG_MODE
				printf("EqExp_Extend out1 true\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
			else{
				recall(recall_pos);
				past opcode = newOpcode(opcode_str, NULL, relexp);
#ifdef DEBUG_MODE
				printf("EqExp_Extend out2 false\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("EqExp_Extend out3 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("EqExp_Extend out4 true\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past LAndExp_Extend(void){
#ifdef DEBUG_MODE
	printf("LAndExp_Extend in\n");
#endif /*DEBUG_MODE*/
	int recall_pos = save_recall_pos();
	advance();

	if(tok == tok_AND){
		char opcode_str[5];
		strcpy(opcode_str, lex_str);

		past eqexp = EqExp();	
		if(eqexp != NULL){

			recall_pos = save_recall_pos();
			past landexp_extend = LAndExp_Extend();

			if(landexp_extend != NULL){

				landexp_extend->left = eqexp;
				past opcode = newOpcode(opcode_str, NULL, landexp_extend);
#ifdef DEBUG_MODE
				printf("LAndExp_Extend out1 true\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
			else{
				recall(recall_pos);
				past opcode = newOpcode(opcode_str, NULL, eqexp);
#ifdef DEBUG_MODE
				printf("LAndExp_Extend out2 false\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("LAndExp_Extend out3 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("LAndExp_Extend out4 true\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past LOrExp_Extend(void){
#ifdef DEBUG_MODE
	printf("LOrExp_Extend in\n");
#endif /*DEBUG_MODE*/
	int recall_pos = save_recall_pos();
	advance();

	if(tok == tok_OR){
		char opcode_str[5];
		strcpy(opcode_str, lex_str);

		past landexp = LAndExp();
		if(landexp != NULL){

			recall_pos = save_recall_pos();
			past lorexp_extend = LOrExp_Extend();

			if(lorexp_extend != NULL){

				lorexp_extend->left = landexp;
				past opcode = newOpcode(opcode_str, NULL, lorexp_extend);
#ifdef DEBUG_MODE
				printf("LOrExp_Extend out1 true\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
			else{
				recall(recall_pos);
				past opcode = newOpcode(opcode_str, NULL, landexp);
#ifdef DEBUG_MODE
				printf("LOrExp_Extend out2 false\n");
#endif /*DEBUG_MODE*/
				return opcode;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("LOrExp_Extend out3 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
#ifdef DEBUG_MODE
		printf("LOrExp_Extend out4 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past ConstExp(){
#ifdef DEBUG_MODE
	printf("ConstExp in\n");
#endif /*DEBUG_MODE*/

	past addexp = AddExp();
	if(addexp != NULL){
#ifdef DEBUG_MODE
		printf("ConstExp out1 true\n");
#endif /*DEBUG_MODE*/
		return addexp;
	}
	else{
#ifdef DEBUG_MODE
		printf("ConstExp out2 false\n");
#endif /*DEBUG_MODE*/
		return NULL;
	}
}

past InitVal(){
#ifdef DEBUG_MODE
	printf("InitVal in\n");
#endif /*DEBUG_MODE*/
	int recall_pos = save_recall_pos();
	advance();

	if(tok == tok_LBRACKET){
		past initval = InitVal();
		if(initval != NULL){

			past initvals = newInitVal(initval, initvals);
			advance();

			while(1){
				if(tok == tok_COMMA){

					initval = InitVal();
					if(initval != NULL){

						initvals = newInitVal(initval, initvals);
						advance();
					}
					else{
#ifdef DEBUG_MODE
						printf("InitVal out1 false\n");
#endif /*DEBUG_MODE*/
						return NULL;
					}
				}
				else
					break;
			}

			if(tok == tok_RBRACKET){
#ifdef DEBUG_MODE
				printf("InitVal out2 true\n");
#endif /*DEBUG_MODE*/
				return initvals;
			}
			else{
#ifdef DEBUG_MODE
				printf("InitVal out3 false\n");
#endif /*DEBUG_MODE*/
				return NULL;
			}
		}
		else{
#ifdef DEBUG_MODE
			printf("InitVal out4 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
	else{
		recall(recall_pos);

		past exp = Exp();
		if(exp != NULL){

			past initval = newInitVal(exp, NULL);
#ifdef DEBUG_MODE
			printf("InitVal out5 true\n");
#endif /*DEBUG_MODE*/
			return initval;
		}
		else{
#ifdef DEBUG_MODE
			printf("InitVal out6 false\n");
#endif /*DEBUG_MODE*/
			return NULL;
		}
	}
}

void ShowAst(past node, int nest){
	if(node == NULL)
		return;

	for(int i = 0; i < nest; i ++)
		printf("  ");
	if(strcmp(node->nodeType, "intValue") == 0)
		printf("%s %d\n", node->nodeType, node->ivalue);
	else if(strcmp(node->nodeType, "expr") == 0)
		printf("%s '%c'\n", node->nodeType, (char)node->ivalue);
	else if(strcmp)
	else;
	ShowAst(node->left, nest+1);
	ShowAst(node->right, nest+1);

}

void FreeAst(past node){
	if(node == NULL)
		return;

	FreeAst(node->left);
	FreeAst(node->right);
	free(node);
}

// 递归下降分析
past Recursive_Parse(void){
	past ret;
	
	first_get = 1;
	getChar();

	return ret = CompUnit();
}

// 分析单文件
int main(int argc, char**argv){
	// fflush(stdout);
	// setvbuf(stdout, NULL, _IONBF, 0);
	// freopen("./my.log", "w", stdout); //打印到my.log文件

	++argv; --argc;
	if(argc < 1){
		printf("请输入要分析的文件\n");
		exit(1);
	}
	
	ana_file = fopen(argv[0], "r");

	if(ana_file == NULL){
		printf("要分析的文件不存在\n");
		exit(1);
	}

	past r = Recursive_Parse();
	ShowAst(r, 0);
	FreeAst(r);

	fclose(ana_file);
	return 0;
}



// 遍历test_cases文件夹
// gcc rdparser.c man_lex.c -o rdparser
// ./rdparser /home/用户名/WorkSpace/Compile_Lab/test_cases/
// int main(int argc, char* argv[]){
    
//     DIR *dir;
//     struct dirent *sourcedir;
//     char *sourcefile, *copyfile; 
//     char sourcepath[80], objectpath[80], temppath[80];  // temppath 是strcat的参数, strcat 会改变传入的第一个参数， 如果直接传 sourcepath 第二次的参数就是错的
//     char flow;

// //     fflush(stdout);
// //     setvbuf(stdout, NULL, _IONBF, 0);
// //     freopen("./my.log", "w", stdout); //打印到my.log文件

// //     /* 检查输入参数 */
// //     for(int i = 1; i < argc; i++)
// //         printf("\nmain函数的参数%d为%s\n",i,argv[i]);

//     /* 得到源路径 */
//     strcpy((char*)&sourcepath, argv[1]);

//     /* 得到文件夹下的文件名 */
//     dir = opendir(argv[1]);
    
//     /* 遍历文件夹 */
//     while (sourcedir = readdir(dir)){
//         if (sourcedir->d_name[0] != '.'){

//             /* 得到当前要分析的文件 */
//             copyfile = sourcedir->d_name;
//             printf("%s\n", copyfile);

//             /* 得到源路径加文件 */
//             strcpy((char*)&temppath, (char*)&sourcepath);
//             sourcefile = strcat(temppath, copyfile);
//             // printf("\nsource file:%s\n", sourcefile);

//             /* 得到源文件文件指针 */
//             ana_file = fopen(sourcefile, "r");
//             if(ana_file == NULL){
//                 printf("\n源文件%s不存在, 程序退出 \n", sourcefile);
//                 exit(0);
//             }
	
// 	    past r = Recursive_Parse();
// 	    ShowAst(r, 0);
// 	    FreeAst(r);

//             /* 关闭文件 */
//             fclose(ana_file);
//         }
//     }

//     /* 关闭文件夹 */
//     closedir(dir);

//     return 0;
// }
