#include <algorithm>
#include <iostream>
#include <fstream>
#include <regex>
#include <set>
#include <vector>

using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define N 1
#define S 2
#define E 3
#define W 4


class Point {
  private:
    int x, y;
  public:
    Point(int x_val, int y_val) { x = x_val; y = y_val; }
};


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


static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
}


int manhattan_distance(int x, int y) {
  return abs(x) + abs(y);
}


void solve_p1(vector<string> &insts) {
  int x = 0, y = 0;
  int direction = N;
  for(vector<string>:: iterator it = insts.begin(); it != insts.end(); ++it) {
    string inst = (*it).erase(0, 1);
    char turn = inst[0];
    int amount;
    try {
      amount = stoi(inst.erase(0, 1));
    } catch (exception e) {
      cout << "Failed to stoi inst: " << inst << endl;
    }

    switch(direction) {
      case N: { direction = (turn == 'L') ? W : E; break; }
      case S: { direction = (turn == 'L') ? E : W; break; }
      case E: { direction = (turn == 'L') ? N : S; break; }
      case W: { direction = (turn == 'L') ? S : N; break; }
    }

    switch(direction) {
      case N: { y += amount; break; }
      case S: { y -= amount; break; }
      case E: { x += amount; break; }
      case W: { x -= amount; break; }
    }
  }
  cout << "PART1: " << manhattan_distance(x, y) << endl;
}


void solve_p2(vector<string> &insts) {
  int x = 0, y = 0;
  int direction = N;
  bool done = false;

  set <pair<int, int> > seen;

  while(!done) {
    for(vector<string>:: iterator it = insts.begin(); it != insts.end(); ++it) {
      string inst = *it;
      ltrim(inst);
      char turn = inst[0];
      int amount;
      inst.erase(0, 1);
      amount = stoi(inst);


      switch(direction) {
        case N: { direction = (turn == 'L') ? W : E; break; }
        case S: { direction = (turn == 'L') ? E : W; break; }
        case E: { direction = (turn == 'L') ? N : S; break; }
        case W: { direction = (turn == 'L') ? S : N; break; }
      }

      for(int i = 0; i < amount; i++) {
        if(false == seen.emplace(x, y).second) {
          done = true;
          break;
        }

        switch(direction) {
          case N: { y +=1; break; }
          case S: { y -=1; break; }
          case E: { x +=1; break; }
          case W: { x -=1; break; }
        }
      }
      if(done) {
        break;
      }
    }
  }

  cout << "PART2: " << manhattan_distance(x, y) << endl;
}


int parse_input(char *input_file, vector<string> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    parsed_input = splitString(line, ",");
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
