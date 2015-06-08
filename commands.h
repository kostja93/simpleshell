#ifndef SMPSH_BASE_COMMANDS_H
#define SMPSH_BASE_COMMANDS_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include "signal_handling.h"

#define __EXTENSIONS__

#define MAX_ARGS 32

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

/*
 * Commands Queue
 * */
void push_command(CommandQueue queue, Command cmd);
Command pull_command(CommandQueue queue);
int is_command_queue_empty(CommandQueue queue);

/*
 * Commands
 * */
Command init_command(char *command);
void append_arg(Command command, char *arg);

/*
 * Execution
 * */
int execute_command(Command command);
void execute_commandp(Command cmd, int amp);
void execute_command_process(Command command, int amp, int input, int output);
void execute_queue(CommandQueue cmds, int amp);

/*
 * Speicherverwaltung
 * */

void free_cmd(Command cmd);
void free_args(ArgList list);
void free_queue(CommandQueue queue);

/*
 * DEBUGGING
 * */
void debug_command(Command cmd);
void print_error(char* error_string);
void debug_arg_array(char** args);
void debug_commands_queue(CommandQueue cmds);

#endif /*SMPSH_BASE_COMMANDS_H*/
