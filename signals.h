#ifndef SIG_H
#define SIG_H

#include <signal.h>

void TSTP_handler(int sig, siginfo_t *info, void *context);
void CHLD_handler(int sig, siginfo_t *info, void *context);
void initializeSignals(void);
void initializeChildrenSetup(void);

#endif

