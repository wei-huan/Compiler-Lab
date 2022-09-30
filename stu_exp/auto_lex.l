%{
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
int yyval;
%}
OCTDIGIT    [0-7]
DIGIT	    [0-9]
ALPHA	    [a-zA-Z]
HEXALPHA    [a-fA-F]
HEXNUM	    0(x|X)({DIGIT}|{HEXALPHA})+
DECNUM	    {DIGIT}+
OCTNUM      0(OCTDIGIT)+
AND	    \&{2}
OR	    \|{2}
WHITESPACE  [ \t\n]+
ID	    (_|{ALPHA})({ALPHA}|{DIGIT}|_)*
COMMENT_LINE (\/{2}(.)*\n)
COMMENT_BLOCK "/*"([^\*]|(\*)+[^/])*"*/" 

%%
"+"  {return tok_ADD;}
"-"  {return tok_SUB;}
"*"  {return tok_MUL;}
"%"  {return tok_MODULO;}
"/"  {return tok_DIV;}
"="  {return tok_ASSIGN;}
"==" {return tok_EQ;}
">=" {return tok_GREATEQ;}
"<=" {return tok_LESSEQ;}
"!=" {return tok_NOTEQ;}
">"  {return tok_GREAT;}
"<"  {return tok_LESS;}
"&&" {return tok_AND;}
"||" {return tok_OR;}
"{"  {return tok_LBRACKET;}
"}"  {return tok_RBRACKET;}
"["  {return tok_LSQUARE;}
"]"  {return tok_RSQUARE;}
"("  {return tok_LPAR;}
")"  {return tok_RPAR;}
";"  {return tok_SEMICOLON;}
","  {return tok_COMMA;}
"if" {return tok_IF;}
"while" {return tok_WHILE;}
"else" {return tok_ELSE;}
"break" {return tok_BREAK;}
"return" {return tok_SEMICOLON;}
"void" {return tok_VOID;}
"continue" {return tok_CONTINUE;}
"int" {return tok_INT;}
"const" {return tok_CONST;}
{HEXNUM}    {yyval = strtol(yytext, NULL, 16); return tok_INTEGER;}
{DECNUM}    {yyval = strtol(yytext, NULL, 10); return tok_INTEGER;}
{OCTNUM}    {yyval = strtol(yytext, NULL, 8); return tok_INTEGER;}
{WHITESPACE} {;}
{COMMENT_LINE} {;}
{COMMENT_BLOCK} {;}
{ID}	    {return tok_ID;}
%%

int yywrap(void){
	return -1;
}

int main(int argc, char** argv){
	int yytype;

	++argv, --argc;
	if(argc > 0)
		yyin = fopen(argv[0], "r");
	else{
		printf("请输入要分析的文件");
		exit(1);
	}

	while(yytype = yylex()){
		if(yytype >= tok_INT && yytype <= tok_SEMICOLON)
			printf("%d\n", yytype);
		else if(yytype >= tok_ID && yytype <= tok_INTEGER)
			printf("%d %s\n", yytype, yytext);
		else
			printf("出错啦, 未知符号\n");
	}

	return 0;
}
