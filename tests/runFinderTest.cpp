////////////////////////////////////////////////////////////////////////////////
//
// runFinderTest.cpp
// test routines for runFinder
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

#include <gtest/gtest.h>
#include <sys/time.h>
#include "../runFinder.hpp"
#include "../bits.h"

using namespace std;

TEST(runFinder, countRuns){
  runFinder rc;
  unsigned int c1;
  struct timeval btv, etv;  
  string s1;
  s1 = "110111010101110110";
  c1 = rc.countRuns(s1);
  EXPECT_EQ(c1, (unsigned int) 8);

  s1 = "1101011010010110101101001011010110011010110100101101011010010110101100101101011010010110101101001011010110011010110100101101011010010110101100101101011010010110101100101101001011010110100101101011001011010110100101101011010010110101100101101001011010110100101101011001011010110100101101011001011010010110101101001011010110010110101101001011010110100101101011001011010110100101101011001011010010110101101001011010110010110101101001011010110100101101011001011010010110101101001011010110010110101101001011010110010110100101101011010010110101100101101011010010110101101001011010110010110101101001011010110010110100101101011010010110101100101101011010010110101100101101001011010110100101101011001011010110100101101011010010110101100101101001011010110100101101011001011010110100101101011001011010010110101101001011010110010110101101001011010110100101101011001011010110100101101011001011010010110101101001011010110010110101101001011010110100101101011001011010010110101101001011010110010110101101001011010110010110100101101011010010110101100101101011010010110101101001011010110010110101101001011010110010110100101101011010010110101100101101011010010110101100101101001011010110100101101011001011010110100101101011010010110101100101101001011010110100101101011001011010110100101101011001011010010110101101001011010110010110101101001011010110100101101011001011010110100101101011001011010010110101101001011010110010110101101001011010110100101101011001011010010110101101001011010110010110101101001011010110010110100101101011010010110101100101101011010010110101101001011010";
  gettimeofday(&btv, NULL);
  c1 = rc.countRuns(s1);
  gettimeofday(&etv, NULL);
  EXPECT_EQ(c1, (unsigned int) 1455);
  return;
}
