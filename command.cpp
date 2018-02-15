/**
 * Contains the implementation of functions declared in command.h.
 */

#include "command.h"
#include "shell.h"
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;


bool Shell::partition_tokens(vector<string> tokens, vector<command_t>& commands) {
  vector<string> delims = { "|", "<", ">", ">>" };
  // declare these once at the beginning
  const vector<string>::iterator begin = delims.begin();
  const vector<string>::iterator end = delims.end();

  // check for delimeters at the beginning of the command
  if (find(begin, end, tokens[0]) != end) {
    cerr << "Pipe or redirect at beginning of command" << endl;
    return false;
  }
  // check for delimeters at the end of the command
  if (find(begin, end, tokens[tokens.size()-1]) != end) {
    cerr << "Pipe or redirect at end of command" << endl;
    return false;
  }

  // check for multiple delimeters next to each other
  for (unsigned int i = 0; i < tokens.size()-1; i++) {
    if ((find(begin, end, tokens[i]) != end) && (find(begin, end, tokens[i+1]) != end)) {
      cerr << "Two pipes or redirects in a row" << endl;
      return false;
    }
  }

  // create temporaty command to use for all commands in the tokens vector
  command_t cmd;
  for (unsigned int i = 0; i < tokens.size(); i++) {
    if (find(begin, end, tokens[i]) == end) {
      // if it's not |, <, >, or >>, add it to a new command
      cmd.argv.push_back(tokens[i]);
    } else if (tokens[i] == delims[0]) { // found a pipe `|`
      if (cmd.output_type != OutputType::WRITE_TO_STDOUT) { // already have an output
        cerr << "Too many outputs" << endl;
        return false;
      }
      cmd.output_type = OutputType::WRITE_TO_PIPE;   // set output to go to pipe
      commands.push_back(cmd);                       // add command to vector of commands
      cmd = command_t();                             // set cmd back to default
      cmd.input_type = InputType::READ_FROM_PIPE;    // set input based on pipe
    } else if (tokens[i] == delims[1]) { // found an input file `<`
      if (cmd.input_type != InputType::READ_FROM_STDIN) { // already have an input
        cerr << "Too many inputs" << endl;
        return false;
      }
      cmd.input_type = InputType::READ_FROM_FILE;    // set input to read from file
      cmd.infile = tokens[++i];                      // set input file and skip next token
    } else { // writing or appending to file
      if (cmd.output_type != OutputType::WRITE_TO_STDOUT) { // already have an output
        cout << "Too many output files" << endl;
        return false;
      }
      if (tokens[i] == delims[2]) { // found output to file `>`
        cmd.output_type = OutputType::WRITE_TO_FILE;  // set output to write to file
      } else {                      // found append to file `>>`
        cmd.output_type = OutputType::APPEND_TO_FILE; // set output to append to file
      }
      cmd.outfile = tokens[++i];                      // set output file and skip next token
    }
  }
  commands.push_back(cmd); // add the final command to the list

  return true;
}


// Feel free to ignore everything below here. It's just code to allow you to
// cout a command in a nice, pretty format. =)


const char* input_types[] = {
  "READ_FROM_STDIN",
  "READ_FROM_FILE",
  "READ_FROM_PIPE"
};


const char* output_types[] = {
  "WRITE_TO_STDOUT",
  "WRITE_TO_PIPE",
  "WRITE_TO_FILE",
  "APPEND_TO_FILE"
};


ostream& operator <<(ostream& out, const command_t& cmd) {
  copy(cmd.argv.begin(), cmd.argv.end(), ostream_iterator<string>(out, " "));

  out << "\n    input:   " << input_types[cmd.input_type]
      << "\n    output:  " << output_types[cmd.output_type]
      << "\n    infile:  " << cmd.infile
      << "\n    outfile: " << cmd.outfile;

  return out;
}
