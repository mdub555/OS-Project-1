/**
 * This file contains implementations of the functions that provide
 * tab-completion for the shell.
 *
 * You will need to finish the implementations of the completion functions,
 * though you're spared from implementing the high-level readline callbacks with
 * their weird static variables...
 */

#include "shell.h"
#include <cstdlib>
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <dirent.h>

using namespace std;


void Shell::get_env_completions(const char* text, vector<string>& matches) {
  // add the environment variables
  // get the text string without the '$'
  string textString = ((string)text).substr(1);
  // loop through the environment variables
  for (char** env = environ; *env != 0; env++) {
    string envString = *env;
    // find the '=' sign in the variable
    string::size_type eq_pos = envString.find("=");
    if (eq_pos == string::npos) continue;
    // get everything before the '=' sign
    else envString = envString.substr(0, eq_pos);
    // check that it matches what we have so far
    if (!envString.compare(0, textString.size(), textString)) {
      matches.push_back("$" + envString);
    }
  }

  // add the local variables
  map<string, string>::iterator it;
  // loop through all the aliases
  for (it = localvars.begin(); it != localvars.end(); it++) {
    // check if it mathces the string so far
    if (!it->first.compare(0, textString.size(), textString)) {
      matches.push_back("$" + it->first);
    }
  }
}


void Shell::get_command_completions(const char* text, vector<string>& matches) {
  string textString = text;
  // add the builtin commands
  // loop through all builtins
  for (map<string, builtin_t>::iterator it = builtins.begin(); it != builtins.end(); it++) {
    // check if it matches the string so far
    if (!it->first.compare(0, textString.size(), textString)) {
      matches.push_back(it->first);
    }
  }

  // add the aliases
  // loop through all the aliases
  for (map<string, string>::iterator it = aliases.begin(); it != aliases.end(); it++) {
    // check if it mathces the string so far
    if (!it->first.compare(0, textString.size(), textString)) {
      matches.push_back(it->first);
    }
  }

  // add the external commands
  DIR           *dirp;      // needed when looping through all the $PATH directories
  struct dirent *directory;
  char* temp = getenv("PATH");
  if (temp == NULL) {
    cerr << __FUNCTION__ << ": $PATH does not exist" << endl;
    return;
  }
  string path = (string)temp;
  string::size_type col_pos;
  // loop through the path and get the individual directories (seperated by colons)
  do {
    col_pos = path.find(":");
    string subpath = path.substr(0, col_pos);
    path = path.substr(col_pos + 1);
    // go into the directory loop through all the files
    dirp = opendir(subpath.c_str());
    if (dirp) {
      while ((directory = readdir(dirp)) != NULL) {
        // check for being executabel
        if (access(directory->d_name, X_OK)) {
          string temp = (string)directory->d_name;
          // check if it matches the string so far
          if (!temp.compare(0, textString.size(), textString)) {
            // add if to the vector
            matches.push_back(temp);
          }
        }
      }
      if (closedir(dirp) != 0) {
        return;
      }
    } else {
      return;
    }
  } while (col_pos != string::npos);
}


char** Shell::word_completion(const char* text, int start, int end) {
  char** matches = NULL;

  if (text[0] == '$') {
    matches = rl_completion_matches(text, env_completion_generator);
  } else if (start == 0) {
    matches = rl_completion_matches(text, command_completion_generator);
  } else {
    // We get directory matches for free (thanks, readline!).
  }

  return matches;
}


char* Shell::env_completion_generator(const char* text, int state) {
  // A list of all the matches.
  // Must be static because this function is called repeatedly.
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches.
  if (state == 0) {
    getInstance().get_env_completions(text, matches);
  }

  // Return a single match (one for each time the function is called).
  return pop_match(matches);
}


char* Shell::command_completion_generator(const char* text, int state) {
  // A list of all the matches.
  // Must be static because this function is called repeatedly.
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches.
  if (state == 0) {
    getInstance().get_command_completions(text, matches);
  }

  // Return a single match (one for each time the function is called).
  return pop_match(matches);
}


char* Shell::pop_match(vector<string>& matches) {
  if (matches.size() > 0) {
    const char* match = matches.back().c_str();

    // We need to return a copy, because readline deallocates when done.
    char* copy = (char*) malloc(strlen(match) + 1);
    strcpy(copy, match);

    // Delete the last element.
    matches.pop_back();

    return copy;
  }

  // No more matches.
  return NULL;
}
