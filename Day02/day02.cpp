#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <vector>

using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

string g_matrix = R"""(0 0 1 0 0
0 2 3 4 0
5 6 7 8 9
0 A B C 0
0 0 D 0 0)""";

// Credit for this function:
// https://stackoverflow.com/questions/30050739/c-split-string-line-by-line-using-stdregex
vector<string> splitString(const std::string& stringToSplit, const std::string& regexPattern)
{
  vector<string> result;

  const regex rgx(regexPattern);
  sregex_token_iterator iter(stringToSplit.begin(), stringToSplit.end(), rgx, -1);

  for (sregex_token_iterator end; iter != end; ++iter)
  {
    result.push_back(iter->str());
  }

  return result;
}


string ints_to_string(vector<int> &vec) {
  vector<int>::const_iterator it;
  stringstream s;
  for( it = vec.begin(); it != vec.end(); ++it )
  {
    s << *it;
  }
  return s.str();
}


void solve_p1(vector<string> &instructions) {
  vector <int> password;
  int keypad[3][3];
  int xpos = 1, ypos = 1;

  for(int i = 0; i < 9; i++ ) {
    keypad[i % 3][i / 3] = i + 1;
  }

  for(auto &instruction: instructions) {
    for(auto &i: instruction) {
      switch (i) {
        case 'U': { if(ypos >= 1) { ypos -= 1; } break; }
        case 'D': { if(ypos <= 1) { ypos += 1; } break; }
        case 'L': { if(xpos >= 1) { xpos -= 1; } break; }
        case 'R': { if(xpos <= 1) { xpos += 1; } break; }
      }
    }
    password.push_back(keypad[xpos][ypos]);
  }

  cout << "PART1: " << ints_to_string(password) << endl;
}


bool p2in_bounds(char keypad[5][5], int xpos, int ypos) {
  if(xpos >= 5 || ypos >= 5 || xpos < 0 || ypos < 0) {
    return false;
  }

  if(keypad[xpos][ypos] == '0') {
    return false;
  }
  return true;
}

void solve_p2(vector<string> &instructions) {
  //      1
  //    2 3 4
  //  5 6 7 8 9
  //    A B C
  //      D
  //
  //  0 0 1 0 0
  //  0 2 3 4 0
  //  5 6 7 8 9
  //  0 A B C 0
  //  0 0 D 0 0

  string password;
  char keypad[5][5] = {0};
  int xpos = 0, ypos = 2;

  vector<string> lines = splitString(g_matrix, "\n");
  int x = 0, y = 0;
  for(auto &line: lines) {
    vector<string> split_line = splitString(line, " ");
    y = 0;
    for(auto it = split_line.begin(); it != split_line.end(); it++) {
      keypad[y][x] = it->c_str()[0];
      y++;
    }
    x++;
  }

  for(auto &instruction: instructions) {
    for(auto &i: instruction) {
      switch (i) {
        case 'U': { if(p2in_bounds(keypad, xpos, ypos - 1)) { ypos -= 1; } break; }
        case 'D': { if(p2in_bounds(keypad, xpos, ypos + 1)) { ypos += 1; } break; }
        case 'L': { if(p2in_bounds(keypad, xpos - 1, ypos)) { xpos -= 1; } break; }
        case 'R': { if(p2in_bounds(keypad, xpos + 1, ypos)) { xpos += 1; } break; }
      }
    }
    password += keypad[xpos][ypos];
  }

  cout << "PART2: " << password << endl;
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
  vector <string> parsed_input;
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
