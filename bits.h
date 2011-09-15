////////////////////////////////////////////////////////////////////////////////
//
// bits.h
// counting runs on short strings using bit parallelism
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

#ifndef __BITS_H__
#define __BITS_H__
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long BVEC; // a type for bit vectors
extern unsigned int NUM_BITS;

#if defined(__LP64__) || defined(_LP64)
#define BITS64
#endif

// print a bit vector to stdout with least significant bit on the right
void print_bvec(BVEC v);

// print a bit vector to stdout with least significant bit on the right
// but masking the digits with '*' where the mask bit = 1
void print_bvec_masked(BVEC v, BVEC mask);

// return the difference in time in seconds between s and e
// (returns negative value if s is after e).
double timediff(struct timeval s, struct timeval e);

// count the number of runs in the least significant len bits of bit vector v,
// using prefix run method as described in:
// K. Hirashima, H. Bannai, W. Matsubara, A. Ishino and A. Shinohara,
// "Bit-parallel algorithms for computing all the runs in a string",
// Proc. Prague Stringology Conference 2009 (PSC 2009), 203-213, (August 2009).
unsigned int count_runs_bits_prefix(BVEC v, int length);

// count the number of runs in the least significant len bits of bit vector v,
// using position run method
// K. Hirashima, H. Bannai, W. Matsubara, A. Ishino and A. Shinohara,
// "Bit-parallel algorithms for computing all the runs in a string",
// Proc. Prague Stringology Conference 2009 (PSC 2009), 203-213, (August 2009).
unsigned int count_runs_bits_position(BVEC v, unsigned int len);

// count the number of runs in the least significant len bits of bit vector v,
// using sieve method
// K. Hirashima, H. Bannai, W. Matsubara, A. Ishino and A. Shinohara,
// "Bit-parallel algorithms for computing all the runs in a string",
// Proc. Prague Stringology Conference 2009 (PSC 2009), 203-213, (August 2009).
unsigned int count_runs_bits_sieve(BVEC v, unsigned int len);

#ifdef __cplusplus
};
#endif

#endif//__BITS_H__
