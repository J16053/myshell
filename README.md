# myshell
A Linux shell for my Operating Systems class

Implemented built-in commands:

- pwd - print name of current/working directory.
- cd [directory] - change the current directory to the one specified by [directory].
- export [variable] - list names of exported shell variables if [variable] is not given, or export [variable].
- history - display the command history list with line numbers.
- !num - execute the command located at line num in history.
- exit - free all space allocated gracefully and exit the application.

For non-built-in commands, the program searches the directory system in the order 
specified by the PATH variable for a file with the same name as the first identifier 
and executes the command.

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

This shell can also parse the following special symbols:

- | pipe connects the standard output of one command to the standard output of another
- < [filename] redirects input to the file specified by [filename] instead of stdin
- > [filename] redirects output to the file specified by [filename] instead of stdout
- >2 [filename] redirects stderr to the file specified by [filename]
- $[varname] is replaced by its value if [varname] refers to an exported shell variable
