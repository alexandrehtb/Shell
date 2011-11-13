#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/**
 * Testa o comando e verifica se eh algum comando builtin. Se for executa
 * e retorna 0, caso contrario retorna 1.
 */

int builtInCommand(char **argv) {
	if (strcmp(argv[0], "exit") == 0)
		my_exit();
	if (strcmp(argv[0], "cd") == 0)
		return changeDirectory(argv);
	if (strcmp(argv[0], "jobs") == 0)
		return printJobs(argv);
	if (strcmp(argv[0], "bg") == 0)
		return bg(argv);
	if (strcmp(argv[0], "fg") == 0)
		return fg(argv);

	return 0;
}

void my_exit() {
	printf("Bye, bye...\n\n");
	exit(EXIT_SUCCESS);
}

int changeDirectory(char *argv[]) {
	if (argv[1] == NULL) {
		chdir(getenv("HOME"));
	}
	else {
		if (chdir(argv[1]) == -1) {
			fprintf(stderr, "cd: %s: Diretorio nao encontrado\n", argv[1]);
		}
	}

	return 1;
}

int printJobs(char *argv[]) {
	return 1;
}

int bg(char *argv[]) {
	return 1;
}

int fg(char *argv[]) {
	return 1;
}
