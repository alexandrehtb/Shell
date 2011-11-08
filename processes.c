#include "definitions.h"
#include "processes.h"
#include "strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

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
	       /*
	       executeCommand(command, file, newDescriptor, executionMode);

	       exit(EXIT_SUCCESS);

	       INICIO TESTES
	       */

	       signal(SIGCHLD, &signalChildHandler); // desvia o sinal do processo filho
	       usleep(20000);
	       setpgrp(); //?
	       if (executionMode == FOREGROUND)
		       tcsetpgrp(SHELL_TERMINAL, getpid());
	       if (executionMode == BACKGROUND)
		       printf("[%d] NUMERO DO PROCESSO\n", ++numActiveJobs);

	       executeCommand(command, file, newDescriptor, executionMode);
printf("vai sair...\n");
	       exit(EXIT_SUCCESS);
       }
       else {
	       /*
	       int status=2000;
	       wait(status);
	       */
	       setpgid(pid, pid);

	       jobsList = insertJob(pid, pid, *(command), file, (int) executionMode);

	       typeJob *job = getJob(pid, BY_PROCESS_ID);

	       if (executionMode == FOREGROUND)
		       putJobForeground(job, FALSE);
	       if (executionMode == BACKGROUND)
		       putJobBackground(job, FALSE);
       }
}

void putJobForeground(typeJob *job, int continueJob) {
	job->status = FOREGROUND;
	tcsetpgrp(SHELL_TERMINAL, job->pgid);
	if (continueJob)
		if (kill(-job->pgid, SIGCONT) < 0)
			perror("kill (SIGCONT)");

	waitJob(job);
	tcsetpgrp(SHELL_TERMINAL, SHELL_PGID);
}

void putJobBackground(typeJob *job, int continueJob) {
	if (job == NULL)
		return;

	if (continueJob && job->status != WAITING)
		job->status = WAITING;
	if (continueJob)
		if (kill(-job->pgid, SIGCONT) < 0)
			perror("kill (SIGOUT)");

	tcsetpgrp(SHELL_TERMINAL, SHELL_PGID);
}

void waitJob(typeJob *job) {
	int terminationStatus;
	
	while (waitpid(job->pid, &terminationStatus, WNOHANG) == 0)
		if (job->status == SUSPENDED)
			return;
	jobsList = delJob(job);
}

typeJob *insertJob(pid_t pid, pid_t pgid, char *name, char *descriptor, int status) {
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

int changeJobStatus(int pid, int status) {
	usleep(10000);
	typeJob *job = jobsList;

	if (job == NULL)
		return 0;
	else {
		int counter = 0;
		while (job != NULL) {
			if (job->pid == pid) {
				job->status = status;
				return TRUE;
			}
			counter++;
			job = job->next;
		}
		return FALSE;
	}
}

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

void signalChildHandler(int p) {
	pid_t pid;
	int terminationStatus;

	pid = waitpid(WAIT_ANY, &terminationStatus, WUNTRACED | WNOHANG);
	if (pid > 0) {
		typeJob *job = getJob(pid, BY_PROCESS_ID);
		if (job == NULL)
			return;
		if (WIFEXITED(terminationStatus)) {
			if (job->status == BACKGROUND) {
				printf("\n[%d]+ Done\t	%s\n", job->id, job->name);
				jobsList = delJob(job);
			}
		}
		else if (WIFSIGNALED(terminationStatus)) {
			printf("\n[%d]+ KILLED\t	%s\n", job->id, job->name);
			jobsList = delJob(job);
		}
		else if (WIFSTOPPED(terminationStatus)) {
				if (job->status == BACKGROUND) {
					tcsetpgrp(SHELL_TERMINAL, SHELL_PGID);
					changeJobStatus(pid, WAITING);
					printf("\n[%d]+   suspended [wants input]\t	%s\n", numActiveJobs, job->name);
				}
				else {
					tcsetpgrp(SHELL_TERMINAL, job->pgid);
					changeJobStatus(pid, SUSPENDED);
					printf("\n[%d]+   stopped\t	%s\n", numActiveJobs, job->name);
				}
				return;
		}
		else {
			if (job->status == BACKGROUND)
				jobsList = delJob(job);
		}
		tcsetpgrp(SHELL_TERMINAL, SHELL_PGID);
	}
}
