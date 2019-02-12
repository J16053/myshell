# myshell
A Linux shell for my Operating Systems class

Implemented built-in commands:

- pwd - print name of current/working directory.
- cd - change the current directory.
- export - names of exported shell variables.
- history - display the command history list with line numbers.
- !num - execute the command located at line num in history.
- exit - free all space allocated gracefully and exit the application.

For non-built-in commands, the program searches the directory system in the order 
specified by the PATH variable for a file with the same name as the first identifier 
and displays the full path of the command and argument(s) passed.

Usage
------
Compile the code in terminal using make:
```
$ make
```
Then run the executable myshell file:
```
$ ./myshell
```
A prompt will appear that looks like this:
```
>>
```
Simply type a command into the prompt and hit enter. Arguments are space separated. 
For example, the following command will export the PATH variable with a value of /bin/bash:
```
>> export PATH=/bin/bash
```
