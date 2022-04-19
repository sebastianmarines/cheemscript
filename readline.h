#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <string.h>


char* readline(char* prompt);

void add_history(char* unused);

#else
#include <editline/readline.h>
#endif
