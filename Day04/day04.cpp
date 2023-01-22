#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <map>

using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1


bool cmp(pair<char, int>& a,
    pair<char, int>& b) {
  if(a.second == b.second) {
    return a.first < b.first;
  }
  return a.second > b.second;
}


bool room_check(string ciphertext, string checksum) {
  map <char, int> letters;
  ciphertext.erase(std::remove(ciphertext.begin(), ciphertext.end(), '-'), ciphertext.end());
  for(auto it = ciphertext.begin(); it != ciphertext.end(); it++) {
    letters[*it]++;
  }

  vector<pair<char, int> > sorted_letters;
  for (auto& it : letters) {
    sorted_letters.push_back(it);
  }
  sort(sorted_letters.begin(), sorted_letters.end(), cmp);

  for(int i = 0; i < checksum.length(); i++) {
    if(sorted_letters[i].first != checksum[i]) {
      return false;
    }
  }
  return true;
}


string decipher(string ciphertext, int shift) {
  string plaintext;
  for(auto it = ciphertext.begin(); it != ciphertext.end(); it++) {
    if(*it == '-') {
      plaintext.push_back(' ');
    } else {
      int c = *it + shift;
      if(c > 'z') {
        c = 'a' + (c - 'z' - 1);
      }
      plaintext.push_back((char)c);
    }
  }
  return plaintext;
}


void solve_p1(vector<string> vals) {
  int sum = 0;
  const regex rgx("([-a-z]+)([0-9]+)\\[([a-z]+)\\]");
  for(auto it = vals.begin(); it != vals.end(); it++) {
    smatch matches;
    if(regex_search(*it, matches, rgx)) {
      if(room_check(matches[1].str(), matches[3].str())){
        sum += stoi(matches[2].str());
      }
    }
  }
  cout << "PART1: " << sum << endl;
}


void solve_p2(vector<string> vals) {
  string sector_id;
  const regex rgx("([-a-z]+)([0-9]+)\\[([a-z]+)\\]");
  for(auto it = vals.begin(); it != vals.end(); it++) {
    smatch matches;
    if(regex_search(*it, matches, rgx)) {
      if((decipher(matches[1].str(), stoi(matches[2].str()) % 26)).compare("northpole object storage ") == 0) {
        sector_id = matches[2].str();
        break;
      }
    }
  }
  cout << "PART2: " << sector_id << endl;
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
