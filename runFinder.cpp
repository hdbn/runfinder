////////////////////////////////////////////////////////////////////////////////
//
// runFinder.cpp
// find and count runs using (almost) linear time algorithm
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

#include "runFinder.hpp"
#include <cassert>
#include <sys/time.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

////////////////////////////////////////////////////////////////////////////////

run::run()
  : b_pos(0), period(0), e_pos(0)
{};

run::run(int b_pos_, int period_, int e_pos_)
  : b_pos(b_pos_), period(period_), e_pos(e_pos_)
{};

void runFinder::findRuns(const string & s, 
			 vector<run> & runs, 
			 enum ALGFLAG algf){
  vector<vector<pair<unsigned int, unsigned int> > > runs_by_bpos;
  runFinder::runsAux(s, runs_by_bpos, algf);
  runs.clear();
  vector<pair<unsigned int, unsigned int> >::const_reverse_iterator itr;  
  for(unsigned int beginp = 0; beginp < runs_by_bpos.size(); beginp++){
    for(itr = runs_by_bpos[beginp].rbegin(); itr != runs_by_bpos[beginp].rend(); itr++){
      runs.push_back(run(beginp, (*itr).second, (*itr).first));
    }
  }
  return;
}

unsigned int runFinder::countRuns(const std::string & s, enum ALGFLAG algf){
  vector<vector<pair<unsigned int, unsigned int> > > runs_by_bpos;
  return (runFinder::runsAux(s, runs_by_bpos, algf)); 
}

