#include "csvdata.h"

csvdata::csvdata(const string &file_name, string &out_file_name, bool has_header_) {
  // if output file is not specified, then it will be "csvdata.out"
  if (out_file_name.empty()) {
    out_file_name = "csvdata.out";
  }
  has_header = has_header_;
  line_num = 0;
  ifstream in_file(file_name);
  assert(access(file_name.c_str(), F_OK) != -1);
  fout = ofstream(out_file_name);
  string line;
  recs.clear();
  bool initial = true;
  while (getline(in_file, line)) {
    if (initial) {
      initial_setup(line);
      initial = false;
      if (has_header) continue;
    }
    aggregation tmp(line_num++);
    tmp.input_line(line, statistics_of_cols);
    recs.push_back(tmp);
  }
}

void csvdata::initial_setup(const string &line) {
  num_of_cols = 1;
  statistics_of_cols.push_back(statistics());
  string buf;
  for (auto ele : line) {
    if (ele == ',') {
      if (has_header) {
        header_names.push_back(buf);
        buf.clear();
      } else {
        header_names.push_back(to_string(num_of_cols - 1));
      }
      num_of_cols++;
      statistics_of_cols.push_back(statistics());
    } else {
      buf.push_back(ele);
    }
  }
  // set header names if specified
  if (has_header) {
    header_names.push_back(buf);
  } else {
    header_names.push_back(to_string(num_of_cols - 1));
  }
}

// output the processed csv file.
void csvdata::print_all(const set<int> &st) {
  while (recs[0].content.size() > header_names.size()) header_names.push_back("new");
  bool init = true;
  for (int i = 0; i < header_names.size(); i++)
    if (st.empty() || st.find(i) != st.end()) {
      if (init) init = false;
      else fout << ",";
      fout << "col_" << header_names[i];
    }
  fout << endl;
  for (auto i : recs) {
    i.report(st, fout);
  }
}

// create a new colume to store the resule the operation
void csvdata::create_col(int col1, int col2, char op) {
  for (auto &rc : recs) {
    rc.add_col(col1, col2, op);
  }
}

// print statistics about specified colume to stdout
void csvdata::show_statistics(int col_no) {
  if (col_no < 0 || col_no >= statistics_of_cols.size()) {
    cout << "col number out of bound\n";
  } else {
    statistics_of_cols[col_no].show(col_no);
  }
}


// This function only called when doing outer join. You should add aggregations which may not
// originally existed.
void csvdata::add_new_aggregations(const csvdata &other, int col_no) {
  set<string> st;
  for (auto i : recs) {
    assert(col_no >= 0 and col_no < i.content.size());
    st.insert(i.content[col_no]);
  }
  for (auto i : other.recs) {
    if (st.find(i.content[col_no]) == st.end()) {
      aggregation tmp(line_num++);
      for (int k = 0; k < num_of_cols; k++) {
        if (k != col_no) {
          string str_tmp = "";
          tmp.input_word(str_tmp);
        } else {
          tmp.input_word(i.content[col_no]);
        }
      }
      for (int k = 0; k < i.content.size(); k++) {
        if (k == col_no) continue;
        tmp.input_word(i.content[k]);
      }
      recs.push_back(tmp);
    }
  }
}

// Doing the join operation
void csvdata::join(const csvdata &other, bool outer, int col_no) {
  map<string, aggregation> exist;
  for (auto i : other.recs) {
    assert(col_no >= 0 and col_no < i.content.size());
    exist[i.content[col_no]] = i;
  }
  for (int i = 0; i < recs.size(); ) {
    assert(col_no >= 0 and col_no < recs[i].content.size());
    if (exist.find(recs[i].content[col_no]) == exist.end()) {
      // not found
      if (outer) {
        i++;
      } else {
        recs.erase(recs.begin() + i);
      }
    } else {
      for (int k = 0; k < exist[recs[i].content[col_no]].content.size(); k++) {
        if (k == col_no) continue;
        recs[i].input_word(exist[recs[i].content[col_no]].content[k]);
      }
      i++;
    }
  }
  if (outer) add_new_aggregations(other, col_no);
  // merge statistics for columes
  for (int k = 0; k < other.statistics_of_cols.size(); k++) {
    if (col_no == k) continue;
    statistics_of_cols.push_back(other.statistics_of_cols[k]);
  }
}
