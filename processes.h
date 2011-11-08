#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define FOREGROUND 'f'
#define BACKGROUND 'b'
#define SUSPENDED 's'
#define WAITING 'W'

#define BY_PROCESS_ID 1
#define BY_JOB_ID 2
#define BY_JOB_STATUS 3

static int numActiveJobs = 0;

typedef struct job {
	int id;
	char *name;
	pid_t pid;
	pid_t pgid;
	int status;
	char *descriptor;
	struct job *next;
} typeJob;

static typeJob *jobsList = NULL;

void executeCommand(char *command[], char *file, int newDescrciptor, int executionMode);
void launchJob(char *command[], char *file, int newDescriptor, int executionMode);
typeJob *insertJob(pid_t pid, pid_t pgid, char *name, char *descriptor, int status);
typeJob *delJob(typeJob *job);
void putJobForeground(typeJob *job, int continueJob);
void putJobBackground(typeJob *job, int continueJob);
void waitJob(typeJob *job);
void signalChildHandler(int p);
int changeJobStatus(int pid, int status);
typeJob *getJob(int searchValue, int searchParameter);

#endif
