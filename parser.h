#ifndef _csvdata_PARSER_
#define _csvdata_PARSER_

#include <set>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
using namespace std;

struct parser {
  //operation and col1 col2
  int col1 = -1, col2 = -1;
  char oper;

  set<int> cols_print, cols_selected;
  string in_file_name, out_file_name, join_file_name;
  bool outer = false;
  int join_col = -1;
  bool has_header = false;

  double str_to_d(const string &buf) {
    double ans;
    try {
      ans = stoi(buf);
    } catch(...) {
      cout << "Input a valid col number\n";
    }
    return ans;
  }

  // "Perform addition, subtraction, division, and multiplication on two columns"
  void calc_handler() {
    int N = strlen(optarg);
    string buf;
    // parse the formula to create new colume including error detecting
    for (int i = 0; i < N; i++) {
      if (optarg[i] == '+' || optarg[i] == '-' || optarg[i] == 'x' || optarg[i] == '/') {
        oper = optarg[i];
        col1 = str_to_d(buf);
        buf.clear();
      } else {
        buf.push_back(optarg[i]);
      }
    }
    col2 = str_to_d(buf);
  }

  void parse_helper(set<int> &st, string s) {
    int N = s.size();
    string buf;
    int range_start = -1, range_end = -1;
    bool range_select = false;
    //parse the input and put each colume into the cols_print
    for (int i = 0; i < N; i++) {
      if (s[i] == ',') {
        st.insert(str_to_d(buf));
        buf.clear();
      } else if (s[i] == '-') {
        range_select = true;
        range_start = str_to_d(buf);
        buf.clear();
      } else {
        buf.push_back(s[i]);
      }
    }
    if (range_select) {
      range_end = str_to_d(buf);
      for (int i = range_start; i <= range_end; i++) {
        st.insert(i);
      }
      return;
    }
    st.insert(str_to_d(buf));
  }

  void parse(int argc, char* argv[]) {
    char c;
    int optLen;
    while ((c = getopt(argc, argv, "Ohp:c:s:j:o:")) != -1) {
      // print stats for specified colums;
      switch (c)
      {
      case 'p':
          // coresponding to "Output statistics on a column: min, max, median and average"
          parse_helper(cols_print, string(optarg));
          break;
      case 'c':
          calc_handler();
          break;
      // specify output file name
      case 'o':
          out_file_name = string(optarg);
          break;
      // specify selected columes
      case 's':
          parse_helper(cols_selected, string(optarg));
          break;
      // join (inner join for default)
      case 'j':
          join_col = str_to_d(string(optarg));
          break;
      // outer join
      case 'O':
          outer = true;
          break;
      case 'h':
          has_header = true;
          break;
      default:
          break;
      }
    //   if (c == 'p') {
    //     // coresponding to "Output statistics on a column: min, max, median and average"
    //     parse_helper(cols_print, string(optarg));
    //   } else if (c == 'c') {
    //     calc_handler();
    //     // specify output file name
    //   } else if (c == 'o') {
    //     out_file_name = string(optarg);
    //     // specify selected columes
    //   } else if (c == 's') {
    //     parse_helper(cols_selected, string(optarg));
    //     // join (inner join for default)
    //   } else if (c == 'j') {
    //     join_col = str_to_d(string(optarg));
    //     // outer join?
    //   } else if (c == 'O') {
    //     outer = true;
    //   } else if (c == 'h') {
    //     has_header = true;
    //   }
    }
    join_file_name = string(argv[argc - 2]);
    in_file_name = string(argv[argc - 1]);
  }

  parser() {}
};

#endif
