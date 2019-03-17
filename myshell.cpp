#include <cstdlib> // standard library
#include <iostream> // input/output
#include <cstring> // strcmp()
#include <cerrno> // errno
#include <unistd.h> // system calls
#include <cstdio> // remove()
#include <sys/wait.h> // wait()
#include <fcntl.h> // open()
#include "History.hpp"
#include "List.hpp"

#define MAXINPUT 256 // max number of characters in array
#define MAXARGS 50 // max number of separate arguments
#define READ_END 0
#define WRITE_END 1
#define STDIN 0
#define STDOUT 1
#define STDERR 2

using namespace std;

void expandVariables(int argc, char** argv, List* env);
bool redirectIO(int argc, char** argv, List* env, const char* startDir, string& cwd);
void execute(int argc, char** argv, List* env, const char* startDir, string& cwd);

int main() {	
	char input[MAXINPUT]; // stores user input
	char* argv[MAXARGS]; // stores individual arguments, NULL-terminated
	List env = List(); // stores list of exported variables
	char nullpath[] = "PATH=";
	env.add(nullpath); // export PATH variable
	const char* startDir = get_current_dir_name();
	if (startDir == NULL) {
		cout << "Error: could not get starting directory" << endl;
		exit(1);
	}
	string cwd("PWD=");
	cwd += startDir;
	env.add(&cwd[0]); // export PWD variable
	while (true) {
		bool original = true; // false if process is forked child
		
		// print prompt
		cout << ">> ";
		
		// read input
		cin.getline(input, MAXINPUT); // reads user input into input, '\0' terminated
		if (input[0] == '!') {
			historyLookup(startDir, input);
		}

		// add inputted command to history
		updateHistory(startDir, input);
		
		// parse input into space-separated tokens stored in argv
		int argc = 0; // number of arguments
		char* arg; // temporary variable to store individual arguments
		arg = strtok(input, " "); // splits input at space
		while (arg != NULL && argc < MAXARGS) {
			argv[argc] = arg; // argv is an array of pointers to arguments
			argc++;
			arg = strtok(NULL, " ");
		}
		// argv is NULL-terminated
		argv[argc] = NULL;

		// expand shell variables $
		expandVariables(argc, argv, &env);

		// parse pipes
		bool end = false; 		
		int i = 0;
		int p[2]; // pipe
		int stdout_copy = dup(STDOUT);
		pid_t pid; // process ID
		while (!end) {
			if (strcmp(argv[i], "|") == 0 && i+1 < argc) {
				if (pipe(p) == -1) {
					cerr << "Pipe failed: " << strerror(errno) << endl;
					exit(1);
				}
				pid = fork();
				if (pid == -1) {
					cerr << "Fork failed" << endl;
					exit(1);
				} else if (pid == 0) { // child process
					original = false;
					close(p[WRITE_END]);
					close(STDIN);
					dup2(p[READ_END], STDIN);

					// shift argv structure for sub-command
					for (int j = 0; j+i < argc; j++) {
						argv[j] = argv[j+i+1];
					}
					argc -= i+1;
					i = 0;
				} else { // parent process
					close(p[READ_END]);
					close(STDOUT);
					dup2(p[WRITE_END], STDOUT);
					argv[i] = NULL;
					argc = i;
					if (!redirectIO(argc, argv, &env, startDir, cwd)) {
						execute(argc, argv, &env, startDir, cwd);
					}
					close(p[WRITE_END]);
					close(STDOUT);
					wait(NULL);
					dup2(stdout_copy, STDOUT);
					break;
				}
			}
			i++;
			if (i == argc) end = true;
		}
		
		// no pipes in command
		if (end && !redirectIO(argc, argv, &env, startDir, cwd)) {
			execute(argc, argv, &env, startDir, cwd);
		}

		if (!original) {
			exit(0); // only original process should loop
		}
	}
	return 0;
}

/**
 * Expand shell variable names (starting with $)
 * to corresponding values in place in argv structure
 */
void expandVariables(int argc, char** argv, List* env) {
	for (int i = 0; i < argc; i++) {
		if (argv[i][0] == '$' && argv[i][1] != '\0') {
			char* val = (*env).getVal(&argv[i][1]);
			if (val != NULL) {
				argv[i] = val;
			}
		}
	}
}

/**
 * Redirects input/output if command contains <, >, or 2>
 * Forks a child process which executes the command with redirected IO
 * Returns true if redirect needed (and therefore command executed)
 * Returns false if no redirect (command not yet executed)
 */
