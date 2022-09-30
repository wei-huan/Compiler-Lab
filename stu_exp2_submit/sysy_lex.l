%{
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

int yylval;

%}

NOTE [^\n]*
STRING [^"]*

%%

"//"{NOTE}				{ /* ignore note */ }
["][^"]*["]				{ yylval = atoi(yytext); return STRING; }

"int"					{ yylval = atoi(yytext); return INT; }
"str"					{ yylval = atoi(yytext); return STR; }
"void"					{ yylval = atoi(yytext); return VOID; }
"if"					{ yylval = atoi(yytext); return IF; }
"else"					{ yylval = atoi(yytext); return ELSE; }
"while"					{ yylval = atoi(yytext); return WHILE; }
"return"				{ yylval = atoi(yytext); return RETURN; }
"print"					{ yylval = atoi(yytext); return PRINT; }
"scan"					{ yylval = atoi(yytext); return SCAN; }
"assign"				{ yylval = atoi(yytext); return ASSIGN; }

"+"|"-"|"*"|"/"|"%"|"="	{ return yytext[0]; }
"("|")"|"["|"]"|"{"|"}"	{ return yytext[0]; }
","|";"					{ return yytext[0]; }

">"|"<"|"<="|">="|"=="	{ yylval = atoi(yytext); return CMP; }
[a-z][a-zA-Z0-9|_]*		{ yylval = atoi(yytext); return ID; }
[0-9]+ 					{ yylval = atoi(yytext); return NUMBER; }

[ \t] 					{ /* ignore whitespace */ }
. 						{ printf("Mystery character %c\n", *yytext); }

%%

int yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}

int yywrap()
{
	return 1;
}
