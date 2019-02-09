#include <iostream> // may be redundant - consider changing couts to printfs
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include "files.hpp"

using namespace std;
const int MAXINPUT = 256; // number of characters until command prompt stops reading 
const int MAXARGS = 3; // will have at most 3 arguments according to project description which I may have misunderstood

int main() {	
	char input[MAXINPUT];
	char * argv[MAXARGS];
	char * startDir = NULL;
	string path = "";
	startDir = getcwd(startDir, MAXINPUT); 
	// char nullpath[] = "PATH=";
	// exportVariable(startDir, nullpath);
	if (startDir == NULL) {
		cout << "something's wrong startDir is NULL" << endl;
		exit(0);
	}
	while (true) {
		cout << ">> ";
		int argc = 0; // number of arguments
		cin.getline(input, MAXINPUT); // '\0' at end of input
		if (input[0] == '!') {
			historyLookup(input, startDir);
		}
		updateHistory(input, startDir);
		char * arg; // temporary variable
		arg = strtok(input, " ");
		while (arg != NULL) {
			argv[argc] = arg; // argv will just contain pointers
			argc++;
			arg = strtok(NULL, " ");
		}	
		if (strcmp(argv[0], "pwd") == 0) {
			char * name = NULL;
			name = getcwd(name, MAXINPUT); // copies current startDir to *name
			if (name != NULL) {
				printf("%s\n", name);
			} else {
				printf("Error:  %s", name);
			}
			delete name;
		} else if (strcmp(argv[0], "cd") == 0) {
			if (chdir(argv[1]) == -1) {	// chdir returns -1 when unsuccessful
				printf("%s\n", strerror(errno)); // print error
			}
		} else if (strcmp(argv[0], "export") == 0) {
			if (argc == 1) {
				displayVariables(startDir);
			} else {
				exportVariable(startDir, argv[1]);
			} 
		} else if (strcmp(argv[0], "history") == 0) {
			displayHistory(startDir);
		} else if (strcmp(argv[0], "exit") == 0) {
			exit(0);
		} else {
			// see if command exists, if yes display full startDir
			cout << "command not found" << endl;
		}
	}
	// no need for deletes - tell me, was any memory dynamically allocated?
	// however, I could consider dynamically allocating input, that would potentially be smart
	return 0;
}

