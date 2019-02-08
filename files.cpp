#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>
#include "files.hpp"

using namespace std;

void displayHistory(char * path) {
	ifstream history;
	string filename(path);
	filename +=  "/history.txt";
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

void setPath(char * path, char * value) {
	ofstream variables;
	string filename(path);
	filename += "/export.txt";
	variables.open(filename.c_str());
	if (variables.is_open()) {
		variables << "PATH=" << value << endl;
		variables.close();
	} else {
		cerr << "Error opening the file" << endl;
	}
}

void displayVariables(char * path) {
	ifstream variables;
	string filename(path);
	filename += "/export.txt";
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

void exportVariable(char * path, char * input) {
	char * current = path;
	string varName = "";
	while (*current != '\0' && *current != '=') {
		varName += *current;
		current++;
	}
	fstream variables;
	string filename(path);
	filename += "/export.txt";
	variables.open(filename.c_str()); // should make a... dictionary?
}

bool updateHistory(char command[], char * path) {
	ofstream history;
	string filename(path);
	filename += "/history.txt";
	history.open(filename.c_str(), ios::app);
	if (history.is_open()) {
		history << command << endl;
		history.close();
		return true;
	} else {
		cerr << "Error opening the file" << endl;
		return false;
	}
}

void historyLookup(char * input, char * path) {
	bool valid = true;
	char * current = &input[1];
	// check if input is of form: !(number)
	while (isspace(*current) == 0 && *current != '\0' && valid == true) {
		if (isdigit(*current) == 0) {
			valid = false;
		}
		current++;
	}
	if (valid) {
		ifstream history;
		string filename(path);
		filename += "/history.txt";
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
				printf("%s\n", input);
			} else {
				cout << "No such line in history" << endl;
			}
			history.close();
		} else {
			cerr << "Error opening the file" << endl;
		}
	}
}
