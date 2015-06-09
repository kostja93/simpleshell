#include "commands.h"

Command init_command(char *command) {
    Command newCommand = (Command) malloc(sizeof(struct command_struct));
    newCommand->cmd    = (char *)  malloc(strlen(command)+1);
    strcpy(newCommand->cmd, command);
    return newCommand;
}

void append_arg(Command command, char *arg) {
    ArgList list = command->args;

    ArgList newElement = (ArgList) malloc(sizeof(struct args_struct));
    newElement->arg = (char *) malloc(strlen(arg) +1);
    strcpy(newElement->arg, arg);

    if (list == NULL)
        command->args = newElement;
    else {
        while (list->next != NULL) {
            list = list->next;
        }
        list->next = newElement;
    }
}

void push_command(CommandQueue queue, Command cmd) {
    CommandQueue newQ;
    CommandQueue tmpQ;

    if (queue == NULL) {
        print_error("invalid queue given");
        return;
    }
    if (cmd == NULL) {
        print_error("invalid command given");
        return;
    }

    newQ = (CommandQueue) malloc(sizeof(struct command_queue_struct));
    newQ->cmd = cmd;
    tmpQ = queue;

    if (tmpQ->cmd == NULL) {
        tmpQ->cmd = cmd;
        return;
    }

    while (tmpQ->next != NULL) {
        tmpQ = tmpQ->next;
    }

    tmpQ->next = newQ;
}

Command pull_command(CommandQueue queue) {
    CommandQueue tmpQ = queue;

    while (tmpQ->next->next != NULL) {
        tmpQ = tmpQ->next;
    }

    queue = tmpQ;

    return tmpQ->cmd;
}

int is_command_queue_empty(CommandQueue queue) {
    if ( queue == NULL || queue->cmd == NULL )
        return 1;
    else
        return 0;
}

int execute_command(Command command) {

    char *cmd;
    int executed;

    if (command == NULL) {
        print_error("Uninitialzied use of command in execute_command");
        return 0;
    }

    cmd = command->cmd;
    executed = 0;

    if (cmd == NULL) {
        print_error("No command given in execute_command");
        return 0;
    }

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
        if (command->args != NULL && command->args->arg != NULL)
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
    if (strcmp(cmd, "sig_install") == 0) {
        ArgList arguments = command->args;
        char *signal, *handler;
        int signalValue;
        signal    = arguments->arg;
        arguments = arguments->next;
        handler   = arguments->arg;
        signalValue = sig_getsignalnumber(signal);
        printf("Installing new handler %s for %s (%d)\n", handler, signal, signalValue);

        installSignalHandler(signalValue, getHandler(handler));
        executed = 1;
    }
    if (strcmp(cmd, "sig_uninstall") == 0) {
        int signalValue;
        signalValue = sig_getsignalnumber(command->args->arg);
        printf("Removing handler from signal %s (%d)\n", command->args->arg, signalValue);
        uninstallSignalHandler(signalValue);
        executed = 1;
    }

    return executed;
}

int get_arg_list_length(ArgList list) {
    int i = 1;
    ArgList tmp = list;

    if (tmp == NULL) {
        return 0;
    }

    while (tmp->next != NULL) {
        i++;
        tmp = tmp->next;
    }

    return i;
}

char* append_dir(char *cmd) {
    char* path = "/bin/";
    char* append;

    if (cmd == NULL) {
        print_error("No valid command given in append_dir");
        return NULL;
    }

    if((append = malloc(strlen(cmd) + strlen(path) +1)) != NULL) {
        append[0] = '\0';
        strcat(append, path);
        strcat(append, cmd);
    } else {
        print_error("Couldn't append string");
        return NULL;
    }

    return append;
}

void get_array_of_args(Command cmd, char** args) {
    int i;
    ArgList tmp;
    i = 1;
    tmp = cmd->args;
    if (NULL == cmd) {
        print_error("Unvalid command given");
        return;
    }
    if (NULL == args) {
        print_error("Unvalid array for char array given");
        return;
    }
    args[0]     = append_dir(cmd->cmd);

    if (tmp != NULL) {
        args[i++] = tmp->arg;
        while (tmp->next != NULL) {
            args[i++] = tmp->arg;
            tmp = tmp->next;
        }
    }

    args[i] = (char *) 0;
}

