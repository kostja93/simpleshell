#include "commands.h"

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

CommandQueue initQueue() {
    CommandQueue result = malloc(sizeof(struct command_queue_struct));
    result->cmd = NULL;
    result->next = NULL;

    return result;
}

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

int isCommandQueueEmpty(CommandQueue queue) {
    if ( queue->cmd == NULL )
        return 1;
    else
        return 0;
}