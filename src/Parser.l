%option noyywrap nodefault yylineno
%{

#include "Draco2.hpp"
#include "Parser.hpp"
#include "Parser.tab.h" // the output of bison on .y
void yyerror (const Parser*, char*); // we need to forward declare these functions ,
int yyparse (Parser* parser); // don ’t worry about them

%}

%x COMMENTS

EXP ([Ee][-+]?[0-9]+)

%%

[\n\t ]*"/*"            {printf("Begin comments\n"); BEGIN(COMMENTS);}
<COMMENTS>"*/"[\n\t ]* {printf("End comments\n"); BEGIN(INITIAL);}
<COMMENTS>"*/" {printf("End comments2\n"); BEGIN(INITIAL);}
<COMMENTS>.|\n	{printf("CC");}

"//".*\n {}

"cone"		{yylval.shape_type = ST_CONE; return SHAPE;}
"cylinder"	{yylval.shape_type = ST_CYLINDER; return SHAPE;}
"plane"		{yylval.shape_type = ST_PLANE; return SHAPE;}
"sphere"	{yylval.shape_type = ST_SPHERE; return SHAPE;}
"torus"		{yylval.shape_type = ST_TORUS; return SHAPE;}
"triangle"	{yylval.shape_type = ST_TRIANGLE; return SHAPE;}


"camera"		{return CAMERA;}
"area_light" 	{yylval.light_type = LIGHT_SURFACE; return LIGHT;}

"union"			{yylval.csg_type = CSG_UNION; return CSG;}
"intersection" 	{yylval.csg_type = CSG_INTERSECTION; return CSG;}
"difference"	{yylval.csg_type = CSG_DIFFERENCE; return CSG;}
"merge" 		{yylval.csg_type = CSG_MERGE; return CSG;}

"DiffuseMaterial" {yylval.material_type = MATERIAL_DIFFUSE; return MATERIAL;}





"texture"		{return TEXTURE;}

\n*" "*"{"" "*\n*	{return LCB;}
\n?[ \t]*"}"[ \t]*\n*	{return RCB;}

"<" |
">" |
","			{return yytext[0];}


"-"?[0-9]+"."? |
"-"?[0-9]+"."[0-9]*{EXP}? |
"-"?"."[0-9]+{EXP}? { yylval.float_token = atof(yytext); return T_FLOAT; }


[a-zA-Z][a-zA-Z0-9\_]*		{strcpy(yylval.str, yytext); return T_STRING;}

^[ \t]*\n 		{}
\n 			{return EOL;}
[ \t]+		{} // do nothing on whitespace


%%
void yyerror (const Parser* p, char* str) { printf("ERROR : %s\n", str); exit(1);}