//
// Created by Konstantin Gontscharow on 28.05.15.
//

#include "signals.h"



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