#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>
#include "files.hpp"

using namespace std;

/**
 * Prints the contents of /.history.txt to the console with line numbers
 * char * path contains the path to the directory containing /.history.txt
 */

void displayHistory(char * path) {
	ifstream history;
	string filename(path);
	filename +=  "/.history.txt";
	history.open(filename.c_str());
	if (history.is_open()) {
		int lineno = 1;
		string line;
		while (getline(history, line)) {
			cout << setw(5) << right << lineno++ << "  " << line << endl;
		}
		history.close();
	} else {
		cerr << "Error opening the file" << endl;
	}
}

/**
 * Prints the contents of /.export.txt to the console
 * char * path contains the path to the directory containing /.export.txt
 */

void displayVariables(char * path) {
	ifstream variables;
	string filename(path);
	filename += "/.export.txt";
	variables.open(filename.c_str());
	if (variables.is_open()) {
		string line;
		while (getline(variables, line)) {
			cout << line << endl;
		}
		variables.close();
	} else {
		cerr << "Error opening the file" << endl;
	}
}

char * getPath(char * path) {
	ifstream file;
	string filename(path);
	filename += "/.export.txt";
	file.open(filename.c_str());
	string PATH;
	if (file.is_open()) {
		string line;
		getline(file, line);
		PATH = line.substr(5, string::npos);
		file.close();
	} else {
		cerr << "Error opening the file" << endl;
	}
	return &PATH[0];
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
		cerr << "Error opening the file" << endl;
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
		cerr << "Error opening the file" << endl;
	}
}

/**
 * Checks if input is of form: ![0-9]+
 * char * input contains the string to be checked
 * Returns true if input is valid, false otherwise
 */

bool validHistoryInput(char * input) {
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
			cerr << "Error opening the file" << endl;
		}
	}
}
