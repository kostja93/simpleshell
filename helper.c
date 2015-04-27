#include "helper.h"

char* trimQuote(char* string, int stringlen) {
    int i;
    for (i = 0; i < stringlen; i++) {
        string[i] = string[i+1];
        if (string[i+1] == '"')
            string[i] = ' ';
    }

    return string;
}