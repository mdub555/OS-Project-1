/**
 * This file contains the implementations of the builtin functions provided by
 * the shell (or will, once you've finished implementing them all).
 */

#include "shell.h"
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <readline/history.h>

using namespace std;

int get_current_directory(string& return_string) {
  char cwd[256];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror(__FUNCTION__);
    return errno;
  }
  return_string = (string)cwd;
  return 0;
}


int Shell::com_ls(vector<string>& argv) {
  // set the directory to open
  string dirToOpen;
  if (argv.size() == 1) {
    // get the current directory, returning not 0 is an error
    int err = get_current_directory(dirToOpen);
    if (err != 0) {
      return err;
    }
  } else if (argv.size() > 2) {
    // only allow one argument maximum
    cerr <<  __FUNCTION__ << ": Too many arguments." << endl;
    return -1;
  } else {
    // set the path to the argument
    dirToOpen = argv[1];
  }

  // open and read the directory
  DIR           *dirp;
  struct dirent *directory;
  dirp = opendir(dirToOpen.c_str());
  if (dirp) {
    while ((directory = readdir(dirp)) != NULL) {
      cout << directory->d_name << endl;
    }
    if (closedir(dirp) != 0) {
      perror(__FUNCTION__);
      return errno;
    }
  } else {
    perror(__FUNCTION__);
    return errno;
  }
  return 0;
}


int Shell::com_cd(vector<string>& argv) {
  string dir;
  if (argv.size() == 1) {
    // set next directory to be the home directory
    char* tempDir = getenv("HOME");
    if (tempDir == NULL) {
      cerr << __FUNCTION__ << ": HOME environment variable not found." << endl;
      return -1;
    }
    dir = tempDir;
  } else if (argv.size() > 2) {
    // error for too many arguments
    cerr << __FUNCTION__ << ": too many arguments." << endl;
    return -1;
  } else {
    dir = argv[1];
  }

  // change directory
  int ret = chdir(dir.c_str());
  if (ret != 0) {
    perror(__FUNCTION__);
    return errno;
  }

  return 0;
}


int Shell::com_pwd(vector<string>& argv) {
  // check for too many arguments
  if (argv.size() > 1) {
    cerr << __FUNCTION__ << ": Too many arguments." << endl;
    return -1;
  }
  // get the current directory and print it out
  string cwd;
  if (get_current_directory(cwd) == 0) {
    cout << cwd << endl;
  } else {
    cerr << __FUNCTION__ << ": something went wrong" << endl;
    return -1;
  }
  return 0;
}


int Shell::com_alias(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  cout << "alias called" << endl; // delete when implemented
  return 0;
}


int Shell::com_unalias(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  cout << "unalias called" << endl; // delete when implemented
  return 0;
}


int Shell::com_echo(vector<string>& argv) {
  // loop and print all the arguments
  for (size_t i = 1; i < argv.size(); i++) {
    cout << argv[i] << " ";
  }
  cout << endl;
  return 0;
}


int Shell::com_history(vector<string>& argv) {
  // check for too many arguments
  if (argv.size() > 1) {
    cerr << __FUNCTION__ << ": Too many arguments." << endl;
    return -1;
  }
  // get the list of previous commands
  HIST_ENTRY ** hist = history_list();
  // history_list() returns NULL if empty
  if (hist) {
    // the history list ends in NULL, so we can loop like this with an iterator
    for (int i = 0; hist[i]; i++) {
      cout << i + history_base << " " << hist[i]->line << endl;
    }
  }
  return 0;
}


int Shell::com_exit(vector<string>& argv) {
  // exit the program entirely
  exit(EXIT_SUCCESS);
}
