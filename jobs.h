#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>
#include <unistd.h>

#define BY_PROCESS_ID 1
#define BY_JOB_ID 2
#define BY_JOB_STATUS 3
#define BY_FOREGROUND_JOB 4

#define RUNNING 'r'
#define PAUSED 'p'
#define FINISHED 'f'

typedef struct job
{
	int id;
	pid_t pid;
	char *name;
	int status;
	int foreground;
	struct job *next;
} typeJob;

typeJob *createJob(pid_t pid, char *name, int status, int fg);
typeJob *getJob(int searchValue, int searchParameter);
typeJob *delJob(typeJob *job);
void changeStatus(typeJob *job, int status);
void printJobs(void);

#endif

