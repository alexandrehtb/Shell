#ifndef SIGNALS_H
#define SIGNALS_H

#include <sys/wait.h>

void initializeSignals(void);
void TSTP_Handler(int signal, siginfo_t *info, void *context);
void CHLD_Handler(int signal, siginfo_t *info, void *context);

#endif