void execute_command_process(Command command, int amp, int input, int output) {
    int status;
    int pid;
    pid = fork();

    if (pid == 0) {
        char *args[MAX_ARGS];
        if ( (get_arg_list_length(command->args) + 2) > MAX_ARGS) {
            print_error("To many arguments");
            return;
        }
        get_array_of_args(command, args);
        if (input != STDIN_FILENO) {
            if ( dup2(input, STDIN_FILENO) < 0 ){
                print_error("Error while changing stdin");
                exit(-1);
            }
            close_d(input);
        }
        if (output != STDOUT_FILENO) {
            if ( dup2(output, STDOUT_FILENO) < 0){
                print_error("Error while changing stdout");
                exit(-2);
            }
            close_d(output);
        }
        if (execvp(command->cmd, args) == -1 ){
            print_error("Error while loading programm");
            exit(-3);
        }
    } else {
        if ( amp == 0 ) {
            waitpid(pid, &status, 0);
        }
    }
}

void execute_commandp(Command cmd, int amp) {
    if (cmd == NULL) {
        print_error("Uninitialized Command in execute_commandp");
        return;
    }

    if (execute_command(cmd) == 0) {
        execute_command_process(cmd, amp, 0, 1);
    }
}

void execute_queue(CommandQueue cmds, int amp) {
    int fd[2];
    int input;
    int output;
    Command cmd;

    if (cmds == NULL) {
        print_error("Unvalid queue given for execution");
        return;
    }

    output = 0;

    do {
        if (cmds->cmd != NULL) {
            cmd = cmds->cmd;
            if (execute_command(cmd) == 0){
                input = output;
                if( pipe(fd) < 0 ) {
                    print_error("error while creating pipe");
                    return;
                }

                if (cmds->next == NULL){
                    output = STDOUT_FILENO;
                    close_d(fd[1]);
                }
                else
                    output = fd[1];

                execute_command_process(cmd, amp, input, output);
                if (input != STDIN_FILENO)
                    close_d(input);
                if (output != STDOUT_FILENO)
                    close_d(output);

                output = fd[0];
            }
        }
        cmds = cmds->next;
    } while (cmds != NULL);
}

void free_cmd(Command cmd) {
    if (cmd == NULL) {
        print_error("Not freeable");
        return;
    }

    if (NULL != cmd->cmd)
        free(cmd->cmd);
    if (NULL != cmd->args)
        free_args(cmd->args);
    free(cmd);
}

void free_args(ArgList list) {
    if (list == NULL) {
        print_error("Not freeable");
        return;
    }

    if (NULL != list->arg)
        free(list->arg);
    if (list->next != NULL)
        free_args(list->next);
    free(list);
}

void free_queue(CommandQueue queue) {
    return;
    if (queue == NULL) {
        print_error("Not valid queue for freeing");
        return;
    }

    if (queue->cmd != NULL)
        free_cmd(queue->cmd);

    if (queue->next != NULL)
        free_queue(queue->next);

    free(queue);
}

/*
 * DEBUGGING
 * */
void debug_command(Command cmd) {
    ArgList l = cmd->args;
    if (cmd->cmd == NULL) {
        printf("No command given to print\n");
        return;
    }
    printf("-%s\n", cmd->cmd);
    if (l != NULL) {
        do {
            printf("\t-%s\n", l->arg);
            l = l->next;
        } while(l != NULL);
    }
}

void debug_commands_queue(CommandQueue cmds) {
    CommandQueue q;
    q = cmds;
    printf("Printing Queue:\n");
    if (q != NULL)
        do {
            debug_command(q->cmd);
            printf("-----------------\n");
            q = q->next;
        }while (q != NULL);
    else
        printf("Empty Queue");
}

void debug_arg_array(char** args) {
    int i = 0;
    printf("Your arguments:\n");
    while (args[i] != (char) 0)
        printf("{%s}\n",args[i++]);
}

void print_error(char* error_string) {
    int logfile;
    printf("\n\n%s!\n", error_string);

    logfile = open("smpsh.log", O_WRONLY | O_APPEND);
    if (logfile > 0) {
        write(logfile, error_string, strlen(error_string));
    }

    close_d(logfile);
}

void close_d(int descriptor) {
    if (descriptor != STDIN_FILENO && descriptor != STDOUT_FILENO)
        close(descriptor);
}