#ifndef SMPSH_BASE_COMMANDS_H
#define SMPSH_BASE_COMMANDS_H


#include <stdlib.h>

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

void pushCommand(CommandQueue queue, Command cmd);

Command pullCommand(CommandQueue queue);

Command add_command(char *command);

void append_arg(Command command, char *arg);

#endif /*SMPSH_BASE_COMMANDS_H*/
