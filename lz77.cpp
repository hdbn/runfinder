////////////////////////////////////////////////////////////////////////////////
//
// lz77.cpp
// routines for LZ factorization
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

#include <map>
#include <cassert>
#include <iostream>
#include <set>

#include "lz77.hpp"
#include "suffixArray.hpp"

using namespace std;

// linear time computation of s/lz-factorization using suffix and lcp arrays.
// my original two-pass version.
// there seem to be some algorithms that do it in one pass
// -----------------------------------------------------------------------
// Description of algorithm:
// for any given position i in suffix array, let
//   l = max { j | j < i, sa[j] < sa[i] } and
//   r = min { j | j > i, sa[j] < sa[i] }.
// the longest factor starting at i that appears
// in a previous position in text begins at positions
// either l or r, with length LCP(sa[l],sa[i]) or
// LCP(sa[r],sa[i]), whichever is longer. 
// For each position, the algorithm calculates l and r in each pass.
// -----------------------------------------------------------------------
static void LPF_original(const SuffixArrayAux & SAaux, 
			 vector<unsigned int> & POS,
			 vector<unsigned int> & LEN){
  unsigned int i, l, length = SAaux.size();
  const int * sa = SAaux.getSA();
  const std::vector<uInt> & LCP  = SAaux.getLCP();
  vector<pair<unsigned int, unsigned int> > S;
  pair<unsigned int, unsigned int> p;
  
  // for each position i, find largest i' < i with sa[i'] < sa[i]
  // the stack(vector) represents positions in increasing order.
  // first  elm: indices of positions
  // second elm: the lcp value to the previous element in stack.
  for(i = 0; i < length; i++){
    l = LCP[i];
    while(!S.empty() && sa[S.back().first] > sa[i]){ // pop while new element is smaller
      l = std::min(l, S.back().second); S.pop_back();
    }
    if(!S.empty() && l > 0){
      POS[sa[i]] = sa[S.back().first];
      LEN[sa[i]] = l;
    } else {
      POS[sa[i]] = sa[i];
      LEN[sa[i]] = 0;
    }
    p.first = i;
    p.second = l;
    S.push_back(p);
  }

		       
  // for each position i, find smallest i' > i with sa[i'] < sa[i]
  // the stack(vector) represents positions in increasing order.
  // first  elm: indices of positions
  // second elm: the lcp value to the next element in stack.
  S.clear();
  for(i = length; i-- > 0;){
    while(!S.empty() && sa[S.back().first] > sa[i]){
      p = S.back();
      l = p.second; S.pop_back();
      if(!S.empty()) S.back().second = min(l, S.back().second); 
    }
    if((!S.empty()) && ((l = S.back().second) > LEN[sa[i]])){
      POS[sa[i]] = sa[S.back().first];
      LEN[sa[i]] = l;
    }
    p.first = i;
    p.second = LCP[i];
    S.push_back(p);
  }
  
  // assure left most // this isn't actuall needed.
  for(i = 1; i < length; i++){
    if(LEN[i] > 0 && LEN[POS[i]] >= LEN[i])
      POS[i] = POS[POS[i]];
  }
}

void LZ77::lpf(const std::string & str, 
	       std::vector<unsigned int> & POS,
	       std::vector<unsigned int> & LEN,
	       enum ALGFLAG algf){
  SuffixArrayAux SAaux(str);
  POS = LEN = vector<unsigned int>(str.size(),0);
  switch(algf){
  case USE_LPF_ORIGINAL:
    LPF_original(SAaux, POS, LEN); break;
  default:
    assert(false);
  }
}
