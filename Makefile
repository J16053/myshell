
myshell: myshell.o files.o
	g++ myshell.o files.o -o myshell

myshell.o: myshell.cpp
	g++ -c myshell.cpp

files.o: files.cpp files.hpp
	g++ -c files.cpp

clean:
	rm *.o myshell
