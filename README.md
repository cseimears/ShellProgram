# File System Manager - Improved Shell

This is an improved Unix shell written in C by Connor Seimears 
for the CST315 - Operating Systems course project. It includes 
several built-in commands such as: 'ls', 'cd', 'mkdir', 'rm', 
'pwd', 'cp', 'find', 'mv', 'rmdir', 'stat', 'nano', 'echo', 
'history', 'open', 'exit', and more.

## Requirements

This program was written and tested on a Unix-like operating 
system and requires the following libraries:

stdio.h
stdlib.h
string.h
unistd.h
sys/types.h
sys/wait.h
sys/stat.h
readline.h
history.h
limits.h

## Usage

To run the program, follow these steps:

1. Open terminal after saving the project4.zip
2. Navigate to the directory of which 'project4.c' is located.
3. compile project4.c using 'gcc -o shell project4.c -lreadline'
4. Run the program using './shell'

From here, feel free to mess with all of the built-in commands.
Project4.docx provides screenshots of each of the required 
commands under the Test and Validate section.

## Build-in Commands

The Improved Unix Shell includes the following built-in 
commands:

ls [directory]: lists the contents of the current directory or 
the specified directory.

cd [directory]: changes the current directory to the specified 
directory or the parent directory (..).

pwd: prints the current working directory.

mkdir [directory]: creates a new directory with the specified 
name.

rm [file]: deletes the specified file.

cp [source] [destination]: copies the source file to the 
destination file or directory.

find [directory] [filename]: searches for a file with the 
specified name in the specified directory or its subdirectories.

mv [source] [destination]: moves the source file to the 
destination file or directory.

rmdir [directory]: removes the specified directory (must be 
empty).

stat [file]: displays information about the specified file.

nano [file]: opens the specified file in the Nano text editor.

echo [text]: prints the specified text to the console.

history: prints the command history of the current session.

open [file]: opens the specified file using the default program.

exit: exits the shell.

NOTES: To remove non-empty directories, use 'rm -rf [directory]'
