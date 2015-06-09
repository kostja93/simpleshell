//
// Created by Konstantin Gontscharow on 08.06.15.
//

#include "signals.h"

/* FreeBSD */
#ifdef _MYFREEBSD_
char *signallist[] = { " ",\
"SIGHUP",   "SIGINT",   "SIGQUIT",  "SIGILL",   "SIGTRAP",   /* 01 - 05 */ \
"SIGABRT",  "SIGEMT",   "SIGFPE",   "SIGKILL",  "SIGBUS",    /* 06 - 10 */ \
"SIGSEGV",  "SIGSYS",   "SIGPIPE",  "SIGALRM",  "SIGTERM",   /* 11 - 15 */ \
"SIGURG",   "SIGSTOP",  "SIGTSTP",  "SIGCONT",  "SIGCHLD",   /* 16 - 20 */ \
"SIGTTIN",  "SIGTTOU", 	"SIGGIO",   "SIGXCPU",  "SIGXFSZ",   /* 21 - 25 */ \
"SIGVTALRM","SIGPROF", 	"SIGWINCH", "SIGINFO",  "SIGUSR1",   /* 26 - 30 */ \
"SIGUSR2",  "SIGTHR",    NULL                            };  /* 31 - 32 */
#endif

/* SOLARIS */
#ifdef _MYSOLARIS_
char *signallist[] = { " ",\
"SIGHUP",   "SIGINT",   "SIGQUIT",  "SIGILL",   "SIGTRAP",   /* 01 - 05 */
"SIGABRT",  "SIGEMT",   "SIGFPE",   "SIGKILL",  "SIGBUS",    /* 06 - 10 */
"SIGSEGV",  "SIGSYS",   "SIGPIPE",  "SIGALRM",  "SIGTERM",   /* 11 - 15 */
"SIGUSR1",  "SIGUSR2",  "SIGCHLD",  "SIGPWR",   "SIGWINCH",  /* 16 - 20 */
"SIGURG",   "SIGPOLL", 	"SIGSTOP",  "SIGTSTP",  "SIGCONT",   /* 21 - 25 */
"SIGTTIN",  "SIGTTOU", 	"SIGVTALARM","SIGPROF", "SIGXCPU",   /* 26 - 30 */
"SIGXFSZ",  "SIGWAITING",NULL                            };  /* 31 - 32 */
#endif


/* LINUX */
#ifdef _MYLINUX_
char *signallist[] = { " ",\
"SIGHUP",   "SIGINT",   "SIGQUIT",  "SIGILL",   "SIGTRAP",   /* 01 - 05 */
"SIGABRT",  "SIGBUS",   "SIGFPE",   "SIGKILL",  "SIGUSR1",   /* 06 - 10 */
"SIGSEGV",  "SIGUSR2",  "SIGPIPE",  "SIGALRM",  "SIGTERM",   /* 11 - 15 */
"SIGSTKFLT","SIGCHLD",  "SIGCONT",  "SIGSTOP",  "SIGTSTP",   /* 16 - 20 */
"SIGTTIN",  "SIGTTOU", 	"SIGURG",   "SIGXCPU",  "SIGXFSZ",   /* 21 - 25 */
"SIGVTALRM","SIGPROF", 	"SIGWINCH", "SIGIO",    "SIGPWR",    /* 26 - 30 */
"SIGSYS",   "SIGURTMIN", NULL                            };  /* 31 - 32 */
#endif

struct sigaction *oldHandler;

void sig_getlist()
{
    /* lists available signals */
    int sig=1;
    while( signallist[sig] != NULL ) {
        printf("Signal [%2d] %s\n", sig, signallist[sig]);
        sig++;
    }
    return;
}

int sig_getsignalnumber(char *pc)
{
    /* convert string to signal number */
    int sig=1;
    while( signallist[sig] != NULL ) {
        if( strcmp(signallist[sig], pc)== 0 ) break;
        sig++;
    }
    return sig;
}

HandlerFunction getHandler(char* handlerName) {
    if (strcmp(handlerName, "sayHello") == 0) {
        return &sayHello;
    }
    if (strcmp(handlerName, "print")) {
        return &printThatSignalWasCalled;
    }

    return NULL;
}

void installSignalHandler(int signalId, HandlerFunction handler) {
    struct sigaction new_action, old_action;
    new_action.sa_handler = handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = SA_RESTART;

    sigaction(signalId, NULL, &old_action);
    sigaction(signalId, &new_action, NULL);

    oldHandler[signalId] = old_action;
}

void uninstallSignalHandler(int signalId) {
    sigaction(signalId, &oldHandler[signalId], NULL);
}

void initOldHandlerList() {
    oldHandler = malloc(sizeof(struct sigaction) * LISTLENGTH);
}

void sayHello(int signal) {
    printf("Hello World!\n");
}
void printThatSignalWasCalled(int signal) {
    printf("signal %d was called\n", signal);
}