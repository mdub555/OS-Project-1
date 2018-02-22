/**
 * This file contains implementations of the functions that are responsible for
 * executing commands that are not builtins.
 *
 * Much of the code you write will probably be in this file. Add helper methods
 * as needed to keep your code clean. You'll lose points if you write a single
 * monolithic function!
 */

#include "shell.h"
#include "command.h"
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

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
  vector<command_t> commands;
  if (!partition_tokens(tokens, commands)) return -1;

  const int PIPE_READ = 0;  // to acces read and write sides of pipe
  const int PIPE_WRITE = 1;
  int the_pipe[2]; // read is [0], write is [1]
  int read_fd;
  int status = 0;
  for (size_t i = 0; i < commands.size(); i++) {
    int pid;

    // make a char* array for the tokens
    char** cmd = new char*[commands[i].argv.size() + 1]; // leave room for NULL
    // convert the tokens to the char* array
    cmd = to_char_array(commands[i].argv);

    if (commands[i].output_type == OutputType::WRITE_TO_PIPE) { // if we're outputting to pipe
      if (pipe(the_pipe) < 0) { // open the pipe
        perror("opening pipe");
        return errno;
      }
    }

    // fork and check for errors
    if ((pid = fork()) == -1) {
      perror("fork failed");
      return 1;
    }

    if (pid == 0) { // if we're the child process
      // setup the input stream
      if (commands[i].input_type == READ_FROM_PIPE) {
        // dup for reading from the pipe
        if (dup2(read_fd, STDIN_FILENO) < 0) {
          perror("READ_FROM_PIPE dup2 error");
          _exit(errno);
        }
      } else if (commands[i].input_type == READ_FROM_FILE) {
        // dup for reading from a file
        int fd = open(commands[i].infile.c_str(), O_RDONLY, 0644);
        if (fd < 0) {
          perror("READ_FROM_FILE file open error");
          _exit(errno);
        }
        if (dup2(fd, STDIN_FILENO) < 0) {
          perror("READ_FROM_FILE dup2 error");
          _exit(errno);
        }
      }

      // setup the output stream
      if (commands[i].output_type == WRITE_TO_PIPE) {
        // dup for writing to the pipe
        close(the_pipe[PIPE_READ]); // never read from the pipe, read_fd is used instead
        if (dup2(the_pipe[PIPE_WRITE], STDOUT_FILENO) < 0) {
          perror("WRITE_TO_PIPE dup2 error");
          _exit(errno);
        }
      } else if (commands[i].output_type == WRITE_TO_FILE) {
        // dup for writing to a file
        int fd = open(commands[i].outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
          perror("WRITE_TO_FILE file open error");
          _exit(errno);
        }
        if (dup2(fd, STDOUT_FILENO) < 0) {
          perror("WRITE_TO_FILE dup2 error");
          _exit(errno);
        }
      } else if (commands[i].output_type == APPEND_TO_FILE) {
        // dup for appending to a file
        int fd = open(commands[i].outfile.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) {
          perror("APPEND_TO_FILE file open error");
          _exit(errno);
        }
        if (dup2(fd, STDOUT_FILENO) < 0) {
          perror("APPEND_TO_FILE dup2 error");
          _exit(errno);
        }
      }
      // execute the command
      execvp(cmd[0], cmd);

      // exit with an error since this part pf the function should never be reached
      perror("exec failed");
      _exit(EXIT_FAILURE);
    } else {
      // wait for the child process to complete
      waitpid(pid, &status, 0);
    }
    close(the_pipe[PIPE_WRITE]);   // close the pipe
    read_fd = the_pipe[PIPE_READ]; // assign read_fd to the read side of the previous pipe
  }

  // return based on the status of the final command
  if (WIFEXITED(status)) return WEXITSTATUS(status);
  else return EXIT_FAILURE;
}
