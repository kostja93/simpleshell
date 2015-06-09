//
// Created by Konstantin Gontscharow on 08.06.15.
//

#ifndef OS_PRAKTIKUM_SIGNALS_H
#define OS_PRAKTIKUM_SIGNALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void sig_getlist(void);
int  sig_getsignalnumber(char *);

void installSignalHandler(int, HandlerFunction);
HandlerFunction getHandler(char*);

/*
 * Handler functions
 */

void sayHello();
void printThatSignalWasCalled();

#define LISTLENGTH 32
#define _MYLINUX_ 1

extern char *signallist[];

typedef void (*HandlerFunction) (void);

#endif //OS_PRAKTIKUM_SIGNALS_H
