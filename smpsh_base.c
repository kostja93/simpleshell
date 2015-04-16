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
 ************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "token.h"

struct args_struct {
    char *arg;
    struct args_struct *next;
};

struct command_struct {
    char *cmd;
    struct args_struct *args;
};

struct command_queue_struct {
    struct command_struct *cmd;
    struct command_queue_struct *next;
};

typedef struct args_struct* ArgList;
typedef struct command_struct* Command;
typedef struct command_queue_struct* CommandQueue;

void pushCommand(CommandQueue queue, Command cmd) {
    CommandQueue q = malloc(sizeof(struct command_queue_struct));

    CommandQueue tmpQ = queue;
    q->cmd = cmd;

    while (tmpQ->next != NULL) {
        tmpQ = tmpQ->next;
    }

    tmpQ->next = q;
}

Command pullCommand(CommandQueue queue) {
    Command cmd = queue->cmd;
    queue = queue->next;

    return cmd;
}

Command add_command(char *command) {
    Command newCommand = malloc(sizeof(struct command_struct));
    newCommand->cmd = command;
    return newCommand;
}

void append_arg(Command command, char *arg) {
    ArgList list = command->args;
    while (list->next != NULL) {
        list = list->next;
    }
    list->arg = arg;
}

int main(void) {
    char defaulttext[] = "smpsh > ";
    char word[LINEMAX], lastWord[LINEMAX];
    char *prompt = defaulttext;
    int goon;

    /**/
    CommandQueue commands = malloc(sizeof(struct command_queue_struct));
    int cmd = 0;

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
                    printf("T_QUOTE <%s> \n", word);
                    break;
                case T_WORD: {
                    /*
                    Command newCommand = add_command( word );
                    pushCommand(commands, newCommand);
                     */
                    if (strcmp(word, "myecho") == 0){
                        cmd = 1;
                        break;
                    }
                    if (strcmp(word, "exit") == 0){
                        cmd = 2;
                        break;
                    }
                    if (strcmp(word, "pwd") == 0){
                        cmd = 3;
                    }
                    if (strcmp(word, "cd") == 0){
                        cmd = 4;
                    }
                    if (strcmp(word, "kill") == 0){
                        cmd = 5;
                        break;
                    }

                    switch (cmd) {
                        case 1: {
                            printf("%s ", word);
                            break;
                        }
                        case 2: {
                            exit(atoi(word));
                            break;
                        }
                        case 3: {
                            char path[1024];
                            getcwd(path, sizeof(path));
                            printf("%s", path);
                            break;
                        }
                        case 4: {
                            chdir(word);
                            break;
                        }
                        case 5: {
                            kill(atoi(word), SIGKILL);
                            break;
                        }
                    }
                    break;
                };
                case T_BAR:
                    printf("T_BAR \n");
                    break;
                case T_AMP:
                    printf("T_AMP \n");
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
                    printf("\n");
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
        /*
        while (commands != NULL) {
            Command cmd = pullCommand(commands);
            /*
             * Execute Command here
             *
             *
        }*/

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


