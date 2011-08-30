#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define SHELL_NAME shell
#define MAX 100

int main()
{
	string str_buffer;

	getline(cin, str_buffer);
	while (str_buffer != "exit") {
		vector<string> str_vector;
		str_buffer += " NULL ";
		while (str_buffer.length() != 0) {
			str_vector.push_back(str_buffer.substr(0, str_buffer.find_first_of(' ')));
			str_buffer.erase(0, str_buffer.find_first_of(' ') + 1);
		}
		cin >> str_buffer;
	}

	return 0;
}
