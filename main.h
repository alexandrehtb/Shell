#ifndef MAIN_H
#define MAIN_H

#define SHELL_NAME "shell"

#define TRUE 1
#define FALSE !TRUE

void shellPrompt(void);
void nonBuiltIn(char *argv[], char *file, int newDescriptor);

#endif
