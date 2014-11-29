#ifndef _CPU_METER_FUNCTIONS_H
#define _CPU_METER_FUNCTIONS_H

#include <stdint.h>


bool isRDTSCPsupported();
uint64_t startRDTSC();

uint64_t stopRDTSCP();
uint64_t stopRDTSC();



///Simple function, the other complexer functions defined in cCPUMeterFuntions.cpp
///give me problems when compiling for AMD64 architecture. If compiled for X86 32 bit
///Architecture I get a segmentation fault. If compiled for AMD64 i get compiler errors
///saying Error: unsupported instruction `mov'
__inline__ uint64_t RDTSC() {
#ifdef  _X86_CPU
  uint64_t x=0;
  __asm__ volatile ("rdtsc" : "=A" (x));
  return x;
#elif _AMD64_CPU
  uint64_t a=0, d=0;
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  return (d<<32) | a;
#endif
  return 0;
}

#endif
