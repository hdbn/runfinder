////////////////////////////////////////////////////////////////////////////////
//
// suffixArray.hpp
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

#ifndef __SUFFIX_ARRAY_HPP__
#define __SUFFIX_ARRAY_HPP__

#include <string>
#include <vector>

typedef unsigned int uInt;

class SuffixArrayAux {
  int * SA;
  const std::string & t;
  std::vector<uInt> ranka;
  std::vector<uInt> lcpa;  
  void calcRankLcp();
public:
  SuffixArrayAux();
  // construct rank, lcp, suffix arrays for string s
  SuffixArrayAux(const std::string & s);
  ~SuffixArrayAux();
  uInt size() const { return t.size(); }
  const int * getSA() const { return SA; }
  const std::vector<uInt> & getLCP() const { return lcpa; }
  const std::vector<uInt> & getRANK() const { return ranka; }
  const std::string & text() const { return t; };
};

#endif//__SUFFIX_ARRAY_HPP__
