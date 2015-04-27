#include "commands.h"

Command add_command(char *command) {
    Command newCommand = malloc(sizeof(struct command_struct));
    newCommand->cmd    = malloc(sizeof(char));
    strcpy(newCommand->cmd, command);
    newCommand->args   = malloc(sizeof(struct args_struct));
    return newCommand;
}

void append_arg(Command command, char *arg) {
    ArgList list = command->args;
    while (list->next != NULL) {
        list = list->next;
    }
    list->arg = malloc(sizeof(char));
    strcpy(list->arg, arg);
    list->next = malloc(sizeof(struct args_struct));
}

CommandQueue init_queue() {
    CommandQueue result = malloc(sizeof(struct command_queue_struct));
    result->cmd = NULL;
    result->next = NULL;

    return result;
}

void push_command(CommandQueue queue, Command cmd) {
    CommandQueue q = malloc(sizeof(struct command_queue_struct));

    CommandQueue tmpQ = queue;
    q->cmd = cmd;

    while (tmpQ->next != NULL) {
        tmpQ = tmpQ->next;
    }

    tmpQ->next = q;
}

Command pull_command(CommandQueue queue) {
    Command cmd = queue->cmd;
    queue = queue->next;

    return cmd;
}

int is_command_queue_empty(CommandQueue queue) {
    if ( queue->cmd == NULL )
        return 1;
    else
        return 0;
}

void execute_command(Command command) {
    char *cmd = command->cmd;

    if (strcmp(cmd, "myecho") == 0){
        ArgList l = command->args;
        while (l != NULL) {
            if (l->arg != NULL) {
                printf("%s ", l->arg);
            }
            l = l->next;
        }
        printf("\n");
    }
    if (strcmp(cmd, "exit") == 0){
        int errorCode = 0;
        if (command->args->arg != NULL)
            errorCode = atoi(command->args->arg);

        exit(errorCode);
    }
    if (strcmp(cmd, "pwd") == 0){
        char path[1024];
        getcwd(path, sizeof(path));
        printf("%s\n", path);
    }
    if (strcmp(cmd, "cd") == 0){
        if (command->args->arg != NULL)
            chdir(command->args->arg);
    }
    if (strcmp(cmd, "kill") == 0){
        int errorCode = 0;
        if (command->args->arg != NULL)
            errorCode = atoi(command->args->arg);

        kill(errorCode, SIGKILL);
    }
}