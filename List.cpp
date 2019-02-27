#include <iostream>
#include <cstring>
#include "List.hpp"
using namespace std;

void List::add(char * v) {
	Variable * temp = new Variable(v);
	if (head == NULL) {
		head = temp;
	} else {
		Variable * current = head;
		while(current->next != NULL && current->name.compare(temp->name)) {
			current = current->next;
		}
		if (!current->name.compare(temp->name)) current->value = temp->value;
		else current->next = temp;
	}
}

void List::display() {
	Variable * current = head;
	while (current != NULL) {
		cout << current->toString() << endl;
		current = current->next;
	}
}

char * List::getHeadValue() {
	return strdup(head->value.c_str());
}

char * List::getVal(char * name) {
	Variable * current = head;
	while (current != NULL) {
		if (!strcmp(current->name.c_str(), name)) {
			return strdup(current->value.c_str());
		}
		current = current->next;
	}
	return NULL;
}
