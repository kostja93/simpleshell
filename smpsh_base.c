/***********************************************************************
 *
 * PROGRAMM:     smpsh_base.c
 *
 * BESCHREIBUNG: Programmgeruest zum parsen einer Kommandozeile mit lex
 *
 * VERSION:      lex/flex Version; Test der Funktion gettoken()
 *
 * AUTHOR:       R. Mentrup
 *
 * DATUM:        19.03.2014
 *
 * HINWEIS ZUR COMPILIERUNG:
 *      flex smpsh_scanner.l                  (erzeugt lex.yy.c)
 *      gcc -c lex.yy.c			      (erzeugt lex.yy.o)
 *      gcc -g -Wall -ansi -o smpsh_base smpsh_base.c lex.yy.o -lfl
 *      gcc -g -Wall -o smpsh_base smpsh_base.c helper.h helper.c commands.h commands.c lex.yy.o -lfl
 ************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "commands.h"
#include "helper.h"

int main(void) {
    char defaulttext[] = "smpsh > ";
    char word[LINEMAX];
    char *prompt = defaulttext;
    int goon;

    /*Declaration commands variables*/
    Command cmd = NULL;
    int amp     = 0;
    /*CommandQueue cmdQ = init_queue();*/


    /* Schleife ueber alle Eingabezeilen    */
    while (TRUE) {

        /*
         *   In dieser Schleife wird eine Kommandozeile gelesen und in
         *   einzelne Zeichenketten (token) zerlegt. Fuer die Rueckgabewerte
         *   T_WORD und T_QUOTE zeigt word auf die entsprechenden Zeichenketten.
         */

        /*
         *   Prompt ausgeben......
         */
        printf("%s", prompt);
        fflush(stdout);

        /*
         *   Lesen und Zerlegen der Eingabezeile.
         */

        goon = TRUE;
        while (goon) {

            /*
             *
             *      An Stelle der printf()-Befehle sind nachfolgend die
             *      Datenstrukturen fuer die Systemaufrufe aufzubauen.
             *
             */
            switch (gettoken(word, LINEMAX)) {
                case T_QUOTE:
                    strcpy(word, trimQuote(word, sizeof(word)));
                    break;
                case T_WORD: {
                    if (cmd == NULL) {
                        cmd = add_command(word);
                    } else {
                        append_arg(cmd, word);
                    }
                    break;
                };
                case T_BAR:
                    printf("T_BAR \n");
                    break;
                case T_AMP:
                    amp = 1;
                    break;
                case T_SEMI:
                    printf("T_SEMI \n");
                    break;
                case T_GT:
                    printf("T_GT \n");
                    break;
                case T_GTGT:
                    printf("T_GTGT \n");
                    break;
                case T_MISSQUOTE:
                    printf("T_MISSQUOTE\n");
                    break;
                case T_LT:
                    printf("T_LT \n");
                    break;
                case T_NL:
                    goon = FALSE;
                    break;
                case T_NULL:
                    printf("T_NULL \n");
                    goon = FALSE;
                    break;
                case T_EOF:          /* wird von yylex nicht erkannt */
                    printf("T_EOF \n");
                    break;
                default:
                    printf("unknown token %s\n", word);
                    exit(1);
            }
        }

        execute_commandp(cmd, amp);
        cmd = NULL;

        /*
         *
         * Hier sind die Systemroutinen aufzurufen, die die eigentliche
         * Arbeit erledigen.
         *
         *
         *
         */


        /* Eine Kommandozeile wurde durch die Shell abgearbeitet        */
    }

    return 0;
}


