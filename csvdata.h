#ifndef _csvdata_
#define _csvdata_
#include "stats.h"
#include "record.h"
#include "unistd.h"
#include <iostream>
#include <cstdio>
#include <set>
#include <fstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

class csvdata {
  ofstream fout;
  bool has_header;

  void initial_setup(const string &line);
  void add_new_records(const csvdata &other, int col_no);
public:
  vector<stats> stats_of_cols;
  vector<string> header_names;
  vector<record> recs;
  int line_num, num_of_cols;

  csvdata(const string &file_name, string &out_file_name, bool has_header_);

  // show stats for a specific colume
  void show_stats(int col_no);

  // output the processed csv file to a file
  void print_all(const set<int> &st);

  // create a new row according to the expression the user has entered.
  void create_col(int col1, int col2, char op);

  // merge another csvdata instance
  void join(const csvdata &other, bool outer, int col_no);


};

#endif
