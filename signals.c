#include "signals.h"
#include "jobs.h"

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

static struct sigaction act_TSTP;
static struct sigaction oldact_TSTP;

static struct sigaction act_CHLD;
static struct sigaction oldact_CHLD;

extern terminationStatus;

/**
 * Redireciona os sinais TSTP e CHLD para as respectivas funcoes
 * que irao trata-los.
 */

void initializeSignals(void) {
	sigemptyset(&act_TSTP.sa_mask);
	act_TSTP.sa_flags = SA_SIGINFO;
	act_TSTP.sa_sigaction = TSTP_Handler;

	if (sigaction(SIGTSTP, &act_TSTP, &oldact_TSTP) == -1)
		fprintf(stderr, "\nErro ao redirecionar sinal TSTP.\n");

	sigemptyset(&act_CHLD.sa_mask);
	act_CHLD.sa_flags = SA_SIGINFO;
	act_CHLD.sa_sigaction = CHLD_Handler;

	if (sigaction(SIGCHLD, &act_CHLD, &oldact_CHLD) == -1)
		fprintf(stderr, "\nErro ao redirecionar sinal CHLD.\n");
}

void TSTP_Handler(int signal, siginfo_t *info, void *context) {
	typeJob *job = getJob(FOREGROUND, BY_JOB_STATUS);
	printf("%s\n", job->name);
	if (job == NULL)
		return;
	else {
		job->status = SUSPENDED;
		kill(job->pid, SIGSTOP);
	}
}

void CHLD_Handler(int signal, siginfo_t *info, void *context) {
	int status;

	waitpid(info->si_pid, &status, WUNTRACED | WCONTINUED);
	terminationStatus = 0;

	changeJobStatus(getJob(status, BY_PROCESS_ID), status);
}
