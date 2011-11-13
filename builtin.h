#ifndef BUILTIN_H
#define BUILTIN_H

int builtInCommand(char **argv);
void my_exit(void);
int cd(char **argv);
int printJobs(char **argv);
int bg(char **argv);
int fg(char **argv);

#endif
