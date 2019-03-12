%{
#include "Globals.h"

char tokenString[MAXTOKENLEN+1];
%}

DIGITO		[0-9]
ID		    [a-zA-Z0-9]
LETRA 		[a-zA-Z]
COMENTARIO	[^*/]*
WHITESPACE	[ \t]+
SYMBOLERROR [.]

%%
"\n"													{lineno++;}
{WHITESPACE}+                 {}
"/*"                          { char c;
                                while(1)
                                { c = input();
                                  if (c == EOF) break;
                                  if (c == '\n') lineno++;
                                  if (c == '*')
                                  {
                                    c = input();
                                    if (c == '/') break;
                                  }
                                }
                              }
else			        						{return ELSE;}
if				            				{return IF;}
int				            				{return INT;}
return			         					{return RETURN;}
void				        					{return VOID;}
while			        						{return WHILE;}
"+"				            				{return PLUS;}
"-"				            				{return MINUS;}
"*"				            				{return STAR;}
"/"				           					{return BAR;}
"<"				            				{return SLT;}
"<="				       						{return SLTE;}
">"				            				{return SBT;}
">="				        					{return SBTE;}
"=="				        					{return EQUAL;}
"!="				        					{return DIFF;}
"="				            				{return ATRIB;}
";"				            				{return POINTVIG;}
"("				            				{return OPPAR;}
")"				            				{return CLPAR;}
","				            				{return VIG;}
"["								            {return OPARR;}
"]"				            				{return CLARR;}
"{"				 		  	           	{return OPCHA;}
"}"				      	      			{return CLCHA;}
{SYMBOLERROR}                 {return ERRO;}
{LETRA}({LETRA}|{DIGITO})*		{return ID;}
{DIGITO}+			        				{return NUM;}
%%

int yywrap();
int yywrap(){ return 1; }

TokenType getToken(void)
{
      static int firstTime = TRUE;
      TokenType currentToken;
      if (firstTime)
      {
					firstTime = FALSE;
        	lineno++;
        	yyin = source;
        	yyout = listing;
      }
      currentToken = yylex();
      strncpy(tokenString,yytext,MAXTOKENLEN);
      if (Lexical)
			{
        fprintf(listing,"\t%d: ",lineno);
        printToken(currentToken,tokenString);
      }
      return currentToken;
}
