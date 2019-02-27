#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <iostream>
#include "Variable.hpp"
using namespace std;

class List {
	Variable * head;
public:
	List() { head = NULL; }
	void add(char * v);
	void display();
	char * getHeadValue();
	char * getVal(char * name);
};
#endif
