#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define DEBUG 1
#define DIGIT 1
#define ALPHA 2
#define UNDERLINE 3
#define UNKNOWN 4
#define MAX_LEN 100

char lex_str[MAX_LEN+1];
int lex_idx = 0;
FILE* ana_file;
int is_incomm_line = 0;
int is_incomm_block = 0;
int charClass = 0;
int nextToken = 0;
char nextchar = 0, nextchar2 = 0;
char* keywords[] = {"int", "void", "const", "if", "else", "while", "break", "continue", "return", 0};
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
	tok_SEMICOLON	 	 // ;
};

int isunderline(char c){
	if(c == '_')
		return 1;
	else
		return 0;
}

int iscomment(char c1, char c2){
	if(c1 == '/' && c2 == '/' && is_incomm_line == 0 && is_incomm_block == 0){
		is_incomm_line = 1;
	}
	else if(c1 == '/' &&  c2 == '*'){
		is_incomm_block = 1;
	}
	else if(c2 == '\n' && is_incomm_line == 1){
		getChar();
		getChar();
		is_incomm_line = 0;
	}
	else if(c1 == '*' &&  c2 == '/'){
		getChar();
		getChar();
		getChar();
		is_incomm_block = 0;
	}
	
	return (is_incomm_block || is_incomm_line);
}


void addChar(void){
	if(lex_idx >= MAX_LEN - 1){
		printf("要分析的文件太长了\n");
		exit(1);
	}

	lex_str[lex_idx] = nextchar;
	lex_idx++;
	lex_str[lex_idx] = '\0';
}

void getChar(void){
	static int first_get = 1;

	if(first_get == 1){
		nextchar = fgetc(ana_file);
		nextchar2 = fgetc(ana_file);
		first_get = 0;

		if(nextchar == EOF){
			nextToken = EOF;
			return ;
		}
	}
	else{
		nextchar = nextchar2;
		nextchar2 = fgetc(ana_file);
	}

	if(nextchar == EOF)
		charClass = EOF;
	else if(isdigit(nextchar) != 0)
		charClass = DIGIT;
	else if(isalpha(nextchar) != 0)
		charClass = ALPHA;
	else if(isunderline(nextchar) != 0)
		charClass = UNDERLINE;
	else
		charClass = UNKNOWN;
}

void checkKeywords(char str[]){
	int i = 0;

	while(keywords[i] != 0){
		if(strcmp(str, keywords[i]) == 0)
			nextToken = 258 + i;
		i++;
	}
}

void checkSymbol(char c1, char c2){
	switch (c1)
	{
	case ';':
		addChar();
		nextToken = tok_SEMICOLON;
		break;
	case '(':
		addChar();
		nextToken = tok_LPAR;
		break;
	case ')':
		addChar();
		nextToken = tok_RPAR;
		break;
	case '[':
		addChar();
		nextToken = tok_LSQUARE;
		break;
	case ']':
		addChar();
		nextToken = tok_RSQUARE;
		break;
	case '{':
		addChar();
		nextToken = tok_LBRACKET;
		break;
	case '}':
		addChar();
		nextToken = tok_RBRACKET;
		break;
	case '*':
		addChar();
		nextToken = tok_MUL;
		break;
	case '+': 
		addChar();
		nextToken = tok_ADD;
		break;
	case ',':
		addChar();
		nextToken = tok_COMMA;
		break;
	case '%':
		addChar();
		nextToken = tok_MODULO;
		break;
	case '/':
		addChar();
		nextToken = tok_DIV;
		break;
	case '-':
		addChar();
		nextToken = tok_SUB;
		break;
	case '<':
		if(c2 == '='){
			nextToken = tok_LESSEQ;
			addChar();
			getChar();
			addChar();
		}
		else{
			nextToken = tok_LESS;
			addChar();
		}
		break;
	case '=':
		if(c2 == '='){
			nextToken = tok_EQ;
			addChar();
			getChar();
			addChar();
		}
		else{
			nextToken = tok_ASSIGN;
			addChar();
		}
		break;
	case '>':
		if(c2 == '='){
			nextToken = tok_GREATEQ;
			addChar();
			getChar();
			addChar();
		}
		else{
			nextToken = tok_GREAT;
			addChar();
		}
		break;
	case '!':
		if(c2 == '='){
			nextToken = tok_NOTEQ;
			addChar();
			getChar();
			addChar();
		}
		else{
			nextToken = UNKNOWN;
			addChar();
		}
		break;
	case '&':
		if(c2 == '&'){
			nextToken = tok_AND;
			addChar();
			getChar();
			addChar();
		}
		else{
			nextToken = UNKNOWN;
			addChar();
		}
		break;
	case '|':
		if(c2 == '|'){
			nextToken = tok_OR;
			addChar();
			getChar();
			addChar();
		}
		else{
			nextToken = UNKNOWN;
			addChar();
		}
		break;
	case '~':
		nextToken = tok_NOT;
		addChar();
		break;
	default:break;
	} 
}

void getNonBlank(void){
	while(isspace(nextchar) != 0 || iscomment(nextchar, nextchar2) != 0)
		getChar();
}

int lex_ana(void){
	getNonBlank();
	
	switch (charClass)
	{
	case DIGIT:
		do{
			addChar();
			getChar();
		}while(charClass == DIGIT || charClass == ALPHA);
		nextToken = tok_INTEGER;
		break;
	case ALPHA:
		do{
			addChar();
			getChar();
		}while(charClass == DIGIT || charClass == ALPHA || charClass == UNDERLINE);
		nextToken = tok_ID;
		checkKeywords(lex_str);
		break;
	case UNDERLINE:
		do{
			addChar();
			getChar();
		}while(charClass == DIGIT || charClass == ALPHA || charClass == UNDERLINE);
		break;
	case UNKNOWN:
		checkSymbol(nextchar, nextchar2);
		getChar();
		break;
	case EOF:
		nextToken = EOF;
		break;
	default:
		break;
	}

	if(nextToken >= 258 && nextToken <= 289)
		printf("%d\n", nextToken);
		//printf("%c\n", nextToken);
	else if(nextToken == 256 || nextToken == 257)
		//printf("%d %s\n", nextToken, lex_str);
		printf("%d %s\n", nextToken, lex_str);

	lex_idx = 0;
	lex_str[0] = '\0';
	return nextToken;
}

int main(int argc, char* argv[]){
	argc--; argv++;
	if(argc < 1){
		printf("请输入要分析的文件\n");
		exit(1);
	}

	ana_file = fopen(*argv, "r");

	if(ana_file == NULL){
		printf("要分析的文件不存在\n");
		exit(1);
	}
	getChar();

	while (nextToken != EOF)
		lex_ana();

	// printf("EOF\n");

	if(fclose(ana_file) == EOF){
		printf("关闭文件错误\n");
		exit(1);
	}
}
