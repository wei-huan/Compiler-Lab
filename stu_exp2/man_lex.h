#ifndef MAN_LEX_H
#define MAN_LEX_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#define DEBUG 1
#define DIGIT 1
#define ALPHA 2
#define UNDERLINE 3
#define UNKNOWN 4
#define MAX_LEN 100

enum Token_T{
	tok_ID=256,		 // 标识符
	tok_INTEGER,	 	 // 整数
	tok_INT,		 // int
	tok_VOID,		 // void
	tok_CONST,		 // const
	tok_IF,			 // if
	tok_ELSE,		 // else
	tok_WHILE,		 // while
	tok_BREAK,		 // break
	tok_CONTINUE,	 	 // continue
	tok_RETURN,		 // return
	tok_ADD,		 // +
	tok_SUB,		 // -
	tok_MUL,		 // *
	tok_DIV,		 // /
	tok_MODULO,		 // %
	tok_LESS,		 // <
	tok_LESSEQ,		 // <=
	tok_GREAT,		 // >
	tok_GREATEQ,	 	 // >=
	tok_NOTEQ,		 // !=
	tok_EQ,		 	 // ==
	tok_NOT,		 // !
	tok_AND, 		 // &&
	tok_OR,			 // ||
	tok_ASSIGN,		 // =
	tok_LPAR,		 // (
	tok_RPAR,		 // )
	tok_LBRACKET,	 	 // {
	tok_RBRACKET,	 	 // }
	tok_LSQUARE,	 	 // [
	tok_RSQUARE,	 	 // ]
	tok_COMMA,		 // ,
	tok_SEMICOLON,	 	 // ;
	tok_EOF
};

extern FILE* ana_file;
extern int nextToken;
extern int lex_idx;
extern char lex_str[MAX_LEN + 1];
extern int first_get;
extern int last_get;

int isunderline(char c);
int iscomment(char c1, char c2);
void addChar(void);
void getChar(void);
void checkKeywords(char str[]);
void checkSymbol(char c1, char c2);
void getNonBlank(void);
int lex_ana(void);

#endif /*MAN_LEX_H*/
