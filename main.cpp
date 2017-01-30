#include "func_parser.h"
#include "csvdata.h"
#include <set>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
  func_parser psr;
  psr.parse(argc, argv);
  // create a csvdata initialized by input file name and output file name
  csvdata cm(psr.in_file_name, psr.out_file_name, psr.has_header);

  // if the user enter an expression, we need to create a new colume to store the result of the expression
  if (psr.col1 != -1) {
    cm.create_col(psr.col1, psr.col2, psr.oper);
  }
  // if need join, we need to read another csv and merge it to the current csv
  if (psr.join_col >= 0) {
    // create another instance.
    string outputname;
    csvdata another(psr.join_file_name, outputname, psr.has_header);
    cm.join(another, psr.outer, psr.join_col);
  }

  // if request for the colume statistics, we should show the pre-calculated results
  for (auto i : psr.cols_print) {
    cm.show_statistics(i);
  }

  // output the final result to the file
  cm.print_all(psr.cols_selected);
  return 0;
}
