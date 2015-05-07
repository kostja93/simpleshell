#ifndef SMPSH_BASE_COMMANDS_H
#define SMPSH_BASE_COMMANDS_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define __EXTENSIONS__
#include <signal.h>

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

void push_command(CommandQueue queue, Command cmd);

Command pull_command(CommandQueue queue);

Command init_command(char *command);

void append_arg(Command command, char *arg);
int execute_command(Command command);
void execute_commandp(Command cmd, int amp);
void execute_command_process(Command command, int amp, int input, int output);

Command pull_command(CommandQueue queue);
CommandQueue init_queue();
int is_command_queue_empty(CommandQueue queue);

/*
 * Speicherverwaltung
 * */

void free_cmd(Command cmd);
void free_args(ArgList list);

/*
 * DEBUGGING
 * */
void debug_command(Command cmd, char** args);
void print_error(char* error_string);

#endif /*SMPSH_BASE_COMMANDS_H*/
