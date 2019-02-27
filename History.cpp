#include <cstdlib> // standard library
#include <iostream> // input/output
#include <iomanip> // setw()
#include <fstream> // files
#include <cctype> // isdigit()
#include "History.hpp"

using namespace std;

/**
 * Prints the contents of file specified by filename, with or without line numbers
 * string filename contains the path to the file to be printed
 * bool linenumbers is true if line numbers are to be printed, false otherwise
 */

static void print(string filename, bool linenumbers) {
	ifstream file;
	file.open(filename.c_str());
	if (file.is_open()) {
		int lineno = 1;
		string line;
		while (getline(file, line)) {
			if (linenumbers) {
				cout << setw(5) << right << lineno++ << "  " << line << endl;
			} else {
				cout << line << endl;
			}
		}
		file.close();
	} else {
		cerr << "Error opening " << filename << endl;
	}
}

/**
 * Prints the contents of /.history.txt to the console with line numbers
 * char * path contains the path to the directory containing /.history.txt
 */

void displayHistory(const char * path) {
	string filename(path);
	filename +=  "/.history.txt";
	print(filename, true);
}

/**
 * Adds command to /.history.txt
 * char * input contains the string to add to the file
 * char * path is the path to the directory in which /.history.txt is stored
 */

void updateHistory(const char * path, char * input) {
	ofstream history;
	string filename(path);
	filename += "/.history.txt";
	history.open(filename.c_str(), ios::app);
	if (history.is_open()) {
		history << input << endl;
		history.close();
	} else {
		cerr << "Error opening " << filename << endl;
	}
}

/**
 * Checks if input is of form: ![0-9]+
 * char * input contains the string to be checked
 * Returns true if input is valid, false otherwise
 */

static bool validHistoryInput(char * input) {
	if (input[0] != '!') return false;	
	char * current = &input[1];
	while (isspace(*current) == 0 && *current != '\0') {
		// all characters between ! and first whitespace should be digits
		if (isdigit(*current) == 0) {
			return false;
		}
		current++;
	}
	return true;
}

/**
 * Looks for the corresponding line in /.history.txt and stores the command at
 * the specified line number in input
 * char * input contains the line number to be found
 * char * path contains the path to the directory containing /.history.txt
 */

void historyLookup(const char * path, char * input) {
	if (validHistoryInput(input)) {
		ifstream history;
		string filename(path);
		filename += "/.history.txt";
		history.open(filename.c_str());
		int lineno = atoi(&input[1]);
		if (history.is_open()) {
			int count = 1;
			string line;
			while (getline(history, line) && count < lineno) {
				count++;
			}
			if (count == lineno) {
				int i = 0;
				while ( i < line.length()) {
					input[i] = line.at(i);
					i++;
				}
				input[i] = '\0';
			} else {
				cout << "No such line in history" << endl;
			}
			history.close();
		} else {		
			cerr << "Error opening " << filename << endl;
		}
	}
}
