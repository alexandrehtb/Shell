#include "main.h"
#include "jobs.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

extern typeJob *jobsList;

/**
 * Cria uma nova job com seus devidos atributos e
 * a insere na jobsList.
 */
typeJob *createJob(pid_t pid, pid_t pgid, char *name, char *descriptor, int status) {
	usleep(10000);
	typeJob *newJob = malloc(sizeof(typeJob));

	newJob->name = (char *) malloc(sizeof(name));
	newJob->name = strcpy(newJob->name, name);
	newJob->pid = pid;
	newJob->pgid = pgid;
	newJob->status = status;
	newJob->descriptor = (char *) malloc(sizeof(descriptor));
	newJob->descriptor = strcpy(newJob->descriptor, descriptor);
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
 * Dada uma job especifica busca por ela na jobsList,
 * faz a remocao e retorna um ponteiro para a cabeca
 * da lista.
 */
typeJob *delJob(typeJob *job) {
	usleep(10000);
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
 * Dado o valor da busca e o parametro da busca varre
 * a jobsList a procura da jobs especificada e se
 * encontrada retorna um ponteiro para ela e NULL
 * caso contrario.
 */
typeJob *getJob(int searchValue, int searchParameter) {
	usleep(10000);
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
		default:
			return NULL;
			break;
	}
	return NULL;
}

/**
 * Busca pela job com determinado id na jobsList e
 * se encontrada altera seu status para o especificado.
 */
int changeJobStatus(int pid, int status) {
	usleep(10000);
	typeJob *job = jobsList;

	if (job == NULL)
		return 0;
	else {
		while (job != NULL) {
			if (job->pid == pid) {
				job->status = status;
				return TRUE;
			}
			job = job->next;
		}
		return FALSE;
	}
}


/**
 * Espera pelo termino de uma job.
 */


