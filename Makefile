myshell: myshell.o History.o List.o Variable.o
	g++ myshell.o History.o List.o Variable.o -o myshell

myshell.o: myshell.cpp
	g++ -c myshell.cpp

History.o: History.cpp History.hpp
	g++ -c History.cpp

List.o: List.cpp List.hpp
	g++ -c List.cpp

Variable.o: Variable.cpp Variable.hpp
	g++ -c Variable.cpp

clean:
	rm *.o myshell
