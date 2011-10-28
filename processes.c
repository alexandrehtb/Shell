#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void executeCommand(char *command[], char *file, int newDescriptor, int executionMode) {
	if (!execvp(command[0], command))
		printf("Erro: %s\n", command[0]);
}

void launchJob(char *command[], char *file, int newDescriptor, int executionMode) {
       pid_t pid;
       pid = fork();

       if (pid < 0) {
	       printf("Erro: fork()\n");
	       exit(EXIT_FAILURE);
       }
       else if (pid == 0) {
	       executeCommand(command, file, newDescriptor, executionMode);
       }
       else {
	       int status;
	       wait(status);
       }
}

