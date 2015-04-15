/* token.h  -- defines token values returned by yylex and used
               inside smpsh's main function
*/

#ifndef _TOKEN_H
#define _TOKEN_H

#define T_QUOTE  1
#define T_WORD   2
#define T_BAR    3
#define T_AMP    4
#define T_SEMI   5
#define T_GT     6
#define T_GTGT   7
#define T_LT     8
#define T_NL     9
#define T_EOF   10   /* unused */
#define T_NULL  11
#define T_MISSQUOTE 12
#define T_UNKNOWN 13


#define TRUE         1
#define FALSE        0

#define LINEMAX  (20 + 5)  /* max. wortlaenge zum testen (YYLMAX) */


extern int yylex();

int gettoken (char *word, unsigned int len);

#endif
