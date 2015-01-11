#include "cpuProperties.h"
#include <vector>
#include <chrono>
#include <stdio.h>
#include "cMeter.cpp"

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

uint8_t ReadArray(uint8_t* array,size_t size,size_t step)
{
  uint64_t res;
  for(size_t i=0; i<size;i+=step)
    res+=array[i];
  return res;
}

void CreateMemoryMountainFile()
{
  FILE* f=fopen("MemoryMountainDataDump.data","w");
  fprintf(f,"#%19s\t%19s\t%19s\n","Size","Step","MBytes/Secs");
  fclose(f);
}

double getBytesPerMicrosec(size_t size,size_t step, uint64_t time_ns)
{
  double res= (size/step);
  res/=time_ns;
  return res/NS_IN_MICROSECS;
}

void addToMemoryMountainFile(size_t size,size_t step, uint64_t time_ns)
{
  FILE* f=fopen("MemoryMountainDataDump.data","a");
  fprintf(f,"%19d\t%19d\t%30f\n",size,step,getBytesPerMicrosec(size,step,time_ns));
  fclose(f);
}

void addBlankLine()
{
  FILE* f=fopen("MemoryMountainDataDump.data","a");
  fprintf(f,"\n");
  fclose(f);
}

long InvalidateCache()
{///invalidates at least 8 mb cache
  long res=0;
  uint8_t* array=new uint8_t[BYTES_IN_MB *8];
  for(size_t i=0;i<BYTES_IN_MB *8;i++)
    res=array[i];
  delete[] array;
  return res;
}

void GenerateMemoryMountainData(size_t maxSize,size_t increaseStep)
{
  CreateMemoryMountainFile();
  auto stopWatchFunc =[] (){
    auto now = std::chrono::high_resolution_clock::now();
    uint64_t nanos = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count());
    return nanos;
  };
  double min=10,max=0;
  cMeter<uint64_t> stopWatch(stopWatchFunc);
  stopWatch.setUnitName("time_ns");
  stopWatch.setUnitSymbol("ns");
  uint64_t time_ns;
  
  int acc=0;
  uint8_t res;
  long aux=0;
  for (size_t arraySize=increaseStep;arraySize<maxSize;arraySize+=increaseStep)
  {
    uint8_t* array=new uint8_t[arraySize];
    for(size_t step=1;(step<513) && (step<=arraySize);step+=step)
    {
      std::cout<<"Starting tests for arraySize "<<arraySize<<" and step "<<step<<"\n";
     
      stopWatch.start();
      res=ReadArray(array,arraySize,step);
      stopWatch.stop();
      time_ns=stopWatch.peek();
      acc+=res;
      aux+=InvalidateCache();

      if(getBytesPerMicrosec(arraySize,step,time_ns)<min)min=getBytesPerMicrosec(arraySize,step,time_ns);
      if(getBytesPerMicrosec(arraySize,step,time_ns)>max)max=getBytesPerMicrosec(arraySize,step,time_ns);
       
      addToMemoryMountainFile(arraySize,step,time_ns);	
      std::cout<<aux<<" "<<acc<<"\n";//to be certain that the optimizer doesn't discard the values
      aux=0;
    }
    acc=0;
    delete[] array;
    addBlankLine();
  }
  std::cout<<"Min throughput "<<min<<"\t\tmax throughput "<<max<<"\n";
}