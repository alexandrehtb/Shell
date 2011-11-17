#include "jobs.h"
#include "memory.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int numActiveJobs = 0;
extern typeJob *jobsList;


/**
 * Busca por uma determinada job de acordo com o parametro especificado
 * (id do processo, id da job, status da job, e jobs em foreground)
 * e o valor especificado).
 */

typeJob *getJob(int searchValue, int searchParameter) {
	typeJob *job = jobsList;

	switch(searchParameter) {
		case BY_PROCESS_ID:
			while (job != NULL) {
				if (job->pid == searchValue)
					return job;
				else
					job = job->next;
			}
			break;
		case BY_JOB_ID:
			while (job != NULL) {
				if (job->id == searchValue)
					return job;
				else
					job = job->next;
			}
			break;
		case BY_JOB_STATUS:
			while (job != NULL) {
				if (job->status == searchValue)
					return job;
				else
					job = job->next;
			}
			break;
		case BY_FOREGROUND_JOB:
			while (job != NULL) {
				if (job->foreground == searchValue)
					return job;
				else
					job = job->next;
			}
			break;
		default:
			return NULL;
			break;
	}
	return NULL;
}


/**
 * Cria uma nova job, seta seus campos com os respectivos argumentos e a
 * insere na lista de jobs.
 */

typeJob *createJob(pid_t pid, char *name, int status, int fg) {
	typeJob *newJob = (typeJob *) malloc(sizeof(typeJob));

	newJob->name = (char *) malloc(sizeof(name));
	newJob->name = strcpy(newJob->name, name);
	newJob->pid = pid;
	newJob->status = status;
	newJob->foreground = fg;
	newJob->next = NULL;

	if (jobsList == NULL) {
		numActiveJobs++;
		newJob->id = numActiveJobs;
		return newJob;
	}
	else {
		typeJob *aux = jobsList;
		while (aux->next != NULL)
			aux = aux->next;

		newJob->id = aux->id + 1;
		aux->next = newJob;
		numActiveJobs++;
		return jobsList;
	}
}


/**
 * Imprime lista de jobs.
 */

void printJobs(void) {
	typeJob *job = jobsList;

	if (job != NULL) {
		while (job != NULL) {
			printf("[%d]+   %d %30s\n", job->id, job->status, job->name);
			job = job->next;
		}
	}
	printf("\n");
}


/**
 * Busca por uma determinada job e faz sua remocao da lista. Retorna
 * um ponteiro para a jobsList.
 */

typeJob *delJob(typeJob *job) {
	if (jobsList == NULL)
		return NULL;

	typeJob *currentJob, *beforeCurrentJob;

	currentJob = jobsList->next;
	beforeCurrentJob = jobsList;

	if (beforeCurrentJob->pid == job->pid) {
		beforeCurrentJob = beforeCurrentJob->next;
		numActiveJobs--;
		return currentJob;
	}

	while (currentJob != NULL) {
		if (currentJob->pid == job->pid) {
			numActiveJobs--;
			beforeCurrentJob->next = currentJob->next;
		}
		beforeCurrentJob = currentJob;
		currentJob = currentJob->next;
	}
	return jobsList;
}


/**
 * Atualiza status de uma determinada job.
 */

void changeStatus(typeJob *job, int status) {
    if (WIFSTOPPED(status)) {
	job->status = PAUSED;
	job->foreground = 0;
    }
    else if (WIFCONTINUED(status)) {
	job->status = RUNNING;
    }
    else if (WIFEXITED(status)) {
	job->status = FINISHED;
	jobsList = delJob(job);
    }
}


