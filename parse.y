/* parse.y
 * $ BISON -d parse.y   (generates parse.tab.c, parse.tab.h)
 * "parse.tab.h" should be included in the C code generated from "parse.lex".
 *
 * (c) Guoyong Shi, SJTU, 10-08-2007, 01-04-2015

	This parser is to be used with MATLAB mex.
	Run in MATLAB by: mex_parse('netlist.sp')
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_func.h"

void yyerror(char *msg);
char* alloc_str(char *str);
%}

%union
{
	double f;
	int n;
	char* s;
};

/* Redefining data types for the token names */

%token<f> FLOAT
%token<s> STRING
%token<n> INTEGER
%token<s> RESISTOR CAPACITOR INDUCTOR CURRSRC VOLTSRC
%token<s> VCCS
%token<f> VALUE
%token END EOL 

%type<s> resistor capacitor inductor currsrc voltsrc node variable
%type<s> vccs
%type<f> value

%start cktfile

%%

cktfile: netlist end
	   {
	   }
;

end: end EOL
	| END EOL
;

netlist: netlist line
       | line
;

line: component EOL
	| error EOL
	| EOL
;

component: resistor
		 | capacitor
		 | inductor
		 | currsrc
		 | voltsrc
		 | vccs
;

resistor: RESISTOR node node value
		{
			ParseRes($1, $2, $3, $4);
		}
;

capacitor: CAPACITOR node node value					
		{
			ParseCap($1, $2, $3, $4);
		}
		| CAPACITOR node node value value
		{
			ParseCapIC($1, $2, $3, $4, $5);
		}
;		
	
inductor: INDUCTOR node node value  				
		{
			ParseInd($1, $2, $3, $4);
		}
		| INDUCTOR node node value value
		{
			ParseIndIC($1, $2, $3, $4, $5);
		}
;
voltsrc: VOLTSRC node node value						
		{
			ParseVsrc($1, $2, $3, $4);
		}
;
currsrc: CURRSRC node node value						
		{
			ParseIsrc($1, $2, $3, $4);
		}
;
vccs: VCCS node node node node value
		{
			ParseVCCS($1, $2, $3, $4, $5, $6);
		}
;		

node: variable										
		{
			$$ = alloc_str($1);
		}

		| INTEGER
		{
			char s[33];
			sprintf(s, "%d", $1);
			$$ = alloc_str(s);
		}
;

variable: STRING			{ $$ = alloc_str($1); }
		| RESISTOR		{ $$ = alloc_str($1); }
		| CAPACITOR		{ $$ = alloc_str($1); }
		| INDUCTOR 		{ $$ = alloc_str($1); }
		| CURRSRC		{ $$ = alloc_str($1); }
		| VOLTSRC		{ $$ = alloc_str($1); }
;

value: VALUE			{$$ = $1;}
		| FLOAT			{$$ = $1;}
		| INTEGER		{$$ = $1;}
;

%%

/* Provide yyerror(), isatty() to avoid using "-lfl -ly" in linker.
   Can be placed in "parse.lex" as well. */
//--------------------------------------	
void yyerror(char *msg)
{
	fprintf(stderr, msg);
}

int isatty(int handle)
{
	return  0; 	/* regardless of returning 0 or 1 */
}
//--------------------------------------	

char* alloc_str(char *str)
{
	char *str_t;
	str_t = (char *) malloc((strlen(str)+1)*sizeof(char));
	strcpy(str_t, str); 
	str_t[strlen(str)] = '\0'; 
	return str_t;
}

#if 0

#include "parse.yy.c"

int main(int argc, char **args)
{
	FILE *inFile;

	if (argc < 2) {
		printf("Please provide an input file.\n");
		exit(0);
	}

	inFile = fopen(args[1], "r");
	if (!inFile) {
		printf("Did not find the input file \"%s\".\n", args[1]);
		exit(0);
	}
	yyin = inFile;

	yyparse();

	fclose(inFile);

	return (0);
}
#endif

