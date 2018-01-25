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


int Shell::com_ls(vector<string>& argv) {
  DIR           *dirp;
  struct dirent *directory;

  string dirToOpen;
  if (argv.size() <= 1) {
    dirToOpen = ".";
  } else if (argv.size() > 2) {
    // only allow one argument maximum
    cout <<  __FUNCTION__ << ": Too many arguments." << endl;
    return -1;
  } else {
    // set the path to the argument
    dirToOpen = argv[1];
  }

  dirp = opendir(dirToOpen.c_str());
  // error check opendir
  if (errno != 0) {
    perror(__FUNCTION__);
    return errno;
  }

  if (dirp) {
    while ((directory = readdir(dirp)) != NULL) {
      // error check readdir
      if (errno != 0) {
        perror(__FUNCTION__);
        return errno;
      }
      cout << directory->d_name << endl;
    }
    closedir(dirp);
    if (errno != 0) {
      perror(__FUNCTION__);
      return errno;
    }
  }
  return 0;
}


int Shell::com_cd(vector<string>& argv) {
  string dir;
  if (argv.size() == 1) {
    // set next directory to be the home directory
    dir = getenv("HOME");
    if (errno != 0) {
      perror(__FUNCTION__);
      return errno;
    }
  } else if (argv.size() > 2) {
    // error for too many arguments
    cout << __FUNCTION__ << ": too many arguments." << endl;
    return -1;
  } else {
    dir = argv[1];
  }

  // change directory
  chdir(dir.c_str());
  if (errno != 0) {
    perror(__FUNCTION__);
    return errno;
  }

  // get the expanded working directory for PWD
  char cwd[256];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror(__FUNCTION__);
    return errno;
  }
  // set the PWD environment variable to the correct thing
  setenv("PWD", cwd, 1);
  if (errno != 0) {
    perror(__FUNCTION__);
  }

  return errno;
}


int Shell::com_pwd(vector<string>& argv) {
  // print out the environment variable "PWD"
  string pwd = getenv("PWD");
  if (errno == 0) {
    cout << pwd << endl;
  } else {
    perror(__FUNCTION__);
  }
  return errno;
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
