/**
 * This file contains the implementations of the builtin functions provided by
 * the shell (or will, once you've finished implementing them all).
 */

#include "shell.h"
#include <iostream>
#include <dirent.h>

using namespace std;


int Shell::com_ls(vector<string>& argv) {
  DIR           *dirp;
  struct dirent *directory;

  // get the current directory
  string dirToOpen = getenv("PWD");
  // error check getenv()
  if (errno != 0) {
    perror("The following error occured");
    return errno;
  }

  if (argv.size() > 1) {
    // only allow one argument maximum
    if (argv.size() > 2) {
      cout << "The following error occured: Too many arguments." << endl;
      return -1;
    }
    // if the argument is an absolute path, set that as the directory to open
    if (argv[1][0] == '/') {
      dirToOpen = argv[1];
    // else, add it to the end of the path for a relative path
    } else {
      dirToOpen += "/";
      dirToOpen += argv[1];
    }
  }

  dirp = opendir(dirToOpen.c_str());
  // error check opendir
  if (errno != 0) {
    perror("The following error occured");
    return errno;
  }

  if (dirp) {
    while ((directory = readdir(dirp)) != NULL) {
      // error check readdir
      if (errno != 0) {
        perror("The following error occured");
        return errno;
      }
      cout << directory->d_name << endl;
    }
    closedir(dirp);
    if (errno != 0) {
      perror("The following error occured");
      return errno;
    }
  }
  return 0;
}


int Shell::com_cd(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  cout << "cd called" << endl; // delete when implemented
  return 0;
}


int Shell::com_pwd(vector<string>& argv) {
  // print out the environment variable "PWD"
  string pwd = getenv("PWD");
  if (errno == 0) {
    cout << pwd << endl;
  } else {
    perror("The following error occured");
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
  // TODO: YOUR CODE GOES HERE
  cout << "history called" << endl; // delete when implemented
  return 0;
}


int Shell::com_exit(vector<string>& argv) {
  // exit the program entirely
  exit(EXIT_SUCCESS);
}
