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

#endif