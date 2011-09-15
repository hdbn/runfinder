////////////////////////////////////////////////////////////////////////////////
//
// runFinderMain.cpp
// count runs of each line of stdin
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2011 Hideo Bannai
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sys/time.h>
#include "runFinder.hpp"
#include "bits.h"

using namespace std;

int main(int argc, char * argv[]){
  string s;
  runFinder rc;
  vector<run> runs;
  vector<unsigned int> POS, LEN;
  unsigned int i, length;
  struct timeval btv, etv;  
  while(cin >> s){
    gettimeofday(&btv, NULL);
    length = s.size();
    rc.findRuns(s, runs);
    cout << "# of runs = " << runs.size() << endl;
    for(i = 0; i < runs.size(); i++){
      cout << "([" 
	   << runs[i].b_pos << ","
	   << runs[i].e_pos << "],"
	   << runs[i].period << ")" 
	   << endl;
    }
    gettimeofday(&etv, NULL);
    printf("Total Time: approx %.5f seconds\n", timediff(btv, etv));
  }
  return 0;
}
