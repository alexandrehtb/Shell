#include "builtincmd.h"
#include "jobs.h"
#include "memory.h"
#include "signals.h"
#include "string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h>

#define TRUE 1
#define FALSE !TRUE

#define SHELL_NAME "shell"

typeJob *jobsList = NULL;


/**
 * Exibe o prompt.
 */

void shellPrompt(void)
{
	char currentDirectory[512];

	getcwd(currentDirectory, sizeof (currentDirectory));
		printf("%s:%s> ", SHELL_NAME, currentDirectory);
}


/**
 * Executa o comando expecificado por argv
 */

void execute(char **argv) {
	execvp(argv[0], argv);
}


/**
 * Cria um novo processo. No processo filho chama execute que roda o comando em argv
 * enquanto no processo pai verifica se o novo processo deve rodar em foreground.
 * Se sim, espera pelo seu termino.
 */

void createProcess(char **argv, int fg)
{
	pid_t pid;

	if ((pid = fork()) == -1) {
		perror("Erro: fork()\n");
		exit(EXIT_FAILURE);	
	}
	else if (pid == 0) {   
		setpgid(0, 0);
		initializeChildrenSetup();

		execute(argv);

		fprintf(stderr, "Comando nao encontrado.\n");
		exit(EXIT_FAILURE);
	}
	else {
		jobsList = createJob(pid, *(argv), RUNNING, fg);

		if (fg) {
			setpgid(pid, 0);
			waitpid(pid, NULL, 0);
		}
	}
}


/**
 * Faz o parser de um comando nao built in. Verifica se ele deve ser executado
 * em foreground ou background e chama createProcess com a devida flag setada.
 */

void nonBuiltIn(char *cmd)
{
	int fg = 0;

	char **each_command = parser(cmd, "|");

	int i;

	for (i = 0; each_command[i]; i++) {
		char **argv = NULL;

		if (*each_command[i] == '&')
			argv = parser(each_command[i] + 1, " ");
		else {
			argv = parser(each_command[i], " ");
			fg = 1;
		}

		createProcess(argv, fg);

		if (argv)
			freeCommand(argv);
		free(argv);
	}

	free(each_command);
}



int main(void)
{
	initializeSignals();
	shellPrompt();

	while(TRUE) {
		char *cmd = readLine(stdin);

		if (*cmd == '\0') {
			free(cmd);
			continue;
		}

		if (builtInCommand(cmd) == 0)
			nonBuiltIn(cmd);

		shellPrompt();
		free(cmd);
	}

	return 0; 
}

