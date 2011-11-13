#include "main.h"
#include "jobs.h"
#include "builtin.h"
#include "signals.h"
#include "strings.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static pid_t SHELL_PID;
static pid_t SHELL_PGID;

int terminationStatus;

typeJob *jobsList = NULL;

/**
 * Exibe o diretorio corrent seguido do nome do shell e aguarda a entrada
 * de um comando.
 */

void shellPrompt() {
	char currentDirectory[512];

	getcwd(currentDirectory, sizeof (currentDirectory));
		printf("%s:%s> ", SHELL_NAME, currentDirectory);
}

void execute(char *argv[]) {
	execvp(argv[0], argv);
}

void forkProcess(char *argv[], char *file, int newDescriptor, int executionMode) {
	pid_t pid = fork();

	if (pid == -1) {
		printf("Error: fork()\n\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) {
		setpgid(SHELL_PID, SHELL_PID);

		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);

		execute(argv);
		exit(EXIT_SUCCESS);
	}
	else {
		jobsList = createJob(pid, pid, *(argv), file, executionMode);

		typeJob *job = getJob(pid, BY_PROCESS_ID);

		if (executionMode == FOREGROUND) {
			terminationStatus = 1;
			while(terminationStatus);
		}
	}
}

void nonBuiltIn(char *argv[], char *file, int newDescriptor) {
	int fg = BACKGROUND, i = 0;

	while (argv[i + 1] != NULL)
		i++;

	if (strcmp(argv[i], "&") != 0)
		fg = FOREGROUND;

	forkProcess(argv, file, newDescriptor, fg);

	// Precisa liberar *argv[]
}

int main(void) {
	SHELL_PID = getpid();
	initializeSignals();

	while (TRUE) {
		shellPrompt();
		char *line = readLine(stdin);

		if (*line == '\0') {
			free(line);
			continue;
		}

		char **argv = parser(line);
		if (!builtInCommand(argv))
			nonBuiltIn(argv, "STANDARD", 0);
	}

	return 0;
}
