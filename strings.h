#ifndef STRINGS_H
#define STRINGS_H

char *readLine(FILE *stream);
void freeCommand(char *argv[]);
char **parser(char *strings);

#endif
