#include "cCPUMeterFunctions.h"

/*
bool isRDTSCPsupported()
{
  return false;
}

uint64_t startRDTSC()
{
  uint32_t cycles_low=0, cycles_high=0;
#ifdef _X86_CPU
  asm volatile("CPUID\n\t"
	       "RDTSC\n\t"
	       "mov %%edx, %0\n\t"
	       "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
	       "%rax", "%rbx", "%rcx", "%rdx");
#endif
#ifdef _AMD64_CPU
  asm volatile("CPUID\n\t"
	       "RDTSC\n\t"
	       "mov %%rdx, %0\n\t"
	       "mov %%rax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
	       "%rax", "%rbx", "%rcx", "%rdx");
#endif
  uint64_t res=( ((uint64_t)cycles_high << 32) | cycles_low );
  return res;
}

uint64_t stopRDTSCP()
{
  uint32_t cycles_low=0, cycles_high=0;
#ifdef _X86_CPU
  asm volatile("RDTSCP\n\t"
               "mov %%edx,%0\n\t"
               "mov %%eax,%1\n\t"
               "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low)::
               "%rax", "%rbx", "%rcx", "%rdx");
#endif
#ifdef _AMD64_CPU
  asm volatile("RDTSCP\n\t"
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
  uint32_t cycles_low=0, cycles_high=0;
#ifdef _X86_CPU
  asm volatile("mov %%cr0, %%rax\n\t"
               "mov %%rax, %%cr0\n\t"
               "RDTSC\n\t"
               "mov %%edx, %0\n\t"
               "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
               "%rax", "%rdx");
#endif
#ifdef _AMD64_CPU
  asm volatile("mov %%cr0, %%rax\n\t"
               "mov %%rax, %%cr0\n\t"
               "RDTSC\n\t"
               "mov %%rdx, %0\n\t"
               "mov %%rax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
               "%rax", "%rdx");
#endif
  uint64_t res=( ((uint64_t)cycles_high << 32) | cycles_low );
  return res;
}
*/




