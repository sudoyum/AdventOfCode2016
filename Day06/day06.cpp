#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define MSG_LEN 8


// TODO: Algorithm with one iteration through
void solve_p1(vector<string> &vals) {
  string error_corrected;
  int letters[MSG_LEN][26] = {0};
  int column = 0;
  for(auto it = vals.begin(); it != vals.end(); it++) {
    column = 0;
    for (auto& i: *it) {
      letters[column++][i - 'a']++;
    }
  }

  for(int i = 0; i < MSG_LEN; i++) {
    int max = 0, max_index = 0;
    for(int j = 0; j < 26; j++) {
      if(letters[i][j] > max) {
        max = letters[i][j];
        max_index = j;
      }
    }
    error_corrected.push_back(max_index + 'a');
  }

  cout << "PART1: " << error_corrected << endl;
}


void solve_p2(vector<string> &vals) {
  string error_corrected;
  int letters[MSG_LEN][26] = {0};
  int column = 0;
  for(auto it = vals.begin(); it != vals.end(); it++) {
    column = 0;
    for (auto& i: *it) {
      letters[column++][i - 'a']++;
    }
  }

  for(int i = 0; i < MSG_LEN; i++) {
    int min = 0x10000, min_index = 0;
    for(int j = 0; j < 26; j++) {
      if(letters[i][j] != 0 && letters[i][j] < min) {
        min = letters[i][j];
        min_index = j;
      }
    }
    error_corrected.push_back(min_index + 'a');
  }

  cout << "PART2: " << error_corrected << endl;
}


int parse_input(char *input_file, vector<string> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    parsed_input.push_back(line);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<string> &parsed_input) {
  if(p_arg == "p1") {
    solve_p1(parsed_input);
  } else if(p_arg == "p2") {
    solve_p2(parsed_input);
  } else if(p_arg == "both") {
    solve_p1(parsed_input);
    solve_p2(parsed_input);
  }
}


int main(int argc, char *argv[]) {
  int rc = EXIT_SUCCESS;
  vector<string> parsed_input;
  std::vector<std::string> cl_args = {"both", "p1", "p2"};

  if(argc != 3 || count(cl_args.begin(), cl_args.end(), argv[2]) <= 0) {
    cout << "Usage: ./day input.txt p1|p2|both" << endl;
    rc = EXIT_FAILURE;
    goto cleanup;
  }

  if(parse_input(argv[1], parsed_input) == PARSE_FAILURE) {
    cout << "Issue parsing input, exiting" << endl;
    rc = EXIT_FAILURE;
    goto cleanup;
  }

  run_solutions(argv[2], parsed_input);

cleanup:
  return rc;
}
