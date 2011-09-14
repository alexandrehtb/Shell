#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h> // realloc()
using namespace std;

#define SHELL_NAME shell
#define MAX 100

int main()
{
	string str_buffer, tmp;
	char **args = NULL;

	getline(cin, str_buffer);
	while (str_buffer != "exit") {
		str_buffer += " NULL ";

		int i = 0;
		while (str_buffer.length() != 0) {
			args = (char **) realloc(args, (i + 1)*(sizeof(char *)));
			tmp = str_buffer.substr(0, str_buffer.find_first_of(' '));
			args[i] = new char [str_buffer.size() + 1];
			strcpy(args[i], tmp.c_str());
			str_buffer.erase(0, str_buffer.find_first_of(' ') + 1);
			i++;
		}
		
		cin >> str_buffer;
	}

	return 0;
}
