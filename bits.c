////////////////////////////////////////////////////////////////////////////////
//
// bits.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

#include "bits.h"

unsigned int NUM_BITS = sizeof(BVEC) * 8;

void print_bvec(BVEC v){
  unsigned int i;
  printf("0x%016lx:", v);
  for(i = 0; i < NUM_BITS; i++){
    if(!(i % 8)) printf(" ");
    printf("%c", '0' + (int) ((v >> (NUM_BITS-i-1)) & 1));
  }
  return;
}

void print_bvec_masked(BVEC v, BVEC mask){
  unsigned int i;
  printf("0x%016lx:", v);
  for(i = 0; i < NUM_BITS; i++){
    if(!(i % 8)) printf(" ");
    if((mask>>(NUM_BITS-i-1)) & 1){
      printf("%c", '0' + (int) ((v>>(NUM_BITS-i-1)) & 1));
    } else {
      printf("*");
    }
  }
  return;
}

double timediff(struct timeval s, struct timeval e){
  return ((((float)(e.tv_sec - s.tv_sec)) + ((float)(e.tv_usec - s.tv_usec))/1000000));
}


// count the number of contiguous ones
static inline unsigned int one_runs(BVEC v){
  unsigned int count = 0;
  while(v){
    // v &= (v + (1 << __builtin_ctz(v)));
    v &= (v | (v-1)) + 1; // hacker's delight
    count++;
  }
  return count;
}

// SELF_AND: do the operation log (NUM_BITS) times
static inline BVEC self_and(BVEC v, unsigned int k){
  unsigned int s;
  s = k >> 1; v &= (v >> s); k -= s;
  s = k >> 1; v &= (v >> s); k -= s;
  s = k >> 1; v &= (v >> s); k -= s;
  s = k >> 1; v &= (v >> s); k -= s;
#ifdef BITS64
  s = k >> 1; v &= (v >> s); k -= s;
#endif
  s = k >> 1; v &= (v >> s);
  return v;
}

inline unsigned int count_runs_bits_sieve(BVEC v, unsigned int len){
  BVEC mask = (len == sizeof(BVEC) * 8) ? ((unsigned int) -1) : ((((BVEC) 1) << len) - 1);
  len /= 2;                                 // divide length by 2
  unsigned int count = 0, period, hperiod;
  BVEC tmpvec;
  BVEC p_vec[len+1]; // preserve periods  
  // obtain periods
  for(period = 1; period <= len; period++){ // for each period 1 to len
    p_vec[period] = (v ^ ((~v) >> period)) & (mask >> period);
  }
  
  // remove non-primitive runs
  for(period = 1; period <= len; period++){ // for each period 1 to len
    tmpvec = self_and(p_vec[period], period);
    count += one_runs(tmpvec); // it is OK to count this period
    // now sieve the multiples of this period  
    for (hperiod = 2*period; hperiod <= len; hperiod += period ){
      if ( (tmpvec = tmpvec & (tmpvec >> (period))) == 0 ) break;
      p_vec[hperiod] ^= tmpvec;
    }
  }
  return count;
}

// count the number of runs in bit vector v.
// mask is: 0^{NUM_BITS-len}1^len
inline unsigned int count_runs_bits_position(BVEC v, unsigned int len){
  BVEC mask = (len == sizeof(BVEC) * 8) ? ((unsigned int) -1) : ((((BVEC) 1) << len) - 1);
  BVEC runs_by_bpos[len-1], x, tmpvec, tmpvec2;
  unsigned int period, count = 0, bp;
  memset(runs_by_bpos, 0, sizeof(BVEC) * (len-1));          // zero clear
  len /= 2;                                               // divide length by 2
  for(period = 1; period <= len; period++){               // for each period 1 to len
    x = (v ^ ((~v) >> period)) & (mask >> period);
    tmpvec = self_and(x, period);                         // repeats become runs of 1
    while(tmpvec){
      // find beginning position and end position of rightmost run
      // in tmpvec and mark accordingly
      bp = __builtin_ctzl(tmpvec);                        // beginning position of run
      tmpvec2 = tmpvec + (((BVEC) 1) << bp);              // ...0111100 to ...1000000
      tmpvec = tmpvec & tmpvec2;                          // clear righmost run of tmpvec
      tmpvec2 &= -tmpvec2;                                // retain only rightmost bit
      tmpvec2 = tmpvec2 << ((period - 1) << 1);           // shift it to end position      
      count += (runs_by_bpos[bp] & tmpvec2) ? 0 : 1;      // only add count if it was previously un-marked
      runs_by_bpos[bp] |= tmpvec2;
    }
  }
  return count;
}

inline unsigned int count_runs_bits_prefix(BVEC w, int length){
  int numOfRuns = 0;
  int startPos;
  for( startPos=0; startPos < length-1; ++startPos ) {
    BVEC aliveMask = 0;
    BVEC alive = (((BVEC) 1) << ((length - startPos)/2))-1;  //  typical form is 00000000011111
    BVEC bitmask = (~0);        
    int toggle = 0;    //  0 or 1, alternatively
    BVEC p = ((BVEC) 1) << (length - startPos - 1);
    BVEC nextBit = (w & p);
    BVEC bitMaskForXOR = 1;
    BVEC lastBit, lastAlive;
    if ( startPos > 0 ) {
      lastBit = (w & (p << 1)) >> 1;
      if ( (lastBit == nextBit) ) {  // same
	bitmask --;
      }
      alive &= bitmask;  
      bitMaskForXOR = 3;  //  bitMaskForXOR = ( bitMaskForXOR << 1 ) | 1 
    }
    p >>= 1;
    lastBit = nextBit >> 1;
    while( alive ) {
      bitmask = ( bitmask << 1 ) | 1;
      
      if ( (nextBit = (w & p)) != lastBit) { // different 
	bitmask ^= bitMaskForXOR;
      }
      lastBit = nextBit >> 1;
      lastAlive = alive;
      alive &= bitmask;
      if ( (lastAlive ^ alive) & aliveMask ) {
	numOfRuns ++;
      }
      if ( (p >>= 1) == 0 ) {
	break;
      }
      if ((toggle ^= 1) != 0 ) {
	aliveMask = (aliveMask << 1) | 1;
      }
      bitMaskForXOR = (bitMaskForXOR) << 1 | 1;
    }
    if (alive) {
      numOfRuns ++;
    }
  }
  return numOfRuns;
}
