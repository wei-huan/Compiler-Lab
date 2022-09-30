#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yy.lex.h"

enum TokenType {
	IF = 245,
	ELSE = 246,
	WHILE = 247,
	RETURN = 248,
	PRINT = 249,
	SCAN = 250,
	CMP = 251,
	ASSIGN = 252,
	INT = 253,
	STR = 254,
	VOID = 255,
	STRING = 256,
	ID = 257,
	NUMBER = 258,
	EOL = 259
};

extern int yylex();
extern int yylval;
extern char* yytext;
extern FILE* yyin;
int tok;
typedef struct _ast ast;
typedef struct _ast *past;
struct _ast {
	char *nodeType;
	char *content;
	past left;	
	past right;			
};
void advance();
past program();
past external_declaration();
past decl_or_stmt();
past declarator_list();
past intstr_list();
past initializer();
past declarator();
past parameter_list();
past parameter();
past type();
past statement();
past statement_list();
past expression_statement();
past expr();
past cmp_expr();
past add_expr();
past mul_expr();
past primary_expr();
past expr_list();
past id_list();

past newAstNode(char *nodeType, char *content, past left, past right);
void showAst(past node, int nest);
void freeAst(past node);

int main(int argc, char **argv)
{
	yyin = fopen("test.c", "r");
	advance();
	past root = program();
	showAst(root, 0);
	freeAst(root);

	system("pause");
	return 0;
}

void advance() {
	tok = yylex();
	printf("%s\n", yytext);
}

past newAstNode(char *nodeType, char *content, past left, past right) {
	past node = (past)malloc(sizeof(ast));
	if (node == NULL) {
		printf("run out of memory.\n");
		exit(0);
	}
	node->nodeType = nodeType;
	node->content = content;
	node->left = left;
	node->right = right;
	return node;
}

void showAst(past node, int nest) {
	if (node == NULL)
		return;

	int i = 0;
	for (i = 0; i < nest; i++)
		printf("  ");
	printf("%s: %s\n", node->nodeType, node->content);
	showAst(node->left, nest + 1);
	showAst(node->right, nest + 1);
}

