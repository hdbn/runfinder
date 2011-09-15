////////////////////////////////////////////////////////////////////////////////
//
// bitsTest.cpp
// test routines for bit run counting
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
#include <cstdlib>
#include <cstring>
#include <string>
#include "../bits.h"
#include "../runFinder.hpp"

using namespace std;

static const unsigned int max_run_length = 100;

void bits2str(BVEC v, unsigned int l, string & s){
  unsigned int i;
  s.clear();
  for(i = 0; i < l; i++){
    s.push_back((v % 2) ? '1' : '0');
    v = v >> 1;
  }
}

// test results of prefix, position and sieve for all bit strings of certain lengths
TEST(bitsTest, count){
  BVEC i, maxrunv, mask = 1;
  unsigned int c, c1, c2, c3, c4, counts[max_run_length], maxrun, len;
  struct timeval stv, btv, etv;
  unsigned int startbits = 4, endbits = 24;
  runFinder rc;
  string s;

  gettimeofday(&stv, NULL);
  // printf("sizeof(BVEC) = %lu\n", sizeof(BVEC));
  // printf("Bit length: %d\n", NUM_BITS);

  // simply count all runs for all 2^len bit strings 
  // where len = startbits to endbits
  for(len = startbits; len <= endbits; len++){ // for each bit length
    printf("Testing length: %d\n", len);
    // printf("================================================================================\n");
    memset(counts, 0, sizeof(unsigned int) * max_run_length);      // zero clear
    gettimeofday(&btv, NULL);
    mask = (len == sizeof(BVEC) * 8) ? -1 : ((((BVEC) 1) << len) - 1);
    // print_bvec(mask); printf(": <- mask, len = %2u\n", len);
    maxrunv = maxrun = i = 0;
    do {
      c1 = count_runs_bits_prefix(i, len);
      c2 = count_runs_bits_position(i, len);
      c3 = count_runs_bits_sieve(i, len);
      bits2str(i, len, s);
      c4 = rc.countRuns(s);
      EXPECT_EQ(c1, c2);
      if(c1 != c2){
	print_bvec_masked(i, mask);
	printf(": <- MISMATCH: prefix = %d, while position = %d\n", c1, c2);
      }
      EXPECT_EQ(c2, c3);
      if(c2 != c3){ 
	print_bvec_masked(i, mask);
	printf(": <- MISMATCH: position = %d, while sieve = %d\n", c2, c3); 
      }
      EXPECT_EQ(c3, c4);
      if(c3 != c4){ 
	print_bvec_masked(i, mask);
	printf(": <- MISMATCH: sieve = %d, while rc = %d\n", c3, c4); 
      }
      c = c1;
      if(c > maxrun){ maxrun = c; maxrunv = i; }
      counts[c] += 1;
    } while((i+=1) != ((len < NUM_BITS) ? (((BVEC) 1) << len) : 0));
    gettimeofday(&etv, NULL);
    printf("Time: approx %.3f seconds\n", timediff(btv, etv));
    // printf("Histogram of run counts\n");
    // for(i = 0; i < max_run_length; i++){
    //   if(counts[i] > 0) printf("%2lu: %u\n", i, counts[i]);
    // }
    // printf("First string with max runs = %2u for len = %2u -----------------------------------\n", maxrun, len);
    // print_bvec_masked(maxrunv, mask); printf("\n\n");
    // gettimeofday(&etv, NULL);
    // printf("Total Time: approx %.3f seconds\n", timediff(stv, etv));
    // fflush(stdout);
  }
  return;
}
