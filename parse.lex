/* 	parse.lex  
	2003-2015
	Guoyong Shi (c)
 
 * Run at command line:
 * $ flex -oparse.yy.c parse.lex 
   (=> generates "parse.yy.c" which is included
    in the C code generated from parse.y)
 *
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.tab.h"
%}

%option nounistd 

ALPHA			[A-Za-z_]
DIGIT			[0-9]
ALPHANUM		[A-Za-z_0-9]
STRING			{ALPHANUM}+
INTEGER			{DIGIT}+
FLOAT			[\-]?{DIGIT}+"."{DIGIT}+([Ee][\+\-]?{DIGIT}+)?
RESISTOR		[Rr]{ALPHANUM}+
CAPACITOR		[Cc]{ALPHANUM}+
INDUCTOR		[Ll]{ALPHANUM}+
CURRSRC			[Ii]{ALPHANUM}+
VOLTSRC			[Vv]{ALPHANUM}+
VCCS			[Gg]{ALPHANUM}+
EOL				[\n]
DELIMITER		[ \t]+
UNIT			[Ff]|[Pp]|[Nn]|[Uu]|[Mm]|[Kk]|[Mm][Ee][Gg]|[Gg]|[Tt]
VALUE			({FLOAT}|[-]?{INTEGER}){UNIT}
COMMENT			[\*][^\n]+
END				[\.][Ee][Nn][Dd]

%%

{INTEGER}		{yylval.n = atoi(yytext); return INTEGER;}
{FLOAT}			{yylval.f = atof(yytext); return FLOAT;}
{RESISTOR}		{yylval.s = (char*)malloc((strlen(yytext) + 1) * sizeof(char)); 
				 strcpy(yylval.s, yytext);
				 yylval.s[strlen(yytext)] = '\0';
                 		return RESISTOR;
				}
{CAPACITOR}		{yylval.s = (char*)malloc((strlen(yytext) + 1) * sizeof(char)); 
				 strcpy(yylval.s, yytext);
				 yylval.s[strlen(yytext)] = '\0';
                 		return CAPACITOR;
				}
{INDUCTOR}		{yylval.s = (char*)malloc((strlen(yytext) + 1) * sizeof(char)); 
				 strcpy(yylval.s, yytext);
				 yylval.s[strlen(yytext)] = '\0';
                 		return INDUCTOR;
				}
{CURRSRC}		{yylval.s = (char*)malloc((strlen(yytext) + 1) * sizeof(char)); 
				 strcpy(yylval.s, yytext);
				 yylval.s[strlen(yytext)] = '\0';
                 		return CURRSRC;				
				}
{VOLTSRC}		{yylval.s = (char*)malloc((strlen(yytext) + 1) * sizeof(char)); 
				 strcpy(yylval.s, yytext);
				 yylval.s[strlen(yytext)] = '\0';
                 		return VOLTSRC;
				}
{VCCS}			{yylval.s = (char*)malloc((strlen(yytext) + 1) * sizeof(char)); 
				 strcpy(yylval.s, yytext);
				 yylval.s[strlen(yytext)] = '\0';
                 		return VCCS;
				}
{VALUE}			{double value; int len; char u;
				 value = atof(yytext);
				 len = strlen(yytext);
				 u = yytext[len - 1];
				 if (u == 'F' || u == 'f')
					yylval.f = value * 1e-15;
				else if (u == 'P' || u == 'p')
					yylval.f = value * 1e-12;
				 else if (u == 'N' || u == 'n')
					yylval.f = value * 1e-9;
				 else if (u == 'U' || u == 'u')
					yylval.f = value * 1e-6;
				 else if (u == 'M' || u == 'm')
					yylval.f = value * 1e-3;
				 else if (u == 'K' || u == 'k')
					yylval.f = value * 1e3;
				 else if (u == 'G' || u == 'g')
			     {
					if (yytext[len - 2] == 'E' | yytext[len - 2] == 'e')
						yylval.f = value * 1e6;
					else
						yylval.f = value * 1e9;
				 }
				 else if (u == 'T' | u == 't')
					yylval.f = value * 1e12;
				 else
					yylval.f = value;
				 return VALUE;
				}
{STRING}		{yylval.s = (char*)malloc((strlen(yytext) + 1) * sizeof(char)); 
				strcpy(yylval.s, yytext);
				yylval.s[strlen(yytext)] = '\0';
				return STRING;
				}
{EOL}			{return EOL;}
<<EOF>>			{ return EOF; /* yyterminate(); */}
{DELIMITER}		{ /* yylloc->step(); */}
{COMMENT}		{ /* yylloc->step(); */}
{END}			{return END;}

%%

/* Provide yywrap() to avoid using -lfl in linker.
   Also can be placed in the file "parse.y". */

//--------------------------------------	
int yywrap()
{
    return  1;
}
//--------------------------------------	


#if 0
main()
{
	yylex();
}
#endif

