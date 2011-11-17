#include "jobs.h"
#include "signals.h"

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>


extern typeJob *jobsList;

static struct sigaction act_TSTP;
static struct sigaction oldact_TSTP;

static struct sigaction act_CHLD;
static struct sigaction oldact_CHLD;


/**
 * Trata sinais TSTP.
 */

void TSTP_handler(int sig, siginfo_t *info, void *context) {
	typeJob *fgjob = getJob(1, BY_FOREGROUND_JOB);
   
	if (!fgjob) {
		return;
	}
	else {
		fgjob->foreground = 0;
	
		if (fgjob->status != FINISHED) {
			fgjob->status = PAUSED;
			kill(fgjob->pid, SIGSTOP);
		}
	}

}


/**
 * Trata sinais CHLD.
 */

void CHLD_handler(int sig, siginfo_t *info, void *context) {
	int status;

	waitpid(info->si_pid, &status, WUNTRACED | WCONTINUED);

	changeStatus(getJob(info->si_pid, BY_PROCESS_ID), status);
}



/**
 * Redireciona os sinais para seus devidos handlers.
 */

void initializeSignals(void) {
	sigemptyset(&act_TSTP.sa_mask);
	act_TSTP.sa_flags = SA_SIGINFO;
	act_TSTP.sa_sigaction = TSTP_handler;

	if (sigaction(SIGTSTP, &act_TSTP, &oldact_TSTP) == -1)
		fprintf(stderr, "\n\tErro ao redirecionar SIGTSTP."); 


	sigemptyset(&act_CHLD.sa_mask);
	act_CHLD.sa_flags = SA_SIGINFO;
	act_CHLD.sa_sigaction = CHLD_handler;

	if (sigaction(SIGCHLD, &act_CHLD, &oldact_CHLD) == -1)
		fprintf(stderr, "Erro ao redirecionar SIGCHLD."); 
}


/**
 * Redireciona sinais do processo filho.
 */

void initializeChildrenSetup(void) {
	sigaction(SIGTSTP, &oldact_TSTP, NULL);
	sigaction(SIGCHLD, &oldact_CHLD, NULL);
}

