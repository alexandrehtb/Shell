#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <termios.h>

#define TRUE 1
#define FALSE 0

static pid_t SHELL_PID;
static pid_t SHELL_PGID;
static int SHELL_TERMINAL, INTERACTIVE;
static struct termios SHELL_TMODES;
//static char *currentDirectory;

void handleCommand(char *command[]);
int builtInCommand(char *command[]);
void shellPrompt();

#endif
