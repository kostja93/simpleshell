//
// Created by Konstantin Gontscharow on 08.06.15.
//

#ifndef OS_PRAKTIKUM_SIGNALS_H
#define OS_PRAKTIKUM_SIGNALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define LISTLENGTH 32

#define _MYLINUX_ 1

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

void sig_getlist();

int sig_getsignalnumber(char *pc);

#endif //OS_PRAKTIKUM_SIGNALS_H
