#include "record.h"

record::record() {
  content.clear();
  is_number.clear();
  values.clear();
}


record::record(int _line_number_) : line_num(_line_number_) {
  content.clear();
  is_number.clear();
  values.clear();
}

// add a word to the record, at updates the stats for colume at the same time.
void record::input_word(string &word, stats &stats_of_the_col) {
  content.push_back(word);
  if (is_valid_number(word)) {
    double val = stod(word);
    is_number.push_back(true);
    values.push_back(val);
    stats_of_the_col.add_number(val);
  } else {
    is_number.push_back(false);
    values.push_back(0.0);
  }
}
// add a word without updating the stats for colume
void record::input_word(string &word) {
  content.push_back(word);
  if (is_valid_number(word)) {
    double val = stod(word);
    is_number.push_back(true);
    values.push_back(val);
  } else {
    is_number.push_back(false);
    values.push_back(0.0);
  }
}

// read a raw line separated by ',' and process this line
void record::input_line(const string &input, vector<stats> &stats_of_cols) {
  string buf;
  int col_no = 0;
  for (int i = 0; i < input.size(); i++) {
    if (input[i] == ',') {
      input_word(buf, stats_of_cols[col_no++]);
      buf.clear();
    } else {
      buf.push_back(input[i]);
    }
  }
  input_word(buf, stats_of_cols[col_no]);
}

// print this record to the output file
void record::report(const set<int> &st, ofstream &fout) {
  bool init = true;
  for (int i = 0; i < content.size(); i++) {
    // only when this colume is in the selected list
    if (st.find(i) != st.end() or st.empty()) {
      if (init) init = false;
      else fout << ",";
      fout << content[i];
    }
  }
  fout << endl;
}

// add a new colume according to the expression
void record::add_col(int col1, int col2, char op) {
  assert(col1 >= 0 && col1 < content.size());
  assert(col2 >= 0 && col2 < content.size());
  assert(op == '+' || op == '-' || op == 'x' || op == '/');
  double new_val;
  if (op == '+')  new_val = values[col1] + values[col2];
  if (op == '-')  new_val = values[col1] + values[col2];
  if (op == 'x')  new_val = values[col1] * values[col2];
  if (op == '/') {
   // check the numerator is no smaller than eps such the result will not be 
   // infinte large.
    assert(fabs(values[col2]) > 1e-8); 
    new_val = values[col1] / values[col2];                        
  }
  values.push_back(new_val);
  is_number.push_back(true);
  content.push_back(to_string(new_val));
}

// I reference this solution to check a valid number:
// https://leetcode.com/discuss/9013/a-simple-solution-in-cpp

// Note: Using regular expression will make the program pretty slow
// when dealing with the large file.
bool record::is_valid_number(string &s) {
  int i = 0;
  // skip the whilespaces
  for (; s[i] == ' '; i++);
  // skip the sign if exist
  if (s[i] == '+' || s[i] == '-') i++;
  int n_nm(0), n_pt(0);
  for (; (s[i] <= '9' && s[i] >= '0') || s[i] == '.'; i++) {
    s[i] == '.' ? n_pt++ : n_nm++;
  }
  // no more than one point, at least one digit
  if (n_pt > 1 || n_nm < 1) return false;
  // check the exponent if exist
  if(s[i] == 'e') {
    i++;
    // skip the sign
    if(s[i] == '+' || s[i] == '-') i++;
    int n_nm = 0;
    for(; s[i] >= '0' && s[i] <= '9'; i++, n_nm++);
    if(n_nm < 1)  return false;
  }
  // skip the trailing whitespaces
  for(; s[i] == ' '; i++);
  return s[i] == 0;  // must reach the ending 0 of the string
}
