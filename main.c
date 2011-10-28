#include "strings.h"
#include "processes.h"

#define TRUE 1

int main(void)
{
	while(TRUE) {
		char *line = readLine(stdin),
		     **command = parser(line);

		launchJob(command, NULL, 0, 0);
	}

	return 0;
}
