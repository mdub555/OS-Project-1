/**
 * This file contains implementations of the functions that are responsible for
 * executing commands that are not builtins.
 *
 * Much of the code you write will probably be in this file. Add helper methods
 * as needed to keep your code clean. You'll lose points if you write a single
 * monolithic function!
 */

#include "shell.h"
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;


char** to_char_array(vector<string>& tokens) {
  char** result = new char*[tokens.size() + 1];
  // loop through the vector and put the tokens in the result array
  for (size_t i = 0; i < tokens.size(); i++) {
    result[i] = (char*)tokens[i].c_str();
  }
  // add the mandatory NULL at the end of the array
  result[tokens.size()] = NULL;
  return result;
}


int Shell::execute_external_command(vector<string>& tokens) {
  // make a char* array for the tokens
  char** cmd = new char*[tokens.size() + 1]; // leave room for NULL
  // convert the tokens to the char* array
  cmd = to_char_array(tokens);

  int status = 0;
  int pid;

  // fork and check for errors
  if ((pid = fork()) == -1) {
    perror("fork failed");
    return -1;
  }

  if (pid == 0) {
    // execute the command it you're the child
    execvp(cmd[0], cmd);

    // exit with an error since this part pf the function should never be reached
    perror("exec failed");
    _exit(EXIT_FAILURE);
  } else {
    // wait for the child process to complete
    waitpid(pid, &status, 0);
  }

  // return based on the status of the child
  if (WIFEXITED(status)) return WEXITSTATUS(status);
  else return EXIT_FAILURE;
}
