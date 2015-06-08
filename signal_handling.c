//
// Created by Konstantin Gontscharow on 08.06.15.
//

#include "signal_handling.h"

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