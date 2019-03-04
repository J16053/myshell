#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <iostream>
#include "Variable.hpp"
using namespace std;

class List {
private:	
	Variable* head;
public:
	int size;
public:
	List() { head = NULL; size = 0; }
	void add(char* v);
	void display();
	char* getHeadValue();
	char* getVal(char* name);
	void getEnv(char** env);
};
#endif

