#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 512

/* ----------------------------------------------- */
/* LE UMA LINHA DA ENTRADA ESPECIFICADA EM *stream */
/* ----------------------------------------------- */
char *readLine(FILE *stream)
{
	char *string = malloc(LINE_LENGTH);
	unsigned int i = 0, length = LINE_LENGTH;
	int input = 0;

	while ((input = fgetc(stream)) != '\n') {
		if (input == EOF) {
			fprintf(stderr, "\n\nErro de leitura. Abortando.");
			exit(EXIT_FAILURE);
		}

		if (i + 1 >= length) {
			length *= 2;
			string = realloc(string, length); // dobra o tamanho maximo da string
		}

		string[i++] = input; // adiciona o caracter a string
	}

		string[i] = '\0';
		return realloc(string, i + 1); // realoca para o tamanho correto e retorna
}

/* ------------------------------------------------------------- */
/* SEPARA A LINHA DE COMANDO LIDA EM TOKENS SEPARADOS POR ESPACO */
/* ------------------------------------------------------------- */
char **parser(char *string) {
	char **args = malloc(((strlen(string) / 2) + 2) * sizeof (char *)),
	     *token = strtok(string, " ");
	unsigned int i = 0;

	while (token) {
		unsigned int len = strlen(token);
		args[i] = malloc(len + 1);
		strcpy(args[i], token);
		token = strtok(NULL, " ");
		i++;
	}

	args[i] = NULL;
	return realloc(args, (i + 1) * sizeof (char *));
}