bool redirectIO(int argc, char** argv, List* env, const char* startDir, string& cwd) {
	char* infile = NULL;
	char* outfile = NULL;
	char* errfile = NULL;
	
	// parse < > 2>
	for (int i = 0; i < argc; i++) {	
		if ((strcmp(argv[i], "<") == 0 || strcmp(argv[i], ">") == 0 || strcmp(argv[i], "2>") == 0) && i+1 < argc) {
			if (strcmp(argv[i], "<") == 0) {
				infile = argv[i+1];
			} else if (strcmp(argv[i], ">") == 0) {
				outfile = argv[i+1];
			} else if (strcmp(argv[i], "2>") == 0) {
				errfile = argv[i+1];
			} else cerr << "what?" << endl;
			for (int j = i; j+2 <= argc; j++) {
				argv[j] = argv[j+2];
			} 
			argc -= 2;
			if (argv[i] != NULL) i--;
		}
	}

	// redirect input/output
	if (infile != NULL || outfile != NULL || errfile != NULL) {
		int pid = fork();
		if (pid == -1) {
			cerr << "Fork failed" << endl;
			exit(1);
		} else if (pid == 0) {
			if (infile != NULL) { // redirect STDIN
				close(STDIN);
				if (open(infile, O_RDONLY)) cerr << "Input file open failed" << endl;
			}
			if (outfile != NULL) { // redirect STDOUT
				close(STDOUT);
				if (open(outfile, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH) != STDOUT) cerr << "Output file open failed" << endl;
			}
			if (errfile != NULL) { // redirect STDERR
				close(STDERR);
				if (open(errfile, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH) != STDERR) cerr << "Error file open failed" << endl;
			}
			execute(argc, argv, env, startDir, cwd);
			exit(0);
		} else {
			wait(NULL);
		}
		return true;
	} else return false; // nothing redirected
}

/**
 * Executes command stored in argv
 */
void execute(int argc, char** argv, List* env, const char* startDir, string& cwd) {
	if (strcmp(argv[0], "pwd") == 0) { // print working directory
		cout << get_current_dir_name() << endl;
	} else if (strcmp(argv[0], "cd") == 0) { // change directories
		if (argc > 1 && chdir(argv[1]) == -1) {	// chdir returns -1 when unsuccessful and stores error in errno
			cerr << strerror(errno) << endl; // can also consider using perror
		} else if (argc == 1) { // no directory given
			cerr << "No directory specified\nUsage: cd [directory]" << endl;
		} else { // cd successful, update PWD variable
			cwd = (string)"PWD=" + get_current_dir_name();
			(*env).add(&cwd[0]);
		}
	} else if (strcmp(argv[0], "export") == 0) { // export
		if (argc == 1) {
			(*env).display();
		} else {
			(*env).add(strdup(argv[1]));
		} 
	} else if (strcmp(argv[0], "history") == 0) { // history
		displayHistory(startDir);
	} else if (strcmp(argv[0], "exit") == 0) { // exit
		exit(0);
	} else { // not built-in command
		// see if command exists in PATH-specified directories, if yes display full path and args
		char* envp[(*env).getSize()+1];
		(*env).getEnv(envp);
		bool found = false;
		if (argv[0][0] == '/' && access(argv[0], X_OK) == 0) { // absolute path	
			if (fork() == 0) {
				execvpe(argv[0], argv, envp);
			} else {
				wait(NULL);
			}
			found = true;
		} else if (strstr(argv[0], "/") != NULL) { // relative path
			string searchpath = (string)get_current_dir_name() + "/" + argv[0];
			if (access(searchpath.c_str(), X_OK) == 0) {
				if (fork() == 0) {
					execvpe(argv[0], argv, envp);
				} else {
					wait(NULL);
				}
				found = true;
			}
		} else { // look in PATH variable
			char* PATH = (*env).getHeadValue();
			char slash[] = "/";
			char* path; // will hold individual path to search
			path = strtok(PATH, ":");
			while (path != NULL && !found) {
				string searchpath = (string)path + "/" + argv[0];
				if (access(searchpath.c_str(), X_OK) == 0) {	
					argv[0] = &searchpath[0];
					if (fork() == 0) {
						execvpe(argv[0], argv, envp);
					} else {
						wait(NULL);
					}
					found = true;
				}
				path = strtok(NULL, ":");
			}	
		}
		for (int i = 0; i < (*env).getSize(); i++) {
			delete[] envp[i];
		}
		if (!found) cerr << "command not found" << endl;
	}
}

