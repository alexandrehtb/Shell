#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 512

/**
 * Le uma linha de stream e retorna um ponteiro para a string.
 * Em caso de erro exibe mensagem e aborta o programa.
 */

char *readLine(FILE *stream) {
	char *string = malloc(LINE_LENGTH);
	unsigned int i = 0, length = LINE_LENGTH;
	int input = 0;

	while ((input = fgetc(stream)) != '\n') {
		if (input == EOF) {
			fprintf(stderr, "\nErro de leitura. Abortando.\n");
			exit(EXIT_FAILURE);
		}

		if (i + 1 >= length) {
			length *= 2;
			string = realloc(string, length);
		}
		string[i++] = input;
	}
	string[i] = '\0';
	return realloc(string, i + 1);
}

/**
 * Libera o espaco ocupado pelo comando gerado por uma chamada
 * previa do parser().
 */

void freeCommand(char *argv[]) {
	unsigned int i = 0;

	while (argv[i] != NULL) {
		free(argv[i]);
		i++;
	}
//	free(argv);
}

/**
 * Divide a string separado por " " em tokens e retorna um ponteiro para o
 * endereco do primeiro token de um array de tokens.
 */

char **parser(char *string) {
	char **argv = malloc(((strlen(string) /2 ) + 2) * sizeof (char *)),
	     *token = strtok(string, " ");
	unsigned int i = 0;

	while (token) {
		unsigned int len = strlen(token);
		argv[i] = malloc(len + 1);
		strcpy(argv[i], token);
		token = strtok(NULL, " ");
		i++;
	}
	argv[i] = NULL;
	return realloc(argv, (i + 1) * sizeof (char *));
}
