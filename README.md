# Simple Shell
This is a simple shell written in C. It can be used to run commands and scripts.

# Features
Run commands and scripts
Handle pipes and redirection
Create child processes
Wait for child processes to finish
# How to use
To run the shell, simply type ./simple_shell in a terminal. The shell will then prompt you for a command. Type the command and press Enter. The shell will execute the command and print the output.

# Example
The following example shows how to run the ls command:

$ ./simple_shell 
ls
bin  dev  home  lib32  lib64  media  opt  proc  root  run  sbin  snap usr

# Commands
The following table lists the commands that are supported by the shell:

# Command	Description
echo:	Print text.
kill:	Kill a job.
ls:	List files and directories.
pwd:	Print working directory.
sleep:	Sleep for a specified amount of time.
test:	Evaluate a condition.
ctrl + D: to exit the program
";" after the command to run more than one command
&&,|| : to choose to do "and" operator or "or" operator

# Scripts
You can also run scripts from the shell. A script is a file that contains a series of commands. To run a script, type the following command:

./simple_shell script.sh

# Bugs
This is a work in progress, so there may be some bugs. If you find any bugs, please report them to me.
