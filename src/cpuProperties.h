#ifndef CPU_PROPERTIES_H
#define CPU_PROPERTIES_H

#include <iostream>
#include <string>

///MASKS
#define REGISTER_CONTAINS_VALID_INFORMATION_MASK 0x1F
#define CACHE_LEVEL_MASK (0x07<<5)
#define CACHE_LINE_SIZE_MASK 0xFFF
#define CACHE_ASSOCIATIVITY_MASK (0x1FF<<22)
#define CACHE_PARTITIONS_MASK (0x1FF<<12)
#define NS_IN_SECONDS 1000000000
#define NS_IN_MICROSECS 1000
#define BYTES_IN_GB (0x01<<30)
#define BYTES_IN_MB (0x01<<20)
#define BYTES_IN_KB (0x01<<10)


///Commands
#define GET_CACHE_INFO 0x04

enum CacheType {
	    DataCache=1,
	    InstructionCache=2,
	    UnifiedCache=3,
	    Other
};

struct cacheDescription{
      CacheType cacheType;
      uint32_t cacheLevel;
      uint32_t associativity;
      uint32_t lineSize;
      uint32_t partitions;
      uint32_t sets;
};

void PrintCacheLevels();
cacheDescription decodeCacheDescription(uint32_t,uint32_t,uint32_t,uint32_t);
uint32_t CalculateCacheSiez(cacheDescription);

void GenerateMemoryMountainData(size_t,size_t);
uint8_t ReadArray(uint8_t*,size_t,size_t); 
#endif