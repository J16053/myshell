#include <iostream>
#include <cstring>
#include "List.hpp"
using namespace std;

/*
 * Given a char* with format "name=value",
 * add a new Variable to the list
 * or update the existing value of a Variable with the same name
 */

void List::add(char* v) {
	Variable* temp = new Variable(v);
	if (head == NULL) {
		head = temp;
		size++;
	} else {
		Variable* current = head;
		while(current->next != NULL && current->name.compare(temp->name)) {
			current = current->next;
		}
		if (!current->name.compare(temp->name)) {
			current->value = temp->value;
		}
		else {
			current->next = temp;
			size++;
		}
	}
}

/* 
 * Prints all names and values of Variables in List
 */

void List::display() {
	Variable* current = head;
	while (current != NULL) {
		cout << current->toString() << endl;
		current = current->next;
	}
}

/* 
 * Gets value of head variable (useful for PATH variable)
 * return char* value
 */

char* List::getHeadValue() {
	return strdup(head->value.c_str());
}

/* 
 * Gets value of Variable with matching name
 * param char* name to search for
 * return char* value or NULL if no variable matches
 */

char* List::getVal(char* name) {
	Variable* current = head;
	while (current != NULL) {
		if (!strcmp(current->name.c_str(), name)) {
			return strdup(current->value.c_str());
		}
		current = current->next;
	}
	return NULL;
}

/* 
 * Populates env with toStrings of Variables terminated by NULL
 * param char** env is the array of strings to populate
 * and it should be at least equal to size+1
 */

void List::getEnv(char** env) {
	Variable* current = head;
	for (int i = 0; i < size; i++) {
		env[i] = new char[current->toString().size()]; // make sure to delete
		strcpy(env[i], current->toString().c_str());
		current = current->next;
	}
	env[size] = NULL;
}

