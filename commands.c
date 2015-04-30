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
    result->cmd         = malloc(sizeof(struct command_struct));
    result->next        = NULL;

    return result;
}

void push_command(CommandQueue queue, Command cmd) {
    CommandQueue q = init_queue();
    q->cmd = malloc(sizeof(struct command_struct));
    memcpy(q->cmd, cmd, sizeof(struct command_struct));

    CommandQueue tmpQ = queue;

    while (tmpQ->next != NULL) {
        tmpQ = tmpQ->next;
    }

    tmpQ->next = q;
}

Command pull_command(CommandQueue queue) {
    CommandQueue tmpQ = queue;

    while (tmpQ->next->next != NULL) {
        tmpQ = tmpQ->next;
    }

    return tmpQ->cmd;
}

int is_command_queue_empty(CommandQueue queue) {
    if ( queue->cmd == NULL )
        return 1;
    else
        return 0;
}

int execute_command(Command command) {
    char *cmd = command->cmd;
    int executed = 0;

    if (strcmp(cmd, "myecho") == 0){
        ArgList l = command->args;
        while (l != NULL) {
            if (l->arg != NULL) {
                printf("%s ", l->arg);
            }
            l = l->next;
        }
        printf("\n");
        executed = 1;
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
        executed = 1;
    }
    if (strcmp(cmd, "cd") == 0){
        if (command->args->arg != NULL)
            chdir(command->args->arg);

        executed = 1;
    }
    if (strcmp(cmd, "kill") == 0){
        int errorCode = 0;
        if (command->args->arg != NULL)
            errorCode = atoi(command->args->arg);

        kill(errorCode, SIGKILL);
        executed = 1;
    }

    return executed;
}

int get_arg_list_length(ArgList list) {
    int i = 0;
    ArgList tmp = list;
    while (tmp->next != NULL) {
        i++;
        tmp = tmp->next;
    }

    return i;
}

void get_array_of_args(Command cmd, char** args) {
    int i = 1;
    ArgList tmp = cmd->args;
    args[0] = malloc(sizeof(char *));
    args[0] = cmd->cmd;
    while (tmp->next != NULL) {
        args[i] = malloc(sizeof(char *));
        args[i++] = tmp->arg;
        tmp = tmp->next;
    }
}

/*
 * DEBUGGING
 * */
void debug_command(Command cmd, char** args) {
    int i;
    printf("-%s", cmd->cmd);
    for ( i = 0; i <= get_arg_list_length(cmd->args); i++) {
        printf("-%s\n", args[i]);
    }
}

void execute_command_process(Command command, int amp) {
    int pid = fork();

    if (pid == 0) {
        char *args[get_arg_list_length(command->args) + 1];
        get_array_of_args(command, args);

        if (execv(command->cmd, args) == -1 ) {
            printf("An Error accurt while creating a new proccess");
            exit(-1);
        }
    } else {

    }
}

void execute_commandp(Command cmd, int amp) {
    if (execute_command(cmd) == 0) {
        execute_command_process(cmd, amp);
    }
}

void free_cmd(Command cmd) {
    free(cmd->cmd);
    free_args(cmd->args);
    free(cmd);
}

void free_args(ArgList list) {
    free(list->arg);
    if (list->next != NULL)
        free_args(list->next);
    free(list);
}