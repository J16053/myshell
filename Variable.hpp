#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED
#include <string>
using namespace std;
class Variable {
	// member attributes
        string name;
        string value;
        Variable* next;
	// constructor
        Variable(char* v, Variable* next = NULL); 
        string toString();
        friend class List;
};

#endif

