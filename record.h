#ifndef _csvdata_RECORD_
#define _csvdata_RECORD_
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
#include "stats.h"
using namespace std;

// a record is an aggregation of data for a single line.
class record {
private:
  // a vector of boolean which represents if the content of specific colume is a number.
  vector<bool> is_number;

  // a vector of double for each colume. If the content of the colume can't be represents
  // as a double it will be 0.0 and never be used.
  vector<double> values;

  // line number of this record
  int line_num;

  // check if the word is a valid number.
  //
  // Also number .123 is valid while +.123 is not.
  bool is_valid_number(string &s);



public:
  
  // store the content of each colume for a single record
  vector<string> content;

  record(int _line_number_);
  
  record();

  //put a single word into the data struct with updating the colume stats
  void input_word(string &word, stats &stats_of_the_col);

  //put a single word into the data struct without updating the colume stats
  void input_word(string &word);  

  // put a whole line into the data struct, also updates the stats of columes
  void input_line(const string &input, vector<stats> &stats_of_cols);


  // report the content of this record
  void report(const set<int> &st, ofstream &fout);

  // add a new colume for the record
  void add_col(int col1, int col2, char op);

};

#endif
