#include "cpuProperties.h"
#include <vector>

void PrintCacheLevels()
{
  std::vector<cacheDescription> cacheDescriptions;
  uint32_t EAX=1,EBX,ECX,EDX;
  uint32_t cnt=0;
  while ((EAX & REGISTER_CONTAINS_VALID_INFORMATION_MASK)!=0x00)
  {
    __asm__ __volatile__(
	"MOV %3, %%eax\n\t"
	"MOV %4, %%ecx\n\t"
	"CPUID\n\t"
	"MOV %%eax, %0\n\t"
	"MOV %%ebx, %1\n\t"
	"MOV %%ecx, %2\n\t"
	:"=r"(EAX),"=r"(EBX),"=r"(ECX)
	:"r" (GET_CACHE_INFO), "r" (cnt)
	:"%eax","%ebx","%ecx","%edx"
      );
    cnt++;
    if((EAX & REGISTER_CONTAINS_VALID_INFORMATION_MASK)!=0x00)
      cacheDescriptions.push_back(decodeCacheDescription(EAX,EBX,ECX,EDX));
  }
  for(size_t i=0; i<cacheDescriptions.size(); i++)
  {
    std::cout<<"Level "<<cacheDescriptions[i].cacheLevel;
    switch(cacheDescriptions[i].cacheType)
    {
      case CacheType::DataCache:
	 std::cout<<" Data Cache: ";
	 break;
      case CacheType::InstructionCache:
	 std::cout<<" Instruction Cache: ";
	 break;
      case CacheType::UnifiedCache:
	 std::cout<<" Unified Cache: ";
	 break;
      case CacheType::Other:
	 std::cout<<" Other Memory type: ";
	 break;
    }
    std::cout<<cacheDescriptions[i].sets<<" sets with "
	      <<cacheDescriptions[i].associativity<<" way associativity,"
	      <<" line size "<<cacheDescriptions[i].lineSize<<" bytes [Total size: "<<CalculateCacheSiez(cacheDescriptions[i])<<" bytes]\n";
  }
}

cacheDescription decodeCacheDescription(uint32_t eax,uint32_t ebx,uint32_t ecx,uint32_t edx)
{
  cacheDescription res;
  res.cacheType=static_cast<CacheType>(eax & REGISTER_CONTAINS_VALID_INFORMATION_MASK);
  res.lineSize=(ebx & CACHE_LINE_SIZE_MASK)+1;
  res.partitions=((ebx & CACHE_PARTITIONS_MASK)>>12)+1;
  res.associativity=((ebx & CACHE_ASSOCIATIVITY_MASK)>>22)+1;
  res.cacheLevel=((eax & CACHE_LEVEL_MASK)>>5);
  res.sets=ecx+1;
  return res;
}

uint32_t CalculateCacheSiez(cacheDescription desc)
{
  return desc.associativity*desc.partitions*desc.lineSize*desc.sets;
}