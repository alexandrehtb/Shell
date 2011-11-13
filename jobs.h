#include <stdio.h>
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

typeJob *createJob(pid_t pid, pid_t pgid, char *name, char *descriptor, int status);
typeJob *delJob(typeJob *job);
typeJob *getJob(int searchValue, int searchParameter);
