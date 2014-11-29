#include "cCPUMeterFunctions.h"


bool isRDTSCPsupported()
{
  return false;
}

uint64_t startRDTSC()
{
#ifdef _X86_CPU
uint32_t cycles_low=0, cycles_high=0;
asm volatile (
"CPUID\n\t"/*serialize*/
"RDTSC\n\t"/*read the clock*/
"mov %%edx, %0\n\t"
"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: 
"%rax", "%rbx", "%rcx", "%rdx");
#endif
#ifdef _AMD64_CPU
uint64_t cycles_low=0, cycles_high=0;
asm volatile (
"CPUID\n\t"/*serialize*/
"RDTSC\n\t"/*read the clock*/
"mov %%rdx, %0\n\t"
"mov %%rax, %1\n\t": "=r" (cycles_high), "=r"(cycles_low):: 
"%rax", "%rbx", "%rcx", "%rdx");
#endif
  uint64_t res=( ((uint64_t)cycles_high << 32) | cycles_low );
  return res;
}

uint64_t stopRDTSCP()
{
#ifdef _X86_CPU
    uint32_t cycles_low=0, cycles_high=0;
   __asm__ volatile("RDTSCP\n\t"
               "mov %%edx,%0\n\t"
               "mov %%eax,%1\n\t"
               "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low)::
               "%eax", "%ebx", "%ecx", "%edx");
#endif
#ifdef _AMD64_CPU
     uint64_t cycles_low=0, cycles_high=0;
   __asm__ volatile("RDTSCP\n\t"
               "mov %%rdx,%0\n\t"
               "mov %%rax,%1\n\t"
               "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low)::
               "%rax", "%rbx", "%rcx", "%rdx");
#endif
  uint64_t res=( ((uint64_t)cycles_high << 32) | cycles_low );
  return res;
}

uint64_t stopRDTSC()
{
#ifdef _X86_CPU
uint32_t cycles_low=0, cycles_high=0;
asm volatile (
"CPUID\n\t"/*serialize*/
"RDTSC\n\t"/*read the clock*/
"mov %%edx, %0\n\t"
"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: 
"%rax", "%rbx", "%rcx", "%rdx");
#endif
#ifdef _AMD64_CPU
uint64_t cycles_low=0, cycles_high=0;
asm volatile (
"CPUID\n\t"/*serialize*/
"RDTSC\n\t"/*read the clock*/
"mov %%rdx, %0\n\t"
"mov %%rax, %1\n\t": "=r" (cycles_high), "=r"(cycles_low):: 
"%rax", "%rbx", "%rcx", "%rdx");
#endif
  uint64_t res=( ((uint64_t)cycles_high << 32) | cycles_low );
  return res;
}




