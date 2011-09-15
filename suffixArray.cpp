////////////////////////////////////////////////////////////////////////////////
//
// suffixArray.cpp
// routines for suffix array
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

#include "suffixArray.hpp"

// use divsufsort library by Yuta Mori
#include "divsufsort.h"

using namespace std;

SuffixArrayAux::SuffixArrayAux(const string & s) 
  : t(s), ranka(s.size()), lcpa(s.size())
{
  SA = new int[s.size()];
  divsufsort(reinterpret_cast<const unsigned char *>(s.c_str()), SA, s.size());
  this->calcRankLcp();
}

void SuffixArrayAux::calcRankLcp(){
  uInt i, j, h, x;
  const char * text = t.c_str();
  const char * ep = t.c_str() + t.size();

  // compute rank array
  for(i = 0; i < t.size(); i++) ranka[SA[i]] = i;

  // compute lcp array
  for(h = i = 0; i < t.size(); i++){
    x = ranka[i];
    if(x > 0){
      const char * p0, * p1;
      j = SA[x-1];
      p1 = text + i + h;
      p0 = text + j + h;
      while((p0 != ep) && (p1 != ep) && (*p1 == *p0)){
        p1++; p0++; h++;
      }
      lcpa[x] = h;
      if(h > 0) h--;
    }
  }
  return;
}

SuffixArrayAux::~SuffixArrayAux(){
  delete [] SA;
}
