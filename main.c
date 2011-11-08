#include "strings.h"
#include "processes.h"
#include "definitions.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static char *currentDirectory;

void handleCommand(char *argv[]) {
	if (!builtInCommand(argv))
		launchJob(argv, "STANDARD", 0, FOREGROUND);
}

void changeDirectory(char *argv[]) {
	if (argv[1] == NULL) {
		chdir(getenv("HOME"));
	}
	else {
		if (chdir(argv[1]) == -1) {
			printf(" %s directory not found.\n", argv[1]);
		}
	}
}

/* --------------------- */
/* EXIBE PROMPT DO SHELL */
/* --------------------- */
void shellPrompt() {
	printf("%s :> ", getcwd(currentDirectory, 1024));
}

/* ----------------- */
/* COMANDOS BUILT-IN */
/* ----------------- */
int builtInCommand(char *argv[]) {
	if (!strcmp("exit", argv[0])) {
		exit(EXIT_SUCCESS);
	}
	if (!strcmp("cd", argv[0])) {
		changeDirectory(argv);
		return 1;
	}

	return 0;
}

void initialize() {
	SHELL_PID = getpid();
	SHELL_TERMINAL = STDIN_FILENO;
	INTERACTIVE = isatty(SHELL_TERMINAL);

	if (INTERACTIVE) {
		while (tcgetpgrp(SHELL_TERMINAL) != (SHELL_PGID = getpgrp()))
			kill(SHELL_PID, SIGTTIN);

		signal(SIGCHLD, &signalChildHandler);

		setpgid(SHELL_PID, SHELL_PID);
		SHELL_PGID = getpgrp();
		if (SHELL_PID != SHELL_PGID) {
			printf("Error, the shell is not process group leader\n");
			exit(EXIT_FAILURE);
		}
		if (tcsetpgrp(SHELL_TERMINAL, SHELL_PGID) == -1)
			tcgetattr(SHELL_TERMINAL, &SHELL_TMODES);

		currentDirectory = (char *) malloc(sizeof(char)*1024);
	}
	else {
		printf("Could not make SHELL interactive. Exiting.\n");
		exit(EXIT_FAILURE);
	}
}

int main(void) {
	initialize();
	shellPrompt();

	while(TRUE) {
		char *line = readLine(stdin),
		     **argv = parser(line);

		handleCommand(argv);
		destroyCommand(argv);
		shellPrompt();
	}

	return 0;
}
