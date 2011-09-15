////////////////////////////////////////////////////////////////////////////////
//
// lz77.hpp
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

#ifndef __LZ77_HPP__
#define __LZ77_HPP__
#include <vector>
#include <string>

enum ALGFLAG {
  USE_LPF_ORIGINAL,   // use original CPS algorithm for calculating longest previous factor
};

class LZ77 {
public:
  // calculate longest previous factor (position and length)
  // for each position of string str.
  static void lpf(const std::string & str,
		  std::vector<unsigned int> & POS,
		  std::vector<unsigned int> & LEN,
		  enum ALGFLAG = USE_LPF_ORIGINAL);
};

#endif//__LZ77_HPP__

