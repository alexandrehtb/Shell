#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdio.h>

void executeCommand(char *command[], char *file, int newDescrciptor, int executionMode);
void launchJob(char *command[], char *file, int newDescriptor, int executionMode);

#endif
