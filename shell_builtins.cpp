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
  // if no arguments are given, print all current aliases
  if (argv.size() == 1) {
    map<string, string>::iterator it;
    for (it = aliases.begin(); it != aliases.end(); it++) {
      cout << it->first << "=" << it->second << endl;
    }
  }
  for (size_t i = 1; i < argv.size(); i++) {
  //while (token != tokens.end()) {
    string::size_type eq_pos = argv[i].find("=");

    // Failure at the first token not in the form: key=value.
    if (eq_pos == string::npos) {
      cerr << __FUNCTION__ << ": Incorrect alias format." << endl;
      return -1;
    }

    // get the key value pair
    string key = argv[i].substr(0, eq_pos);
    string value = argv[i].substr(eq_pos + 1);
    // add it to the alias map
    if (aliases.count(key) > 0) { // overwrite the value
      aliases.at(key) = value;
    } else {                      // add a new key value pair
      aliases.insert( pair<string, string>(key, value) );
    }
  }

  return 0;
}


int Shell::com_unalias(vector<string>& argv) {
  // needs exactly 1 argument (not including unalias itself)
  if (argv.size() != 2) {
    cerr << __FUNCTION__ << ": Incorrect amount of arguments." << endl;
    return -1;
  }

  if (argv[1] == "-a") {
    aliases.clear();
  } else {
    // since the alias will have been expanded by now, must search for the value to erase it
    map<string, string>::iterator it;
    for (it = aliases.begin(); it != aliases.end(); it++) {
      if (it->second == argv[1]) {
        aliases.erase(it->first);
        break;
      }
    }
  }
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
