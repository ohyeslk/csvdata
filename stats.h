#ifndef _csvdata_STATS_
#define _csvdata_STATS_
#include <queue>
#include <cfloat>
#include <iostream>
using namespace std;

struct stats {
  double min_val, max_val, median_val, average;

  // if any valid numeric data has been add to this struct;
  bool has_data;

  // number of data in this colume
  int num;
  priority_queue<double> small_q, large_q;

  stats() {
    median_val = average = 0;
    min_val = DBL_MAX;
    max_val = DBL_MIN;
    num = 0;
    has_data = false;
  }

  // add a number to the stats of this colume
  void add_number(double x) {
    has_data = true;
    min_val = min(min_val, x);
    max_val = max(max_val, x);
    average = (average * num + x) / (num + 1);
    num++;
    update_median_heap(x);
    if (small_q.size() > large_q.size()) {
      median_val = small_q.top();
    } else {
      median_val = (small_q.top() - large_q.top() + 0.0) / 2;
    }
  }

  // use the running-median model to get the median_val quickly.
  void update_median_heap(double x) {
    double tmp = large_q.empty() ? DBL_MAX : -large_q.top();
    if (x <= tmp) {
      small_q.push(x);
    } else {
      large_q.pop();
      large_q.push(-x);
      small_q.push(tmp);
    }
    while (small_q.size() - large_q.size() > 1) {
      large_q.push(-small_q.top());
      small_q.pop();
    }
  }

  void show(int col_no) {
    if (has_data == false) {
      cout << "numeric data of this colume is not available\n";
    } else {
      cout << "colume_number: " << col_no << endl;
      cout << "********************************\n";
      cout << "minimum value: " << min_val << endl;
      cout << "maximum value: " << max_val << endl;
      cout << "median value: " << median_val << endl;
      cout << "average value: " << average << endl;
      cout << "********************************\n";
    }
  }
};

#endif
