#ifndef _csvdata_aggregation_
#define _csvdata_aggregation_
#include <sstream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <cassert>
#include <set>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>
#include <iostream>
#include "statistics.h"
using namespace std;

// a aggregation is an aggregation of data for a single line.
class aggregation {
private:
  // a vector of boolean which represents if the content of specific colume is a number.
  vector<bool> is_number;

  // a vector of double for each colume. If the content of the colume can't be represents
  // as a double it will be 0.0 and never be used.
  vector<double> values;

  // line number of this aggregation
  int line_num;

  // check if the word is a valid number.
  //
  // Also number .123 is valid while +.123 is not.
  bool is_valid_number(string &s);



public:

  // store the content of each colume for a single aggregation
  vector<string> content;

  aggregation(int _line_number_);

  aggregation();

  //put a single word into the data struct with updating the colume statistics
  void input_word(string &word, statistics &statistics_of_the_col);

  //put a single word into the data struct without updating the colume statistics
  void input_word(string &word);

  // put a whole line into the data struct, also updates the statistics of columes
  void input_line(const string &input, vector<statistics> &statistics_of_cols);


  // report the content of this aggregation
  void report(const set<int> &st, ofstream &fout);

  // add a new colume for the aggregation
  void add_col(int col1, int col2, char op);

};

#endif