void freeAst(past node) {
	if (node == NULL)
		return;
	freeAst(node->left);
	freeAst(node->right);
	free(node);
}
//program
//	: external_declaration
//	| program external_declaration
//	;
past program() {
	past root = NULL;
	if (root = external_declaration())
		root = newAstNode("program", "", root, program());
	return root;
}
//external_declaration
//	: type declarator decl_or_stmt
//	;
past external_declaration() {
	past root = NULL;
	if (root = type()) {
		root = newAstNode("external_declaration", "", root, newAstNode("typebody", "", NULL, NULL));
		if (root->right->left = declarator())
			if (root->right->right = decl_or_stmt())
				;
	}
	return root;
}
//decl_or_stmt
//	: '{' statement_list '}'
//	| '{' '}'
//	| ',' declarator_list ';'
//	| ';'
//	;
past decl_or_stmt() {
	past root = NULL;
	if (tok == '{') {
		advance();
		if (root = statement_list())
			root = newAstNode("decl_or_stmt", "", root, NULL);
		if (tok == '}')
			advance();
	}
	else if (tok == ',') {
		advance();
		if (root = declarator_list())
			if (tok == ';')
				advance();
	}
	else if (tok == ';')
		advance();
	return root;
}
//declarator_list
//	: declarator
//	| declarator_list ',' declarator
//	;
past declarator_list() {
	past root = NULL;
	if (root = declarator()) {
		root = newAstNode("declarator_list", "", root, NULL);
		if (tok == ',') {
			advance();
			root->right = declarator_list();
		}
	}
	return root;
}
//intstr_list
//	: initializer
//	| intstr_list ',' initializer
//	;
past intstr_list() {
	past root = NULL;
	if (root = initializer()) {
		root = newAstNode("intstr_list", "", root, NULL);
		if (tok == ',') {
			advance();
			root->right = intstr_list();
		}
	}
	return root;
}
//initializer
//	: NUMBER
//	| STRING
//	;
past initializer() {
	past root = NULL;
	if (tok == NUMBER) {
		root = newAstNode("number", strdup(yytext), NULL, NULL);
		advance();
	}
	else if (tok == STRING) {
		root = newAstNode("string", strdup(yytext), NULL, NULL);
		advance();
	}
	return root;
}
//declarator
//	: ID
//	| ID '=' expr
//	| ID '(' parameter_list ')'
//	| ID '(' ')'
//	| ID '[' expr ']'
//	| ID '[' ']'
//	| ID '[' expr ']' '=' '{' intstr_list '}'
//	| ID '[' ']' '=' '{' intstr_list '}'
//	;
past declarator() {
	past root = NULL;
	if (tok == ID) {
		root = newAstNode("declarator", strdup(yytext), NULL, NULL);
		advance();
		if (tok == '=') {
			advance();
			root->right = expr();
		}
		else if (tok == '(') {
			advance();
			root->left = parameter_list();
			if (tok == ')')
				advance();
		}
		else if (tok == '[') {
			advance();
			root->left = expr();
			if (tok == ']') {
				advance();
				if (tok == '=') {
					advance();
					if (tok == '{') {
						advance();
						if (root->right = intstr_list()) {
							root->right = newAstNode("InitListExpr", "", root->right, NULL);
							if (tok == '}')
								advance();
						}
					}
				}
			}
		}
	}
	return root;
}
//parameter_list
//	: parameter
//	| parameter_list ',' parameter
//	;
past parameter_list() {
	past root = NULL;
	if (root = parameter()) {
		root = newAstNode("parameter_list", "", root, NULL);
		if (tok == ',') {
			advance();
			root->right = parameter_list();
		}
	}
	return root;
}
//parameter
//	: type ID
//	;
past parameter() {
	past root = NULL;
	if (root = type())
		if (tok == ID) {
			root = newAstNode("parameter", "", root, newAstNode("ID", strdup(yytext), NULL, NULL));
			advance();
		}
	return root;
}
//type
//	: INT
//	| STR
//	| VOID
//	;
past type() {
	past root = NULL;
	if (tok == INT || tok == STR || tok == VOID) {
		root = newAstNode("type", strdup(yytext), NULL, NULL);
		advance();
	}
	return root;
}
//statement
//	: type declarator_list ';'
//	| '{' statement_list '}'
//	| expr_statement	
//	| IF '(' expr ')' statement
//	| IF '(' expr ')' statement ELSE statement
//	| WHILE '(' expr ')' statement
//	| RETURN ';'
//	| RETURN expr ';'
//	| PRINT ';'
//	| PRINT expr_list ';'
//	| SCAN id_list ';'
//	;
past statement() {
	past root = NULL;
	if (root = type()) {
		root = newAstNode("declarator_statement", "", root, NULL);
		if (root->right = declarator_list())
			if (tok == ';')
				advance();
	}
	else if (tok == '{') {
		advance();
		if (root = statement_list()) {
			root = newAstNode("compound_statement", "", root, NULL);
			if (tok == '}')
				advance();
		}
	}
	else if (root = expression_statement())
		;
	else if (tok == IF) {
		root = newAstNode("if_statement", "", NULL, newAstNode("ifbody", "", NULL, NULL));
		advance();
		if (tok == '(') {
			advance();
			if (root->left = expr())
				if (tok == ')') {
					advance();
					if (root->right->left = statement())
						if (tok == ELSE) {
							advance();
							if (root->right->right = statement())
								;
						}
				}
		}
	}
	else if (tok == WHILE) {
		root = newAstNode("while_statement", "", NULL, NULL);
		advance();
		if (tok == '(') {
			advance();
			if (root->left = expr())
				if (tok == ')') {
					advance();
					if (root->right = statement())
						;
				}
		}
	}
	else if (tok == RETURN) {
		root = newAstNode("return_statement", "", NULL, NULL);
		advance();
		root->left = expr();
		if (tok == ';')
			advance();
	}
	else if (tok == PRINT) {
		root = newAstNode("print_statement", "", NULL, NULL);
		advance();
		root->left = expr_list();
		if (tok == ';')
			advance();
	}
	else if (tok == SCAN) {
		root = newAstNode("scan_statement", "", NULL, NULL);
		advance();
		if (root->left = id_list())
			if (tok == ';')
				advance();
	}
	return root;
}
//statement_list
//	: statement
//	| statement_list statement
//	;
past statement_list() {
	past right = statement();
	if (right == NULL)
		return NULL;
	else
		return newAstNode("statement_list", "", statement_list(), right);
}
//expression_statement
//	: ';'
//	| expr ';'
//	;
past expression_statement() {
	past root = NULL;
	if (tok == ';')
		advance();
	else if (root = expr()) {
		if (tok == ';')
			advance();
	}
	return root;
}
//expr
//	: cmp_expr
//	;
past expr() {
	return cmp_expr();
}
//cmp_expr
//	: add_expr
//	| cmp_expr CMP add_expr
//	;
past cmp_expr() {
	past root = NULL;
	root = add_expr();
	if (root)
		while (tok==CMP) {
			root = newAstNode("cmp_expr", strdup(yytext), root, NULL);
			advance();
			root->right = add_expr();
		}
	return root;
}
//add_expr
//	: mul_expr
//	| add_expr '+' mul_expr
//	| add_expr '-' mul_expr
//	;
past add_expr() {
	past root = NULL;
	root = mul_expr();
	if (root)
		while (tok == '+' || tok == '-') {
			root = newAstNode("add_expr", strdup(yytext), root, NULL);
			advance();
			root->right = mul_expr();
		}
	return root;
}
//mul_expr
//	: primary_expr
//	| mul_expr '*' primary_expr
//	| mul_expr '/' primary_expr
//	| mul_expr '%' primary_expr
//	| '-' primary_expr
//	;
past mul_expr() {
	past root = NULL;
	root = primary_expr();
	if (root) {
		while (tok == '*' || tok == '/' || tok == '%') {
			root = newAstNode("mul_expr", strdup(yytext), root, NULL);
			advance();
			root->right = primary_expr();
		}
	}
	else if (tok == '-') {
		root = newAstNode("mul_expr", strdup(yytext), NULL, NULL);
		advance();
		root->right = primary_expr();
	}
	return root;
}
//primary_expr
//	: ID '(' expr_list ')'
//	| ID '(' ')'
//	| '(' expr ')'
//	| ID
//	| NUMBER
//	| STRING
//	| ID ASSIGN expr
//	| ID '=' expr
//	| ID '[' expr ']'
//	| ID '[' expr ']' '=' expr
//	;
past primary_expr() {
	past root = NULL;
	if (tok == ID) {
		root = newAstNode("primary_expr", strdup(yytext), NULL, NULL);
		advance();
		if (tok == '(') {
			advance();
			root->left = expr_list();
			if (tok == ')')
				advance();
		}
		else if (tok == '[') {
			advance();
			if (root->left = expr())
				if (tok == ']') {
					advance();
					if (tok == '=') {
						root = newAstNode("Operator", strdup(yytext), root, NULL);
						advance();
						root->right = expr();
					}
				}
		}
		else if (tok == '=' || tok == ASSIGN) {
			root = newAstNode("primary_expr", strdup(yytext), root, NULL);
			advance();
			root->right = expr();
		}
	}
	else if (tok == '(') {
		advance();
		if (root = expr())
			if (tok == ')')
				advance();
	}
	else if (tok == NUMBER) {
		root = newAstNode("number", strdup(yytext), NULL, NULL);
		advance();
	}

	else if (tok == STRING) {
		root = newAstNode("string", strdup(yytext), NULL, NULL);
		advance();
	}
	return root;
}
//expr_list
//	: expr
//	| expr_list ',' expr
//	;
past expr_list() {
	past root = NULL;
	if (root = expr()) {
		root = newAstNode("expr_list", "", root, NULL);
		if (tok == ',') {
			advance();
			root->right = expr_list();
		}
	}
	return root;
}
//id_list
//	: ID
//	| id_list ',' ID
//	;
past id_list() {
	past root = NULL;
	if (tok == ID) {
		root = newAstNode("id_list", NULL, newAstNode("ID", strdup(yytext), NULL, NULL), NULL);
		advance();
		if (tok == ',') {
			advance();
			root->right = id_list();
		}
	}
	return root;
}
