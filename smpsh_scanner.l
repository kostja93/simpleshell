/* smpsh_scanner.l -- lex Beschreibung des Scanners
 *
 * Diese Datei beschreibt, wie ein Zeichenstrom aus
 * der Eingabezeile zerlegt werden soll. Das Werkzeug
 * lex (bzw. GNU flex) erzeugt aus dieser Beschreibung eine
 * C-Routine,yylex(), die diese Zerlegung vornimmt. 
 *
 * Literatur: Herold, lex & yacc (fuer praktische Beispiele)
 *
 * Last Update 3/2014 men
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

/* buffer extra klein zum testen !! */
#undef YYLMAX
#define YYLMAX LINEMAX

/* %array */ 
/*   `%array' defines yytext to be an array of YYLMAX characters, which defaults to a 
   fairly large value. You can change the size by simply #define YYLMAX to a 
   different value in the first section of your flex input. 
   yyin = stdin; 
   yy_create_buffer( yyin, YY_BUF_SIZE ) );
*/

%}

%array

yyword             ([a-zA-Z_0-9\:\+\-\?\%\$\.\*\(\)\{\}\=/\,\!\#�������]+)
yyquot             (\"[^\"\n]*\")

%%
[ \t]+             ; /* ignore whitespace */
{yyword}           return( T_WORD );
{yyquot}           return( T_QUOTE );
"|"                return( T_BAR );
"&"                return( T_AMP );
";"                return( T_SEMI );
">"                return( T_GT );
">>"               return( T_GTGT );
"<"                return( T_LT );
\"                 return( T_MISSQUOTE );
\r                 return( T_EOF );
\n                 return( T_NL );
\0                 return( T_NULL );
.                  return( T_UNKNOWN);  /* unidentified stuff */  
%%

int gettoken( char *w, unsigned int len)
{
  int t;

  t = yylex();  /* diese Routine wird von lex (bzw. flex) generiert */

  /* yytext = zeiger auf den zur�ckgegebenen Text */

  switch( t ){
  case T_QUOTE:
    strncpy( w, yytext, len );
    break;
  case T_WORD:
    strncpy( w, yytext, len );
    break;
  case T_UNKNOWN: 
    strncpy( w, yytext, len );
    break;
  default:
    w[0] = 0;  /* alten text loeschen */
  }
  
  return t;
}

