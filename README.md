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
cd:	Change directory.
echo:	Print text.
exit:	Exit the shell.
help:	Display help information.
jobs:	List running jobs.
kill:	Kill a job.
ls:	List files and directories.
pwd:	Print working directory.
read:	Read a line of input.
sleep:	Sleep for a specified amount of time.
source:	Execute a script.
test:	Evaluate a condition.
wait:	Wait for a child process to finish.

# Scripts
You can also run scripts from the shell. A script is a file that contains a series of commands. To run a script, type the following command:

./simple_shell script.sh

# Bugs
This is a work in progress, so there may be some bugs. If you find any bugs, please report them to me.
