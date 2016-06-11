%{
/*
	Set Conjunto := { Ele1, Ele2, eneN };
	Sets;
	ShowSets;
	ShowSet Conjunto;
	Union Cn, ...;
	Intersect Cn, ...;
	SetUnion Dest, Cn, ...;
	SetIntersect Dest, Cn, ...;
	Dest<-Conjunto;
	Delete Conjunto;
*/

#include "project.tab.c"

%} 

%option noyywrap 
%option bison-bridge bison-locations

%%

Set						{ yylval->sval = strdup(yytext); return SET; }
Sets					{ yylval->sval = strdup(yytext); return SETS; }
ShowSets				{ yylval->sval = strdup(yytext); return SHOWSETS; }
ShowSet					{ yylval->sval = strdup(yytext); return SHOWSET; }
Union					{ yylval->sval = strdup(yytext); return UNION; }
Intersect				{ yylval->sval = strdup(yytext); return INTERSECT; }
SetUnion				{ yylval->sval = strdup(yytext); return SETUNION; }
SetIntersect			{ yylval->sval = strdup(yytext); return SETINTERSECT; }
Delete					{ yylval->sval = strdup(yytext); return DELETE; }
"\<\-"					{ yylval->sval = strdup(yytext); return OP_COPY; }
":="					{ yylval->sval = strdup(yytext); return OP_ASSIGN; }
"\{"					{ yylval->sval = strdup(yytext); return BRACE_OPEN; }
"\}"					{ yylval->sval = strdup(yytext); return BRACE_CLOSE; }
";"						{ yylval->sval = strdup(yytext); return EOS; }
","						{ yylval->sval = strdup(yytext); return COMMA; }
[a-zA-Z][a-zA-Z0-9]*	{ yylval->sval = strdup(yytext); return KEYWORD; }
\n						{ printf("\n"); }
" "|\t					{}
.					    { yylval->sval = strdup(yytext); return UNKNOWN; }

%%
