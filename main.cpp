
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#include "debug.hpp"

#define NEIGHBORHOOD_SIZE 15

using namespace std;

struct op {
  unsigned machine;
  unsigned cost;

  friend ostream& operator<<(ostream& os, const op& it);
};

using job = vector<op>;
using solution = vector<unsigned>;

ostream& operator<<(ostream& os, const op& it) {
    os << "(" << it.machine << ", " << it.cost << ")";
    return os;
}

vector<vector<unsigned>> costs;
vector<vector<op>> ops;

unsigned n, m;
unsigned it_max;


int help(char *program) {
  cout << program << "<file> <it_max>" << endl;
  return 0;
}

unsigned total_cost(solution s) {
  vector<unsigned> positions(costs.size(), 0);
  vector<unsigned> job_time(costs.size(), 0);
  vector<unsigned> machine_time(costs[0].size(), 0);

  for(unsigned j : s) {
    unsigned o = positions[j];
    unsigned m = ops[j][o].machine;
    unsigned time = costs[j][m] + max(job_time[j], machine_time[m]);
    job_time[j] = time;
    machine_time[m] = time;
    positions[j]++;
  }

  unsigned total = 0;
  for(unsigned m : machine_time)
    total = max(total, m);

  return total;
}

solution permutate(solution &s) {
  unsigned size = s.size();
	int pos1 = 1 + rand() % (size / 4);
	int pos2 = pos1 + 1 + rand() % (size / 4);
	int pos3 = pos2 + 1 + rand() % (size / 4);

	int i;
  solution s_;

	for (i = 0; i < pos1; i++)
    s_.push_back(s[i]);
	for (i = pos3; i < size; i++)
    s_.push_back(s[i]);
	for (i = pos2; i < pos3; i++)
    s_.push_back(s[i]);
	for (i = pos1; i < pos2; i++)
    s_.push_back(s[i]);

  return s_;
}

solution opt_2(solution s) {
  unsigned i = rand() % s.size();
  unsigned j = rand() % s.size();
  unsigned tmp = s[i];

  s[i] = s[j];
  s[j] = tmp;

  return s;
}

solution opt_3(solution s) {
	int i = 1 + rand() % (s.size() / 3);
	int j = i + 1 + rand() % (s.size() / 3);
	int k = j + 1 + rand() % (s.size() / 3);

  unsigned tmp0 = s[i];
  unsigned tmp1 = s[j];
  unsigned tmp2 = s[k];

  solution tmp = s;

  tmp[i] = tmp0;
  tmp[j] = tmp2;
  tmp[k] = tmp1;
  if(total_cost(tmp) < total_cost(s)) {
    s = tmp;
  }

  tmp[i] = tmp2;
  tmp[j] = tmp1;
  tmp[k] = tmp0;
  if(total_cost(tmp) < total_cost(s)) {
    s = tmp;
  }

  tmp[i] = tmp1;
  tmp[j] = tmp0;
  tmp[k] = tmp2;
  if(total_cost(tmp) < total_cost(s)) {
    s = tmp;
  }

  tmp[i] = tmp2;
  tmp[j] = tmp0;
  tmp[k] = tmp1;
  if(total_cost(tmp) < total_cost(s)) {
    s = tmp;
  }

  tmp[i] = tmp2;
  tmp[j] = tmp1;
  tmp[k] = tmp0;
  if(total_cost(tmp) < total_cost(s)) {
    s = tmp;
  }

  return s;
}


solution argmin(solution s, unsigned& k, unsigned p) {
  solution s_ = s;
  for(int i=0; i<p; i++) {
    solution tmp;

    switch (k) {
    case 1: tmp = opt_2(s); break;
    case 2: tmp = opt_3(s); break;
    default: break;
    }

    if(total_cost(tmp) < total_cost(s_))
      s_ = tmp;
  }

  return s_;
}

void sequential_neighborhood_change_step(solution &s, solution &s_, unsigned &k) {
  if(total_cost(s_) < total_cost(s)) {
    s = s_;
    k = 1;
  } else
    k = k + 1;
}

solution vnd_best_improvement(solution &s, unsigned l_max, unsigned& k, unsigned p) {
    unsigned l;
    bool stop;
    solution s_;

    do {
      stop = false;
      l = 1;
      s_ = s;
      do {
        solution s__ = argmin(s, k, p);
        sequential_neighborhood_change_step(s, s__, l);
      } while(l != l_max);
  
      if(total_cost(s_) <= total_cost(s))
        stop = true;
    
    } while(!stop);
    return s_;
}

solution general_vns(solution s) {
	unsigned it = 0, k = 0;
  ///TODO tirar daqui
  unsigned k_max = 2;
  unsigned l_max = 2;

	do {
    k = 1;
    while(k <= k_max) {
      solution s_ = permutate(s);
      solution s__ = vnd_best_improvement(s_, l_max, k, NEIGHBORHOOD_SIZE);
      sequential_neighborhood_change_step(s, s__, k);
    }
    it++;
	} while (it < it_max);
  
  return s;
}

int by_job(op &a, op &b) {
  return a.machine < b.machine;
}

job sort_by (job j){
  sort(j.begin(), j.end(), by_job);
  return j;
}

vector<vector<unsigned>> build_cost_matrix(vector<job> &ops) {
  vector<vector<unsigned>> costs;

  for(job j : ops){
    job j_sorted = sort_by(j);
    
    vector<unsigned> j_cost;

    for(op o : j_sorted)
      j_cost.push_back(o.cost);

    costs.push_back(j_cost);
  }
  return costs;
}

int main (int argc, char *argv[]) {

  if (argc < 1)
    return help(argv[0]);

  srand(time(nullptr));

  ifstream file(argv[1]);
  string line;
  
  it_max = atoi(argv[2]);
  
  for(unsigned i=0; i<4; i++)
    getline(file, line);

  file >> n >> m;
  for(unsigned i=0; i<n; i++) {
    vector<op> job;

    for(unsigned j=0; j<m; j++){
      op curr;
      file >> curr.machine >> curr.cost;
      job.push_back(curr);
    }

    ops.push_back(job);
  }

  costs = build_cost_matrix(ops);

  solution s;
  for (int j=0; j<n; j++)
    for (int i=0; i<m; i++)
      s.push_back(j);
  
  s = permutate(s);

  solution vns = general_vns(s);

  cout << total_cost(vns) << endl;

  file.close();
  return 0;
}