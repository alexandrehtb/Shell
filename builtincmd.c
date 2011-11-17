#include "builtincmd.h"
#include "jobs.h"
#include "memory.h"
#include "string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

extern typeJob *jobs;


/**
 * Verifica se cd possui um argumento. Se sim tenta mudar o diretorio para
 * o especificado, senao altera para o diretorio HOME.
 */

int cd(char *argv) {
	char **tokens = parser(argv, " ");

	if (tokens[1] == NULL) {
		chdir(getenv("HOME"));
	}
	else {
		if (chdir(tokens[1]) == -1) {
			fprintf(stderr, "cd: %s: Diretorio nao encontrado\n", tokens[1]);
		}
	}

	return 1;
}


/**
 * Joga a job com id especificado para background.
 */

void bg(char *argv) {
	char **tokens = parser(argv, " ");

	if (!tokens[1]) {
		fprintf(stderr, "Erro de sintaxe: bg espera um id.\n");
		return;
	}

	typeJob *job = getJob(atoi(tokens[1]), BY_JOB_ID);

	if (!job)  {
		fprintf(stderr, "fg: %d: job nao existente.\n", atoi(tokens[1]));
		return;
	}

	if (job->status == FINISHED)
		return;

	job->foreground = 0;
	job->status = RUNNING;

	kill(job->pid, SIGCONT);
}


/**
 * Joga a job com id especificado para foreground.
 */

void fg(char *argv) {
	char **tokens = parser(argv, " ");

	if (!tokens[1]) {
		fprintf(stderr, "\n\n Erro de sintaxe: fg espera um id.\n");
		return;
	}

	typeJob *job = getJob(atoi(tokens[1]), BY_JOB_ID);

	if (!job) {
		fprintf(stderr, "bg: %d: job nao existente..\n", atoi(tokens[1]));
		return;
	}
	if (job->status == FINISHED)
		return;

	if (job->foreground)
		return;

	job->foreground = 1;
	job->status = RUNNING;

	kill(job->pid, SIGCONT);
}


/**
 * Testa o comando e verifica se eh algum comando builtin. Se for executa
 * e retorna 0, caso contrario retorna 1.
 */

int builtInCommand(char *argv) {
	if (strcmp(argv, "exit") == 0) {
		exit(EXIT_SUCCESS);
	}
	else if (strncmp(argv, "cd", 2) == 0) {
		return cd(argv);
	}
	else if (strcmp(argv, "jobs") == 0) {
		printJobs();

		return 1;
	}
	else if (strncmp(argv, "bg", 2) == 0) {
		bg(argv);
		return 1;
	}
	else if (strncmp(argv, "fg", 2) == 0) {
		fg(argv);
		return 1;
	}

	return 0;
}

