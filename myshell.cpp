#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <cstdio>
#include "files.hpp"

using namespace std;
const int MAXINPUT = 256; // max number of characters in array 
const int MAXARGS = 3; // will have at most 3 arguments according to project description which I may have misunderstood

int main() {	
	char input[MAXINPUT];
	char * argv[MAXARGS];
	char * startDir = NULL;
	startDir = getcwd(startDir, MAXINPUT); 
	char nullpath[] = "PATH=";
	exportVariable(startDir, nullpath);
	if (startDir == NULL) {
		cout << "something's wrong startDir is NULL" << endl;
		exit(0);
	}
	while (true) {
		cout << ">> ";
		int argc = 0; // number of arguments
		cin.getline(input, MAXINPUT); // reads user input into input, '\0' terminated
		if (input[0] == '!') {
			historyLookup(input, startDir);
		}
		updateHistory(input, startDir); // adds inputted command to history
		char * arg; // temporary variable to store individual arguments
		arg = strtok(input, " "); // splits input at space
		while (arg != NULL && argc < MAXARGS) {
			argv[argc] = arg; // argv is an array of pointers to arguments
			argc++;
			arg = strtok(NULL, " ");
		}
		if (strcmp(argv[0], "pwd") == 0) {
			char * name = NULL;
			name = getcwd(name, MAXINPUT); // copies current directory path to *name
			if (name != NULL) {
				cout << name << endl;
			} else {
				cout << strerror(errno) << endl;
			}
		} else if (strcmp(argv[0], "cd") == 0) {
			if (chdir(argv[1]) == -1) {	// chdir returns -1 when unsuccessful and stores error in errno
				cout << strerror(errno) << endl;
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
			string exportFilename(startDir);
			exportFilename += "/.export.txt";
			remove(exportFilename.c_str());
			exit(0);
		} else {
			// see if command exists in PATH-specified directories, if yes display full path and args
			bool found = false;
			char * PATH = getPath(startDir); // contains value of exported path variable
			char slash[] = "/";
			char * path; // will hold individual path to search
			path = strtok(PATH, ":");
			while (path != NULL && !found) {
				string searchpath = path;
				searchpath += "/";
				searchpath += argv[0];
				if (access(searchpath.c_str(), X_OK) == 0) {
					cout << argv[0] << " is an external command (" << searchpath << ")" << endl;
					cout << "command arguments:" << endl;
					for (int i = 1; i < argc; i++) {
						cout << argv[i] << endl;
					}
					found = true;
				}
				path = strtok(NULL, ":");
			}
			if (!found) cout << "command not found" << endl;
		}
	}
	return 0;
} 
