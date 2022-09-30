#include "man_lex.h"

FILE* ana_file;
int lex_idx = 0;
int is_incomm_line = 0;
int is_incomm_block = 0;
int charClass = 0;
int nextToken = 0;

char lex_str[MAX_LEN + 1] = {0};
char nextchar = 0, nextchar2 = 0;
char* keywords[] = {"int", "void", "const", "if", "else", "while", "break", "continue", "return", 0};

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

// int is_end = 0;
int first_get = 1;
int last_get = 0;
void getChar(void){

	if(first_get == 1){
		nextchar = fgetc(ana_file);
		// printf("nextchar: %c\n", nextchar);
		nextchar2 = fgetc(ana_file);
		// printf("nextchar2: %c\n", nextchar2);

		first_get = 0;
	}
	else{
		nextchar = nextchar2;
		// printf("nextchar: %c\n", nextchar);
		nextchar2 = fgetc(ana_file);

		// printf("nextchar2: %c\n", nextchar2);
	}

	if(nextchar2 == EOF)
		last_get = 1;
	else
		last_get = 0;

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
	switch (c1){
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
	// if(feof(ana_file) && is_end == 0){
	// 	printf("getNonBlank_1\n");
	// 	is_end = 1;
	// 	getChar();
	// }
	// else if(feof(ana_file) && is_end == 1){
	// 	printf("getNonBlank_2\n");
	// 	charClass = EOF;
	// }
	// else{
	// 	printf("getNonBlank_3\n");
	// 	is_end = 0;
	// 	while((isspace(nextchar) || iscomment(nextchar, nextchar2)) && charClass != EOF)
	// 		getChar();
//	}

	while(iscomment(nextchar, nextchar2) || isspace(nextchar))
		getChar();
}

int lex_ana(void){
	getNonBlank();

	switch (charClass){
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
		nextToken = tok_ID;
		break;
	case UNKNOWN:
		checkSymbol(nextchar, nextchar2);
		getChar();
		break;
	case EOF:
		// printf("EOF\n");
		nextToken = tok_EOF;
		break;
	default:
		break;
	}

	// if(nextToken >= 256 && nextToken <= 289)
	// 	printf("tok:%d pos:%ld lex:%s\n", nextToken, ftell(ana_file), lex_str);

	lex_idx = 0;
	lex_str[0] = '\0';
	return nextToken;
}

// int main(int argc, char* argv[]){
// 	argc--; argv++;
// 	if(argc < 1){
// 		printf("请输入要分析的文件\n");
// 		exit(1);
// 	}

// 	ana_file = fopen(*argv, "r");

// 	if(ana_file == NULL){
// 		printf("要分析的文件不存在\n");
// 		exit(1);
// 	}

// 	getChar();
// 	while (nextToken != tok_EOF)
// 		lex_ana();

// 	// printf("EOF\n");

// 	if(fclose(ana_file) == EOF){
// 		printf("关闭文件错误\n");
// 		exit(1);
// 	}
// }
