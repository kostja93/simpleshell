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
    int wrong_input = 0;
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
                    if ( (strlen(word) > 0) && (NULL != cmd) ) {
                        append_arg(cmd, word);
                    }
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
                    /*
                     * In Queue Speichern
                     * */
                    break;
                case T_AMP:
                    amp = 1;
                    break;
                case T_NL:
                    goon = FALSE;
                    break;
                case T_NULL:
                    goon = FALSE;
                    break;
                default:
                    wrong_input = 1;
                    break;
            }
        }
        if (cmd != NULL && wrong_input != 1) {
            execute_commandp(cmd, amp);
            free_cmd(cmd);
            cmd = NULL;
        }
        if (wrong_input == 1 && strlen(word) >= 1){
            printf("Unknown command or wrong usage! \n");
            wrong_input = 0;
        }

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