unsigned int runFinder::runsAux(const string & s, 
				 vector<vector<pair<unsigned int, unsigned int> > > & runs_by_bpos,
				 enum ALGFLAG algf){
  unsigned int i, j, k, beginp, endp, p, length, count;
  std::vector<unsigned int> POS, LEN;
  LZ77::lpf(s, POS, LEN, algf);
  length = s.size();
  runs_by_bpos = vector<vector<pair<unsigned int, unsigned int> > >(length);
  vector<vector<pair<unsigned int, unsigned int> > > runs_by_epos(length);

  count = 0;

  unsigned int tlen, ulen, tbp, prevubp, ubp;

  ////////////////////////////////////////////////////////////////////////////////
  // find type 1 runs: 
  // those that touch the boundary of the begining of u, and ends in u, where u is a lz factor
  // ubp:     beginnin position of lz factor u
  // prevubp: beginnin position of previous lz factor
  ////////////////////////////////////////////////////////////////////////////////
  for(prevubp = 0, ubp = 1;
      ubp < length;
      prevubp=ubp, ubp += max((unsigned int) 1,LEN[ubp])){
    ulen = max((unsigned int) 1,LEN[ubp]); // length of u
    tlen = 2 * LEN[prevubp] + ulen;        // maximum length of t that we need to consider.
    tlen = (tlen > ubp) ? ubp : tlen;      // t can't go past the beggining of the string
    tbp = ubp - tlen;                      // beginning position of t

    //             tlen              ulen
    //   |--------- t --------|------- u -------|
    //    tbp                  ubp

    // Naive implemantation
    // this makes the algorithm non-linear time, but I'm not sure which would be faster.

    // runs that start in t and end in u, with at least one full period in t.
    // we also need to include runs which are suffixes of the previous factor
    for(i = 1; i <= tlen; i++){                                      // checking period = i      
      //   |--------- t --------|------- u -------|
      //    tbp                  ubp
      //              |--- i ---|
      //              |- j ->   |- j ->
      for(j = 0; j < ulen && s[ubp-i+j] == s[ubp+j]; j++){};         // check forward      
      if((j == ulen) && (ubp + j - 1 < length) && (s[ubp-i+j] == s[ubp+j])) 
	continue; // ignore if run extends beyond u. 

      //   |--------- t --------|------- u -------|
      //    tbp                  ubp
      //              |--- i ---|
      //        <- k -|   <- k -|
      for(k = 0; i + k < tlen && s[ubp-1-k-i] == s[ubp-1-k]; k++){}; // check backward
      if((j > 0 || prevubp <= ubp - i - k) // crosses or is a suffix of previous factor
	 && j+k >= i){
	// cout << "found: " << "([" << ubp-i-k << "," << ubp+j-1 << "]," << i << ")" << endl;
	runs_by_epos[ubp+j-1].push_back(make_pair(ubp-i-k,i));
      }
    }

    // runs that start in t and end in u, with at least one full period in u.
    // we also need to include runs which are prefixes of u.
    for(i = 1; i <= ulen; i++){                                      // checking period = i
      //   |--------- t --------|------- u -------|
      //    tbp                  ubp
      //                        |--- i ---|
      //                        |- j ->   |- j ->
      for(j = 0; i + j < ulen && s[ubp+j] == s[ubp+i+j]; j++){} ;    // check forward
      if(i+j == ulen && (ubp + i + j - 1 < length) && s[ubp+j] == s[ubp+i+j]) 
	continue; // ignore if run, extends beyond u.

      //   |--------- t --------|------- u -------|
      //    tbp                  ubp
      //                        |--- i ---|
      //                  <- k -|   <- k -|
      for(k = 0; k < tlen && s[ubp-1-k] == s[ubp+i-1-k]; k++){};     // check backward
      if(j+k >= i){
	// cout << "found: " << "([" << ubp-k << "," << ubp+i-1+j << "]," << i << ")" << endl;
	runs_by_epos[ubp+i-1+j].push_back(make_pair(ubp-k, i)); 
      }
    }
    
    // note that including the runs that only touch the boundary of u is important
    // in order to count the run, when the u begins in the middle of a begining of a previously
    // occurring run. (it is difficult to copy the run, when we don't know where it started)
  }
  
  // count them with sort/uniq by beginpos and endpos
  // runs_by_bpos[beginp] contains runs (endp, period) in decreasing order of endp
  for(endp = length-1; endp > 0; endp--){ // bigger ones first
    for(p = 0; p < runs_by_epos[endp].size(); p++){
      beginp = runs_by_epos[endp][p].first;
      if(runs_by_bpos[beginp].empty() || runs_by_bpos[beginp].back().first != endp){
	runs_by_bpos[beginp].push_back(make_pair(endp, runs_by_epos[endp][p].second));
	count++;
      }
    }
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  // count number of type 2 runs: runs that are completely contained in lz factors
  ////////////////////////////////////////////////////////////////////////////////
  for(ubp = 1; ubp < length; ubp += max((unsigned int) 1,LEN[ubp])){
    ulen = max((unsigned int) 1,LEN[ubp]);
    unsigned int prevfactorbp = POS[ubp];   // begin position of previous factor
    if(prevfactorbp != ubp){
      assert(LEN[ubp] > 0);
      for(i = 1; i + 1 < ulen; i++){            // for each position in factor
	// check the number of runs that started in previous factor that fit in current factor.
	// we count the run only if it is a proper factor of u, 
	// or if it is a proper suffix of the last lz factor
	beginp = prevfactorbp + i;
	unsigned int llen = runs_by_bpos[beginp].size();
	unsigned int lastj = 0;
	for(j = llen; j-- > 0;){   // check from shorter runs
	  endp = runs_by_bpos[beginp][j].first;
	  // cout << "ubp = " << ubp << " i = " << i << " check: [" << beginp << "," << endp << "] : ulen = " << ulen << endl;
	  // cout << "period = " << runs_by_bpos[beginp][j].second << endl;
	  if(!((endp - beginp + 1 < ulen - i) // a proper factor of u
	       || ((ubp + ulen >= length) // u is last lz factor
		   && (ulen - i >= runs_by_bpos[beginp][j].second * 2) // long enough to be a run
		   ))){
	    // cout << "NO"<< endl;
	    lastj = j+1;
	    break;
	  } else {
	    // cout << "yes" << endl;
	  }
	}
	for(j = lastj; j < llen; j++){           // push the small enough ones into the new list (smaller last) 
	  endp = min(length - 1, ubp + runs_by_bpos[beginp][j].first - prevfactorbp);
	  // cout << "new: [" << ubp+i << "," << endp << "]" << endl;
	  runs_by_bpos[ubp + i].push_back(make_pair(endp, runs_by_bpos[beginp][j].second));
	  count++;
	}
      }
    } else {
      assert(LEN[ubp] == 0);
    }
  }
  return count;
}
