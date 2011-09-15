////////////////////////////////////////////////////////////////////////////////
//
// runFinder.hpp
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

#ifndef __RUN_FINDER_HPP__
#define __RUN_FINDER_HPP__

#include "lz77.hpp"

// class for runs
class run {
public:
  unsigned int b_pos;
  unsigned int period;
  unsigned int e_pos;
  run();
  run(int b_pos_, int period_, int e_pos_);
};

// class for counting runs
class runFinder {
  static unsigned int runsAux(const std::string & s,
			      std::vector<std::vector<std::pair<unsigned int, unsigned int> > > & runs_by_bpos,
			      enum ALGFLAG algf = USE_LPF_ORIGINAL);  
 public:
  
  // count runs in string s.
  static unsigned int countRuns(const std::string & s,
				enum ALGFLAG algf = USE_LPF_ORIGINAL);

  // find all runs in string s.
  // the result will be contained in runs.
  static void findRuns(const std::string & s,
		       std::vector<run> & runs,
		       enum ALGFLAG algf = USE_LPF_ORIGINAL);  
};

#endif//__RUN_FINDER_HPP__
