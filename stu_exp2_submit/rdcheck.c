#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yy.lex.h"

enum yytokentype {
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

void advance();
int program();
int program_1();
int external_declaration();
int decl_or_stmt();
int declarator_list();
int declarator_list_1();
int intstr_list();
int intstr_list_1();
int initializer();
int declarator();
int parameter_list();
int parameter_list_1();
int parameter();
int type();
int statement();
int statement_list();
int statement_list_1();
int expression_statement();
int expr();
int cmp_expr();
int cmp_expr_1();
int add_expr();
int add_expr_1();
int mul_expr();
int mul_expr_1();
int primary_expr();
int expr_list();
int expr_list_1();
int id_list();
int id_list_1();

int main(int argc, char **argv)
{
	yyin = fopen("test.c", "r");
	advance();
	int r = program();
	if (r == 1)
		printf("True!\n\n");
	else
		printf("Fault!\n\n");
	system("pause");
	return 0;
}

void advance() {
	tok = yylex();
	printf("%s\n", yytext);
}
//program
//	: external_declaration
//	| program external_declaration
//	;
int program() {
	if (external_declaration())
		if (program_1())
			return 1;
	return 0;
}
int program_1() {
	if (external_declaration()) {
		if (program_1())
			return 1;
	}
	else return 1;
	return 0;
}
//external_declaration
//	: type declarator decl_or_stmt
//	;
int external_declaration() {
	if (type())
		if (declarator())
			if (decl_or_stmt())
				return 1;
	return 0;
}

//decl_or_stmt
//	: '{' statement_list '}'
//	| '{' '}'
//	| ',' declarator_list ';'
//	| ';'
//	;
int decl_or_stmt() {
	if (tok == '{') {
		advance();
		if (tok == '}') {
			advance();
			return 1;
		}
	}
	else if (tok == ',') {
		advance();
		if (declarator_list())
			if (tok == ';') {
				advance();
				return 1;
			}
	}
	else if (tok == ';') {
		advance();
		return 1;
	}
	return 0;
}

//declarator_list
//	: declarator
//	| declarator_list ',' declarator
//	;
int declarator_list() {
	if (declarator())
		if (declarator_list_1())
			return 1;
	return 0;
}
int declarator_list_1() {
	if (tok == ',') {
		advance();
		if (declarator())
			if (declarator_list_1())
				return 1;
	}
	else return 1;
	return 0;
}

//intstr_list
//	: initializer
//	| intstr_list ',' initializer
//	;
int intstr_list() {
	if (initializer())
		if (intstr_list_1())
			return 1;
	return 0;
}
int intstr_list_1() {
	if (tok == ',') {
		advance();
		if (initializer())
			if (intstr_list_1())
				return 1;
	}
	else return 1;
	return 0;
}

//initializer
//	: NUMBER
//	| STRING
//	;
int initializer() {
	if (tok == NUMBER) {
		advance();
		return 1;
	}
	else if (tok == STRING) {
		advance();
		return 1;
	}
	return 0;
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
int declarator() {
	if (tok == ID) {
		advance();
		if (tok == '=') {
			advance();
			if (expr())
				return 1;
		}
		else if (tok == '(') {
			advance();
			if (parameter_list())
				;
			if (tok == ')') {
				advance();
				return 1;
			}
		}
		else if (tok == '[') {
			advance();
			if (expr())
				;
			if (tok == ']') {
				advance();
				if (tok == '=') {
					advance();
					if (tok == '{') {
						advance();
						if (intstr_list())
							if (tok == '}') {
								advance();
								return 1;
							}
					}
					return 0;
				}
				return 1;
			}
		}
		return 1;
	}
	return 0;
}

//parameter_list
//	: parameter
//	| parameter_list ',' parameter
//	;
int parameter_list() {
	if (parameter())
		if (parameter_list_1())
			return 1;
	return 0;
}
int parameter_list_1() {
	if (tok == ',') {
		advance();
		if (parameter())
			if (parameter_list_1())
				return 1;
	}
	else return 1;
	return 0;
}
//parameter
//	: type ID
//	;
int parameter() {
	if (type())
		if (tok == ID) {
			advance();
			return 1;
		}
	return 0;
}
//type
//	: INT
//	| STR
//	| VOID
//	;
int type() {
	if (tok == INT) {
		advance();
		return 1;
	}
	else if (tok == STR) {
		advance();
		return 1;
	}
	else if (tok == VOID) {
		advance();
		return 1;
	}
	return 0;
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
int statement() {
	if (type()) {
		if (declarator_list())
			if (tok == ';') {
				advance();
				return 1;
			}
	}
	else if (tok == '{') {
		advance();
		if (statement_list())
			if (tok == '}') {
				advance();
				return 1;
			}
	}
	else if (expression_statement())
		return 1;
	else if (tok == IF) {
		advance();
		if (tok == '(') {
			advance();
			if (expr())
				if (tok == ')') {
					advance();
					if (statement()) {
						if (tok == ELSE) {
							advance();
							if (statement())
								return 1;
						}
						else return 1;
					}
				}
		}
	}
	else if (tok == WHILE) {
		advance();
		if (tok == '(') {
			advance();
			if (expr())
				if (tok == ')') {
					advance();
					if (statement())
						return 1;
				}
		}
	}
	else if (tok == RETURN) {
		advance();
		if (expr())
			;
		if (tok == ';') {
			advance();
			return 1;
		}
	}
	else if (tok == PRINT) {
		advance();
		if (expr_list())
			;
		if (tok == ';') {
			advance();
			return 1;
		}
	}
	else if (tok == SCAN) {
		advance();
		if (id_list())
			if (tok == ';') {
				advance();
				return 1;
			}
	}
	return 0;
}
//statement_list
//	: statement
//	| statement_list statement
//	;
int statement_list() {
	if (statement())
		if (statement_list_1())
			return 1;
	return 0;
}
int statement_list_1() {
	if (statement()) {
		if (statement_list_1())
			return 1;
	}
	else return 1;
	return 0;
}
//expression_statement
//	: ';'
//	| expr ';'
//	;
int expression_statement() {
	if (tok == ';') {
		advance();
		return 1;
	}
	else if (expr())
		if (tok == ';') {
			advance();
			return 1;
		}
	return 0;
}
//expr
//	: cmp_expr
//	;
int expr() {
	if (cmp_expr())
		return 1;
	return 0;
}
//cmp_expr
//	: add_expr
//	| cmp_expr CMP add_expr
//	;
int cmp_expr() {
	if (add_expr())
		if (cmp_expr_1())
			return 1;
	return 0;
}
int cmp_expr_1() {
	if (tok == CMP) {
		advance();
		if (add_expr())
			if (cmp_expr_1())
				return 1;
	}
	else return 1;
	return 0;
}
//add_expr
//	: mul_expr
//	| add_expr '+' mul_expr
//	| add_expr '-' mul_expr
//	;
int add_expr() {
	if (mul_expr())
		if (add_expr_1())
			return 1;
	return 0;
}
int add_expr_1() {
	if (tok == '+' || tok == '-') {
		advance();
		if (mul_expr())
			if (add_expr_1())
				return 1;
	}
	else return 1;
	return 0;
}
//mul_expr
//	: primary_expr
//	| mul_expr '*' primary_expr
//	| mul_expr '/' primary_expr
//	| mul_expr '%' primary_expr
//	| '-' primary_expr
//	;
int mul_expr() {
	if (tok == '-')
		advance();
	if (primary_expr())
		if (mul_expr_1())
			return 1;
	return 0;
}
int mul_expr_1() {
	if (tok == '*' || tok == '/' || tok == '%') {
		advance();
		if (primary_expr())
			if (mul_expr_1())
				return 1;
	}
	else return 1;
	return 0;
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
int primary_expr() {
	if (tok == ID) {
		advance();
		if (tok == '(') {
			advance();
			if (expr_list())
				;
			if (tok == ')') {
				advance();
				return 1;
			}
		}
		else if (tok == ASSIGN) {
			advance();
			if (expr())
				return 1;
		}
		else if (tok == '=') {
			advance();
			if (expr())
				return 1;
		}
		else if (tok == '[') {
			advance();
			if (expr())
				if (tok == ']') {
					advance();
					if (tok == '=') {
						advance();
						if (expr())
							return 1;
						else
							return 0;
					}
					return 1;
				}
		}
		else return 1;
	}
	else if (tok == '(') {
		advance();
		if (expr())
			if (tok == ')') {
				advance();
				return 1;
			}
	}
	else if (tok == NUMBER) {
		advance();
		return 1;
	}
	else if (tok == STRING) {
		advance();
		return 1;
	}
	return 0;
}
//expr_list
//	: expr
//	| expr_list ',' expr
//	;
int expr_list() {
	if (expr())
		if (expr_list_1())
			return 1;
	return 0;
}
int expr_list_1() {
	if (tok == ',') {
		advance();
		if (expr())
			if (expr_list_1())
				return 1;
	}
	else return 1;
	return 0;
}
//id_list
//	: ID
//	| id_list ',' ID
//	;
int id_list() {
	if (tok == ID) {
		advance();
		if (id_list_1())
			return 1;
	}
	return 0;
}
int id_list_1() {
	if (tok == ',') {
		advance();
		if (tok == ID) {
			advance();
			if (id_list_1())
				return 1;
		}
	}
	else return 1;
	return 0;
}
