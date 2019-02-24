#include <cstdlib> // standard library
#include <cstddef> // size_t
#include <iostream> // input/output
#include <iomanip> // setw()
#include <fstream> // files
#include <cstring> // strcat(), strcmp()
#include <cctype> // isdigit()
#include "files.hpp"

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

void displayHistory(char * path) {
	string filename(path);
	filename +=  "/.history.txt";
	print(filename, true);
}

/**
 * Prints the contents of /.export.txt to the console
 * char * path contains the path to the directory containing /.export.txt
 */

void displayVariables(char * path) {
	string filename(path);
	filename += "/.export.txt";
	print(filename, false);
}

/**
 * Gets value of variable specified by var from /.export.txt
 * char * path contains the path to the directory containing /.export.txt
 * char * var contains the variable name to be fetched
 * Returns a pointer to the value of the variable
 */

char * getVariable(char * path, char * var) {
	ifstream file;
	string filename(path);
	filename += "/.export.txt";
	file.open(filename.c_str());
	string value;
	if (file.is_open()) {
		string line;
		getline(file, line);
		char equals[] = "=";
		strcat(var, equals);
		if (strcmp(var, line.substr(0, strlen(var)).c_str()) == 0) {
			value = line.substr(strlen(var), string::npos);
		}
		file.close();
	} else {
		cerr << "Error opening " << filename << endl;
	}
	return &value[0];
}

/**
 * Exports a variable to /.export.txt
 * char * path is the path to the directory in which /.export.txt is stored
 * char * input is the variable and value to be stored in /.export.txt
 * If the variable already exists, its value will be updated
 * Otherwise, the variable and its value will be added to /.export.txt
 */

void exportVariable(char * path, char * input) {
	char * current = input;
	string newVarName = "";
	while (*current != '\0' && *current != '=') {
		newVarName += *current;
		current++;
	}
	// open file streams for reading and writing
	ifstream readVariables;
	ofstream writeVariables;
	string filename(path);	
	string tempfilename = filename + "/.temp.txt";
	filename += "/.export.txt";
	readVariables.open(filename.c_str());
	if (readVariables.fail()) {
		writeVariables.open(filename.c_str());
		writeVariables << input << endl;
		writeVariables.close();
	} else {
		writeVariables.open(tempfilename.c_str());
		if (readVariables.is_open() && writeVariables.is_open()) {
			string line;
			bool existing = false;
			while (getline(readVariables, line)) {
				size_t equals = line.find_first_of("=");
				string oldVarName = line.substr(0, equals);
				if (newVarName == oldVarName) {
					// update existing exported variable
					writeVariables << input << endl;
					existing = true;
				} else {
					writeVariables << line << endl;
				}
			}
			if (!existing) {
				// append new exported variable to file
				writeVariables << input << endl;
			} 
			readVariables.close();
			writeVariables.close();
			// replace old file with newly written file
			remove(filename.c_str());
			rename(tempfilename.c_str(), filename.c_str());
		} else {
			cerr << "Error opening " << filename << endl;
		}
	}
}

/**
 * Adds command to /.history.txt
 * char * input contains the string to add to the file
 * char * path is the path to the directory in which /.history.txt is stored
 */

void updateHistory(char * input, char * path) {
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

void historyLookup(char * input, char * path) {
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
