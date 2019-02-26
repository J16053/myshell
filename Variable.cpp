#include <string>
#include "Variable.hpp"
using namespace std;

Variable::Variable(char * v, Variable * next) {
	size_t n = 0;
	char * current = v;
	while (*current != '\0') {
		if (*current == '=') break;
		current++;
		n++;
	}
	this->name.assign(v, n);
	if (*current != '\0')
		this->value.assign(current+1); 
	else this->value.assign(current);
	this->next = next;
}
string Variable::toString() {
	return name + "=" + value;
}
