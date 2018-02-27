# Project 1

Your final submission must contain a README file with the following:
 * Your name.
 * A list of all the files in your submission and what each does.
 * Any unusual / interesting features in your programs.
 * Approximate number of hours you spent on the project.

## Name
Mitchell Waibel

## Files
* `README.md`
  This file.
* `command.cpp`
  Contains the definition of the `partition_tokens` function that takes a vector of tokens
  and puts it into a `command_t` struct that defines a command with unique input and output
  file descriptors, along with the input and output files (if needed)
* `command.h`
  Contains the declaration for the `command_t` struct and `partition_tokens` function.
* `main.cpp`
  Only spawns the shell. Has no other uses.
* `makefile`
  Contains the build code for this project. When `make` is used in this directory, the
  `MyShell` executable is built.
* `shell.h`
  Contains all function and variable definitions needed for the shell to run correctly. This
  includes all functions that are defined in the `shell_*.cpp` files.
* `shell_builtins.cpp`
  Definitions for all functions that are built into the shell. These commands are `ls`, `cd`,
  `pwd`, `alias`, `unalias`, `echo`, `history`, and `exit`.
* `shell_cmd_execution.cpp`
  Runs an external command, which can include pipes and file redirection. Piping and file
  redirection does not work for builtin commands, since the code is not structured for that
  purpose.
* `shell_core.cpp`
  Creates the shell singleton, runs the shell, tokenizes the input, dispaches commands,
  and handles all necessary substitution.
* `shell_tab_completion.cpp`
  Returns all appropriate tab completions to the readline library, given what has already
  been typed into the command line.
  

## Interesting Features
This shell program has all the standard features. No extra features or functionality was added.

## Time Spent
| Deliverable                          | Time     |
| ------------------------------------ | --------:|
| Shell builtins                       |  5 hours |
| History expansion and tab completion |  5 hours |
| Piping and file redirection          |  7 hours |
| _Total_                              | 17 hours |

## Graded
* Score tag final: 160/160
* Score Total: 200/200
* ~~Tab completion doesn't work for executables on $PATH -6~~
