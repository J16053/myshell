#include <cstdlib> // standard library
#include <iostream> // input/output
#include <cstring> // strcmp()
#include <cerrno> // errno
#include <unistd.h> // system calls
#include <cstdio> // remove()
#include "History.hpp"
#include "List.hpp"

using namespace std;
const int MAXINPUT = 256; // max number of characters in array
const int MAXARGS = 50; // max number of separate arguments

int main() {	
	char input[MAXINPUT]; // stores user input
	char * argv[MAXARGS]; // stores individual arguments
	List env = List(); // stores list of exported variables
	char nullpath[] = "PATH=";
	env.add(nullpath); // export PATH variable
	const char * startDir = get_current_dir_name();
	if (startDir == NULL) {
		cout << "Error: could not get starting directory" << endl;
		exit(1);
	}
	string cwd("PWD=");
	cwd += startDir;
	env.add(&cwd[0]); // export PWD variable
	while (true) {
		cout << ">> ";
		int argc = 0; // number of arguments
		cin.getline(input, MAXINPUT); // reads user input into input, '\0' terminated
		if (input[0] == '!') {
			historyLookup(startDir, input);
		}
		updateHistory(startDir, input); // adds inputted command to history
		char * arg; // temporary variable to store individual arguments
		arg = strtok(input, " "); // splits input at space
		while (arg != NULL && argc < MAXARGS) {
			argv[argc] = arg; // argv is an array of pointers to arguments
			argc++;
			arg = strtok(NULL, " ");
		}
		if (strcmp(argv[0], "pwd") == 0) { // print working directory
			cout << get_current_dir_name() << endl;
		} else if (strcmp(argv[0], "cd") == 0) { // change directories
			if (argc > 1 && chdir(argv[1]) == -1) {	// chdir returns -1 when unsuccessful and stores error in errno
				cout << strerror(errno) << endl; // can also consider using perror
			} else if (argc == 1) { // no directory given
				cerr << "No directory specified\nUsage: cd [directory]" << endl;
			} else { // cd successful, update PWD variable
				cwd = (string)"PWD=" + get_current_dir_name();
				env.add(&cwd[0]);
			}
		} else if (strcmp(argv[0], "export") == 0) { // export
			if (argc == 1) {
				env.display();
			} else {
				env.add(strdup(argv[1]));
			} 
		} else if (strcmp(argv[0], "history") == 0) { // history
			displayHistory(startDir);
		} else if (strcmp(argv[0], "exit") == 0) { // exit
			exit(0);
		} else { // not built-in command
			// see if command exists in PATH-specified directories, if yes display full path and args
			bool found = false;
			string varname = "PATH";
			char * PATH = env.getHeadValue();
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
