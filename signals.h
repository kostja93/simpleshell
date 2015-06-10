//
// Created by Konstantin Gontscharow on 08.06.15.
//

#ifndef OS_PRAKTIKUM_SIGNALS_H
#define OS_PRAKTIKUM_SIGNALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

typedef void (*HandlerFunction) (int);

void sig_getlist(void);
int  sig_getsignalnumber(char *);

/*
 * Signal installation
 */

void installSignalHandler(int, HandlerFunction);
void uninstallSignalHandler(int);
HandlerFunction getHandler(char*);
void initOldHandlerList();

/*
 * Handler functions
 */

void sayHello(int);
void printThatSignalWasCalled(int);

/*
 * Signal blocking
 */
extern sigset_t *blockedSignals;

void blockSignal(int);
void unblockSignal(int);

#define LISTLENGTH 32
#define _MYLINUX_ 1

extern char *signallist[];
extern struct sigaction *oldHandler;

#endif //OS_PRAKTIKUM_SIGNALS_H
