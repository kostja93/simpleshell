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
    if (NULL == cmd)
        print_error("Unvalid command given");
    if (NULL == args)
        print_error("Unvalid array for char array given");

    int i = 1;
    ArgList tmp = cmd->args;
    /*
     * creating full path
     * */
    char* path = "/bin/";
    if((args[0] = malloc(strlen(cmd->cmd) + strlen(path) +1)) != NULL) {
        args[0][0] = '\0';
        strcat(args[0], path);
        strcat(args[0], cmd->cmd);
    } else {
        print_error("Couldn't append string");
    }

    while (tmp->next != NULL) {
        args[i++] = tmp->arg;
        tmp = tmp->next;
    }
    args[i] = (char *) 0;
}

void execute_command_process(Command command, int amp, int input, int output) {
    int status;
    int pid = fork();

    if (pid == 0) {
        char *args[get_arg_list_length(command->args) + 2];
        get_array_of_args(command, args);

        if (input != 0) {
            if ( dup2(input, 0) < 0 )
                print_error("Error while changing stdin");
        }
        if (output != 1) {
            if ( dup2(output, 1) < 0)
                print_error("Error while changing stdout");
        }

        if (execvp(command->cmd, args) == -1 )
            print_error("Error while creating new process");
    } else {
        if ( amp == 0 ) {
            waitpid(pid, &status, 0);
        }
    }
}

void execute_commandp(Command cmd, int amp) {
    if (execute_command(cmd) == 0) {
        execute_command_process(cmd, amp, 0, 1);
    }
}

void free_cmd(Command cmd) {
    if (NULL != cmd->cmd)
        free(cmd->cmd);
    if (NULL != cmd->args)
        free_args(cmd->args);
    free(cmd);
}

void free_args(ArgList list) {
    if (NULL != list->arg)
        free(list->arg);
    if (list->next != NULL)
        free_args(list->next);
    free(list);
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

void print_error(char* error_string) {
    printf("\n\n%s!\n", error_string);
    exit(-1);
}