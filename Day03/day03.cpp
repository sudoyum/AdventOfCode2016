#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1


// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
}


// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}


// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
  ltrim(s);
  return s;
}


// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
  rtrim(s);
  return s;
}


// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
  trim(s);
  return s;
}


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


bool valid_triangle(int side1,
    int side2,
    int side3) {
  return (side1 + side2 > side3 &&
      side2 + side3 > side1 &&
      side1 + side3 > side2);
}


void solve_p1(vector<string> &vals) {
  int possible = 0;
  for(string triangle: vals) {
    trim(triangle);
    vector<string> dimensions = splitString(triangle, "\\s+");
    if(valid_triangle(stoi(dimensions[0]),
          stoi(dimensions[1]),
          stoi(dimensions[2]))) {
      possible++;
    }
  }
  cout << "PART1: " << possible << endl;
}


void solve_p2(vector<string> &vals) {
  int possible = 0;
  int iter = 0;
  vector<vector<string> > three_triangles;
  for(string triangle: vals) {
    trim(triangle);
    vector<string> single = splitString(triangle, "\\s+");
    three_triangles.push_back(single);
    if(++iter % 3 == 0) {
      for(int i = 0; i < 3; i++) {
        if(valid_triangle(stoi(three_triangles[0][i]),
              stoi(three_triangles[1][i]),
              stoi(three_triangles[2][i]))) {
          possible++;
        }
      }
      three_triangles.clear();
    }
  }
  cout << "PART2: " << possible << endl;
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
  vector<string> cl_args = {"both", "p1", "p2"};

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
