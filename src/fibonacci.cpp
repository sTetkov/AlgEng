/// \brief This is a implementation of three different algorithms
///        for the calculation of the Fibonacci number.
///
///This program implements three different algorithms for the calculation of the fibonacci number
/// f(n) where f(n) = f(n-1) + f(n-2) and f(0) = 0 and f(1) = 1.
///
///

#include "strcMatrix.h"
#include "cMeter.h"
#include <cmath>
#include <ctime>
#include <functional>
#include "cMeterDataDump.h"
#include "cCPUMeterFunctions.h"
#include "cSortableArray.h"

///Ugly but at the moment necessary to allow the compilation of templates
#include "cMeter.cpp"
#include "cMeterDataDump.cpp"
#include "cSortableArray.cpp"


#ifdef TEST_RUN
#include "gtest/gtest.h"
#include <random>
#endif


#define _MAX_FIB_NUMBER 93
#define _MAX_FIB_NUMBER_DERIVED_FUNCTION 76
#define _RND_NUMBER_GEN_SEED 42
#define _MAX_VECTOR_SIZE_NUMBER 100
#define _CPU_CYCLES_MEASUREMENT "CPU_cycles"
#define _TIME_UNITS_MEASUREMENT "Time_ticks"
#define _NORMAL_DIST "normal_distribution"
#define _SORTED_ARRAY "sorted_array"
#define _REVERSE_SORTED_ARRAY "reverse_sorted_array"
#define _SCRAMBLED_ARRAY "scrambled_array"
#define _REPEATED_ELEMENTS_ARRAY "array_with_repeated_elements"
#define _EMPTY_STRING ""
///
///Prototypes
///
unsigned long fibonacciRet(unsigned int);
unsigned long fibonacciArray(unsigned int);
unsigned long fibonacciConst(unsigned int);
strcMatrix expBySquaring(unsigned int);
unsigned long fibonacciExpBySquare(unsigned int);
unsigned long fibonacciDerivedFormula(unsigned int);
unsigned long fibonacciExpBySquareConsMem(unsigned int);
void initializeMeterFunctions();

void generateFibonacciLookUpTable(unsigned int);
unsigned long getLookUpTableFibonacciValue(unsigned int);
unsigned long fibLookupTable[94];

#ifdef TEST_RUN
std::vector<int> generateRandomVector(int, int);
std::vector<int> generateRandomVector(int, int, std::string);
std::string generateFileName(std::string, std::string, std::string);
#endif

cMeter<clock_t>* stopWatch;
std::function<clock_t()> stopWatchFunc;
cMeterDataDump<clock_t>* stopWatchDataDump;
cMeter<uint64_t>* cpuMeter;
std::function<uint64_t()> startCPUMeterFunc;
std::function<uint64_t()> stopCPUMeterFunc;
cMeterDataDump<uint64_t>* cpuCycleDataDump;

int main(
         #ifdef TEST_RUN
         int argc, char **argv
         #endif
         )
{
    ///There are macros that trick the preprocessor in generating a look up table using constexpr, but they are a little bit a mess.
    ///I will include them once the rest of the code is fixed. For the moment I generate the table at runtime.
    generateFibonacciLookUpTable(_MAX_FIB_NUMBER);
    #ifdef TEST_RUN
    ::testing::InitGoogleTest(&argc, argv);
    initializeMeterFunctions();
    return RUN_ALL_TESTS();
    #else
    return 0;
    #endif // TEST_RUN
}

/// \brief This implements the classic recursive algorithm for the calculation of the fibonacci number.
///
///This algorithm, while quite straightforward, is extremely inefficient, having a runtime of 2^n and a similar
///memory usage.
unsigned long fibonacciRet(unsigned int n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value" << n << "was passed to fibonacciRet" ;
    #endif
    if(n<2)
        return static_cast<unsigned long>(n);
    return fibonacciRet(n-1)+fibonacciRet(n-2);
}

#ifdef TEST_RUN
TEST (FibonacciTest,FibonacciRecursiveFunction)
{
        ASSERT_EQ(fibonacciRet(0),0);
        ASSERT_EQ(fibonacciRet(1),1);
        ASSERT_EQ(fibonacciRet(2),1);
        ASSERT_EQ(fibonacciRet(3),2);
        ASSERT_EQ(fibonacciRet(4),3);
        ASSERT_EQ(fibonacciRet(5),5);
        ASSERT_EQ(fibonacciRet(6),8);
        ASSERT_EQ(fibonacciRet(7),13);
        ASSERT_EQ(fibonacciRet(8),21);
        ASSERT_EQ(fibonacciRet(9),34);
        ASSERT_EQ(fibonacciRet(10),55);
}
#endif // TEST_RUN
///\brief This implements an algorithm for the calculation of the fibonacci number that uses an array to
///have a linear runtime and memory usage
///
///In this case we usa an array to store all intermediate values used for the calculation.
unsigned long fibonacciArray(unsigned int n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value" << n << "was passed to fibonacciArray";
    #endif
    if(n<2)
        return static_cast<unsigned long>(n);
    unsigned long *piVec=new unsigned long[n];
    piVec[0]=0;
    piVec[1]=1;
    for (int i=2;i<n;i++)
        piVec[i]=piVec[i-1]+piVec[i-2];
    unsigned long ret=piVec[n-1]+piVec[n-2];
    delete[] piVec;
    return ret;
}

#ifdef TEST_RUN
TEST (FibonacciTest,FibonacciArrayFunction)
{
        ASSERT_EQ(fibonacciArray(0),0);
        ASSERT_EQ(fibonacciArray(1),1);
        ASSERT_EQ(fibonacciArray(2),1);
        ASSERT_EQ(fibonacciArray(3),2);
        ASSERT_EQ(fibonacciArray(4),3);
        ASSERT_EQ(fibonacciArray(5),5);
        ASSERT_EQ(fibonacciArray(6),8);
        ASSERT_EQ(fibonacciArray(7),13);
        ASSERT_EQ(fibonacciArray(8),21);
        ASSERT_EQ(fibonacciArray(9),34);
        ASSERT_EQ(fibonacciArray(10),55);
}
#endif

///\brief This implements an algorithm for the calculation of the fibonacci number that has a
///linear runtime and a constant memory usage.
unsigned long fibonacciConst(unsigned int n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value" << n << "was passed to fibonacciConst";
    #endif
    if (n<2)
        return static_cast<unsigned long>(n);
    unsigned long fib1=0;
    unsigned long fib2=1;
    unsigned long ret=0;
    for (int i=2;i<n+1;i++)
    {
        ret=fib1+fib2;
        fib1=fib2;
        fib2=ret;
    }
    return ret;
}

#ifdef TEST_RUN
TEST (FibonacciTest,FibonacciConstFunction)
{
        ASSERT_EQ(fibonacciConst(0),0);
        ASSERT_EQ(fibonacciConst(1),1);
        ASSERT_EQ(fibonacciConst(2),1);
        ASSERT_EQ(fibonacciConst(3),2);
        ASSERT_EQ(fibonacciConst(4),3);
        ASSERT_EQ(fibonacciConst(5),5);
        ASSERT_EQ(fibonacciConst(6),8);
        ASSERT_EQ(fibonacciConst(7),13);
        ASSERT_EQ(fibonacciConst(8),21);
        ASSERT_EQ(fibonacciConst(9),34);
        ASSERT_EQ(fibonacciConst(10),55);
}
#endif


///\brief Implements exponentiation by squaring for matrix | 0 | 1 |
///                                                        | 1 | 1 |
strcMatrix expBySquaring(unsigned int n)
{
    if(n>2)
    {
        if(n%2==0)
        {
            strcMatrix baseM=expBySquaring(n/2);
            strcMatrix resM;
            resM.multiply(baseM,baseM);
            return resM;
        }
        else
        {
            strcMatrix expM=expBySquaring((n-1)/2);
            strcMatrix auxM;
            strcMatrix baseM;
            strcMatrix resM;
            *baseM(0,1)=1;
            *baseM(1,0)=1;
            *baseM(1,1)=1;
            auxM.multiply(expM,expM);
            resM.multiply(baseM,auxM);
            return resM;
        }
    }

    strcMatrix baseM;
    *baseM(0,1)=1;
    *baseM(1,0)=1;
    *baseM(1,1)=1;
    if(n==1)
        return baseM;
    strcMatrix resM;
    resM.multiply(baseM,baseM);
    return resM;
}

#ifdef TEST_RUN
TEST (FibonacciTest,expBySquareFunction)
{
    strcMatrix m=expBySquaring(2);
    ASSERT_EQ(*m(0,0),1)<< "expBySquareFunction(2)(0,0) returned "<<*m(0,0)<< " instead of "<<1;
    ASSERT_EQ(*m(0,1),1)<< "expBySquareFunction(2)(0,1) returned "<<*m(0,1)<< " instead of "<<1;
    ASSERT_EQ(*m(1,0),1)<< "expBySquareFunction(2)(1,0) returned "<<*m(1,0)<< " instead of "<<1;
    ASSERT_EQ(*m(1,1),2)<< "expBySquareFunction(2)(1,1) returned "<<*m(1,1)<< " instead of "<<2;

    m=expBySquaring(3);
    ASSERT_EQ(*m(0,0),1)<< "expBySquareFunction(3)(0,0) returned "<<*m(0,0)<< " instead of "<<1;
    ASSERT_EQ(*m(0,1),2)<< "expBySquareFunction(3)(0,1) returned "<<*m(0,1)<< " instead of "<<2;
    ASSERT_EQ(*m(1,0),2)<< "expBySquareFunction(3)(1,0) returned "<<*m(1,0)<< " instead of "<<2;
    ASSERT_EQ(*m(1,1),3)<< "expBySquareFunction(3)(1,1) returned "<<*m(1,1)<< " instead of "<<3;
}
#endif

///\brief Claculates the n-th fibonacci number using matrixes and exponentiation by square in logarithmic time

unsigned long fibonacciExpBySquare(unsigned int n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value" << n << "was passed to fibonacciExpBySquare";
    #endif
    if (n<2)
        return static_cast<unsigned long>(n);
    strcMatrix m=expBySquaring(n);
    unsigned long ret=*m(0,1);
    return ret;
}

#ifdef TEST_RUN
TEST (FibonacciTest,fibonacciExpBySquareFunction)
{
        ASSERT_EQ(fibonacciExpBySquare(0),0)<< "fibonacciExpBySquare returned "<<fibonacciExpBySquare(0)<< " instead of "<<0;
        ASSERT_EQ(fibonacciExpBySquare(1),1)<< "fibonacciExpBySquare returned "<<fibonacciExpBySquare(1)<< " instead of "<<1;
        ASSERT_EQ(fibonacciExpBySquare(2),1)<< "fibonacciExpBySquare returned "<<fibonacciExpBySquare(2)<< " instead of "<<1;
        ASSERT_EQ(fibonacciExpBySquare(3),2)<< "fibonacciExpBySquare returned "<<fibonacciExpBySquare(3)<< " instead of "<<2;
        ASSERT_EQ(fibonacciExpBySquare(4),3)<< "fibonacciExpBySquare returned "<<fibonacciExpBySquare(4)<< " instead of "<<3;
        ASSERT_EQ(fibonacciExpBySquare(5),5)<< "fibonacciExpBySquare returned "<<fibonacciExpBySquare(5)<< " instead of "<<5;
        ASSERT_EQ(fibonacciExpBySquare(6),8)<< "fibonacciExpBySquare returned "<<fibonacciExpBySquare(6)<< " instead of "<<8;
        ASSERT_EQ(fibonacciExpBySquare(7),13)<< "fibonacciExpBySquare returned "<<fibonacciExpBySquare(7)<< " instead of "<<13;
        ASSERT_EQ(fibonacciExpBySquare(8),21)<< "fibonacciExpBySquare returned "<<fibonacciExpBySquare(8)<< " instead of "<<21;
        ASSERT_EQ(fibonacciExpBySquare(9),34)<< "fibonacciExpBySquare returned "<<fibonacciExpBySquare(9)<< " instead of "<<34;
        ASSERT_EQ(fibonacciExpBySquare(10),55)<< "fibonacciExpBySquare returned "<<fibonacciExpBySquare(10)<< " instead of "<<55;
}
#endif


///\brief Calculates n-th fibonacci number using the derived formula in linear time.

unsigned long fibonacciDerivedFormula(unsigned int n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER_DERIVED_FUNCTION)<< "A excessive value"<< n << "was passed to fibonacciDerivedFormula";
    #endif
    if (n<2)
        return static_cast<unsigned long>(n);
    unsigned long res=static_cast<unsigned long>(floor( (1/sqrt(5))*(pow(((1+sqrt(5))/2),n))+0.5f ));
    return res;
}
#ifdef TEST_RUN
TEST (FibonacciTest,fibonacciDerivedFormulaFunction)
{
        ASSERT_EQ(fibonacciDerivedFormula(0),0);
        ASSERT_EQ(fibonacciDerivedFormula(1),1);
        ASSERT_EQ(fibonacciDerivedFormula(2),1);
        ASSERT_EQ(fibonacciDerivedFormula(3),2);
        ASSERT_EQ(fibonacciDerivedFormula(4),3);
        ASSERT_EQ(fibonacciDerivedFormula(5),5);
        ASSERT_EQ(fibonacciDerivedFormula(6),8);
        ASSERT_EQ(fibonacciDerivedFormula(7),13);
        ASSERT_EQ(fibonacciDerivedFormula(8),21);
        ASSERT_EQ(fibonacciDerivedFormula(9),34);
        ASSERT_EQ(fibonacciDerivedFormula(10),55);
}
#endif

///\brief Is calculated on compile time. Uses ternary operator as it can
///       have only one return statement.
///       Usage found here: --> http://www.cprogramming.com/c++11/c++11-compile-time-processing-with-constexpr.html
constexpr unsigned long fibonacciCompileTime (unsigned int n)
{

    return n > 1 ? fibonacciConst(static_cast<int>(n)) : n;
}

#ifdef TEST_RUN
TEST (FibonacciTest,fibonacciCompileTimeBuildTableFunction)
{
    ASSERT_EQ(fibonacciCompileTime(0),0);
    ASSERT_EQ(fibonacciCompileTime(10),55);
    ASSERT_EQ(fibonacciCompileTime(20),6765);
    ASSERT_EQ(fibonacciCompileTime(30),832040);
    ASSERT_EQ(fibonacciCompileTime(40),102334155);
    ASSERT_EQ(fibonacciCompileTime(50),12586269025);
    ASSERT_EQ(fibonacciCompileTime(60),1548008755920);
    ASSERT_EQ(fibonacciCompileTime(70),190392490709135);
    ASSERT_EQ(fibonacciCompileTime(80),23416728348467685);
    ASSERT_EQ(fibonacciCompileTime(90),2880067194370816120);
}
#endif

void generateFibonacciLookUpTable(unsigned int n)
{
  for (int i=0;i<(n+1);i++)
    fibLookupTable[i]=fibonacciConst(i);
}

unsigned long getLookUpTableFibonacciValue(unsigned int n)
{
  #ifdef TEST_RUN
  EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value"<< n << "was passed to getLookUpTableFibonacciValue";
  #endif
  return fibLookupTable[n];
}

/// \brief: fibonacci number Exponentiation By Squaring using constant memory
///
unsigned long fibonacciExpBySquareConsMem(unsigned int n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value" << n << "was passed to fibonacciExpBySquareConsMem";
    #endif
    if (n<2)
        return static_cast<unsigned long>(n);

   //Until the matrix class is improved the multiplication requires some auxiliary matrixes to store values
   strcMatrix evenM;
   strcMatrix oddM;
   strcMatrix auxM;
   strcMatrix resM;

   bool useOdd=false;
   bool useEven=false;
   bool oddFirst=false;
   bool oddExp=false;
   
   *resM(0,1)=1;
   *resM(1,0)=1;
   *resM(1,1)=1;
    
   *evenM(0,1)=1;
   *evenM(1,0)=1;
   *evenM(1,1)=1;

   *oddM(0,1)=1;
   *oddM(1,0)=1;
   *oddM(1,1)=1;        
   
   while(n>1)
   {    
     if(n%2==0)
     {
	useEven=true;//There was some even squaring
       	*auxM(0,0)=*evenM(0,0);
	*auxM(0,1)=*evenM(0,1);
	*auxM(1,0)=*evenM(1,0);
	*auxM(1,1)=*evenM(1,1);
	evenM.multiply(auxM,auxM);
	n/=2;
     }
     else
     {
       if(!useEven)//if the first exponent was odd I need to multiply an odd number of Matrixes
	  oddFirst=true;
       else
       {
	 oddExp=true; //if a later exponent was odd, I need to squar the odd number of matrixes
	  *auxM(0,0)=*oddM(0,0);
	  *auxM(0,1)=*oddM(0,1);
	  *auxM(1,0)=*oddM(1,0);
	  *auxM(1,1)=*oddM(1,1);
	  oddM.multiply(auxM,auxM);
       }
       useOdd=true;
       n--;
     }
   }
   if(oddFirst&&oddExp)//I multiply the odd matrixes with the base value [[0,1][1,1]] only if there was squaring, otherwise the oddMatrix already has the right value
   {
     strcMatrix baseM;
     *baseM(0,1)=1;
     *baseM(1,0)=1;
     *baseM(1,1)=1;
     *auxM(0,0)=*oddM(0,0);
     *auxM(0,1)=*oddM(0,1);
     *auxM(1,0)=*oddM(1,0);
     *auxM(1,1)=*oddM(1,1);
     oddM.multiply(baseM,auxM);
   }
   if(useOdd&&useEven)
     resM.multiply(oddM,evenM);
   else if(useOdd)
   {
     *resM(0,0)=*oddM(0,0);
     *resM(0,1)=*oddM(0,1);
     *resM(1,0)=*oddM(1,0);
     *resM(1,1)=*oddM(1,1);
   }
   else if(useEven)
   {
     *resM(0,0)=*evenM(0,0);
     *resM(0,1)=*evenM(0,1);
     *resM(1,0)=*evenM(1,0);
     *resM(1,1)=*evenM(1,1);
   }
   unsigned long ret=*resM(0,1);
   return ret;  
}

#ifdef TEST_RUN
TEST (FibonacciTest,fibonacciExpByConstMemSquareFunction)
{
        ASSERT_EQ(fibonacciExpBySquareConsMem(0),0)<< "fibonacciExpBySquareConsMem returned "<<fibonacciExpBySquareConsMem(0)<< " instead of "<<0;
        ASSERT_EQ(fibonacciExpBySquareConsMem(1),1)<< "fibonacciExpBySquareConsMem returned "<<fibonacciExpBySquareConsMem(1)<< " instead of "<<1;
        ASSERT_EQ(fibonacciExpBySquareConsMem(2),1)<< "fibonacciExpBySquareConsMem returned "<<fibonacciExpBySquareConsMem(2)<< " instead of "<<1;
        ASSERT_EQ(fibonacciExpBySquareConsMem(3),2)<< "fibonacciExpBySquareConsMem returned "<<fibonacciExpBySquareConsMem(3)<< " instead of "<<2;
        ASSERT_EQ(fibonacciExpBySquareConsMem(4),3)<< "fibonacciExpBySquareConsMem returned "<<fibonacciExpBySquareConsMem(4)<< " instead of "<<3;
        ASSERT_EQ(fibonacciExpBySquareConsMem(5),5)<< "fibonacciExpBySquareConsMem returned "<<fibonacciExpBySquareConsMem(5)<< " instead of "<<5;
        ASSERT_EQ(fibonacciExpBySquareConsMem(6),8)<< "fibonacciExpBySquareConsMem returned "<<fibonacciExpBySquareConsMem(6)<< " instead of "<<8;
        ASSERT_EQ(fibonacciExpBySquareConsMem(7),13)<< "fibonacciExpBySquareConsMem returned "<<fibonacciExpBySquareConsMem(7)<< " instead of "<<13;
        ASSERT_EQ(fibonacciExpBySquareConsMem(8),21)<< "fibonacciExpBySquareConsMem returned "<<fibonacciExpBySquareConsMem(8)<< " instead of "<<21;
        ASSERT_EQ(fibonacciExpBySquareConsMem(9),34)<< "fibonacciExpBySquareConsMem returned "<<fibonacciExpBySquareConsMem(9)<< " instead of "<<34;
        ASSERT_EQ(fibonacciExpBySquareConsMem(10),55)<< "fibonacciExpBySquareConsMem returned "<<fibonacciExpBySquareConsMem(10)<< " instead of "<<55;
}
#endif



void initializeMeterFunctions()
{
    stopWatchFunc=clock;
    stopWatch=new cMeter<clock_t>(stopWatchFunc);
    stopWatch->setUnitName("time_us");
    stopWatch->setUnitSymbol("us");
    stopWatchDataDump=new cMeterDataDump<clock_t>(stopWatch);
    /*Losing too much precision during the convertion
    auto func=[] (clock_t val){return ((static_cast<float>(val))/CLOCKS_PER_SEC)*1000000;};
    stopWatchDataDump->setConversionFunction(func);*/
   
    /*startCPUMeterFunc=startRDTSC;
    if(isRDTSCPsupported())
      stopCPUMeterFunc=stopRDTSCP;
    else
      stopCPUMeterFunc=stopRDTSC;
    cpuMeter=new cMeter<uint64_t>(startCPUMeterFunc);
    cpuMeter->setSpecificStopFunction(stopCPUMeterFunc);*/
    startCPUMeterFunc=RDTSC;
    cpuMeter=new cMeter<uint64_t>(startCPUMeterFunc);
    cpuMeter->setUnitName("cpu_cycles");
    cpuMeter->setUnitSymbol("cycles");
    cpuCycleDataDump=new cMeterDataDump<uint64_t>(cpuMeter);
}

#ifdef TEST_RUN
TEST (FibonacciPerformanceTest,fibonacciFunctionsTime)
{
    unsigned long ret=0;
    int batchNumber=_MAX_FIB_NUMBER;
    int testPerBatch=10;
    
    int negativeOffsetFibRecursive=73;
    int negativeOffsetDerivedFormula=_MAX_FIB_NUMBER - _MAX_FIB_NUMBER_DERIVED_FUNCTION;
    stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,_EMPTY_STRING,"fibonacci_recursive"));
    for (int i=0; i<batchNumber - negativeOffsetFibRecursive; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            stopWatchDataDump->StartMeter();
            ret=fibonacciRet(i);
            stopWatchDataDump->StopMeter();
        }
        stopWatchDataDump->StoreBatch();
    }
    stopWatchDataDump->dumpData();

    stopWatchDataDump->resetData();
    stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,_EMPTY_STRING,"fibonacciArray"));
    for (int i=0; i<batchNumber; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            stopWatchDataDump->StartMeter();
            ret=fibonacciArray(i);
            stopWatchDataDump->StopMeter();
        }
        stopWatchDataDump->StoreBatch();
    }
    stopWatchDataDump->dumpData();

    stopWatchDataDump->resetData();
    stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,_EMPTY_STRING,"fibonacciConst"));
    for (int i=0; i<batchNumber; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            stopWatchDataDump->StartMeter();
            ret=fibonacciConst(i);
            stopWatchDataDump->StopMeter();
        }
        stopWatchDataDump->StoreBatch();
    }
    stopWatchDataDump->dumpData();

    stopWatchDataDump->resetData();
    stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,_EMPTY_STRING,"fibonacciExpBySquare"));
    for (int i=0; i<batchNumber; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            stopWatchDataDump->StartMeter();
            ret=fibonacciExpBySquare(i);
            stopWatchDataDump->StopMeter();
        }
        stopWatchDataDump->StoreBatch();
    }
    stopWatchDataDump->dumpData();

    stopWatchDataDump->resetData();
    stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,_EMPTY_STRING,"fibonacciDerivedFormula"));
    for (int i=0; i<batchNumber - negativeOffsetDerivedFormula; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            stopWatchDataDump->StartMeter();
            ret=fibonacciDerivedFormula(i);
            stopWatchDataDump->StopMeter();
        }
        stopWatchDataDump->StoreBatch();
    }
    stopWatchDataDump->dumpData();

    stopWatchDataDump->resetData();
    stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,_EMPTY_STRING,"fibonacciCompileTime"));
    for (int i=0; i<batchNumber; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            stopWatchDataDump->StartMeter();
            ret=getLookUpTableFibonacciValue(i);
            stopWatchDataDump->StopMeter();
        }
        stopWatchDataDump->StoreBatch();
    }
    stopWatchDataDump->dumpData();
    stopWatchDataDump->resetData();
    stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,_EMPTY_STRING,"fibonacciExBySquaringConstantMemory"));
    for (int i=0; i<batchNumber; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            stopWatchDataDump->StartMeter();
            ret=fibonacciExpBySquareConsMem(i);
            stopWatchDataDump->StopMeter();
        }
        stopWatchDataDump->StoreBatch();
    }
    stopWatchDataDump->dumpData();
    stopWatchDataDump->resetData();
}
#endif

#ifdef TEST_RUN
TEST (FibonacciPerformanceTest,fibonacciFunctionsCPUCycles)
{
    unsigned long ret=0;
    int batchNumber=_MAX_FIB_NUMBER;
    int testPerBatch=10;
    
    int negativeOffsetFibRecursive=73;
    int negativeOffsetDerivedFormula=_MAX_FIB_NUMBER - _MAX_FIB_NUMBER_DERIVED_FUNCTION;
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,_EMPTY_STRING,"fibonacci_recursive"));
    for (int i=0; i<batchNumber - negativeOffsetFibRecursive; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            cpuCycleDataDump->StartMeter();
            ret=fibonacciRet(i);
            cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();

    cpuCycleDataDump->resetData();
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,_EMPTY_STRING,"fibonacciArray"));
    for (int i=0; i<batchNumber; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            cpuCycleDataDump->StartMeter();
            ret=fibonacciArray(i);
            cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();

    cpuCycleDataDump->resetData();
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,_EMPTY_STRING,"fibonacciConst"));
    for (int i=0; i<batchNumber; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            cpuCycleDataDump->StartMeter();
            ret=fibonacciConst(i);
            cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();

    cpuCycleDataDump->resetData();
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,_EMPTY_STRING,"fibonacciExpBySquare"));
    for (int i=0; i<batchNumber; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            cpuCycleDataDump->StartMeter();
            ret=fibonacciExpBySquare(i);
            cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();

    cpuCycleDataDump->resetData();
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,_EMPTY_STRING,"fibonacciDerivedFormula"));
    for (int i=0; i<batchNumber - negativeOffsetDerivedFormula; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            cpuCycleDataDump->StartMeter();
            ret=fibonacciDerivedFormula(i);
            cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();

    cpuCycleDataDump->resetData();
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,_EMPTY_STRING,"fibonacciCompileTime"));
    for (int i=0; i<batchNumber; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            cpuCycleDataDump->StartMeter();
            ret=getLookUpTableFibonacciValue(i);
            cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,_EMPTY_STRING,"fibonacciExpBySquaringConstantMemory"));
    for (int i=0; i<batchNumber; i++)
    {
        for(int j=0;j<testPerBatch;j++)
        {
            cpuCycleDataDump->StartMeter();
            ret=fibonacciExpBySquareConsMem(i);
            cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
  
}
#endif

#ifdef TEST_RUN
std::vector<int> generateRandomVector(int seed, int size)
{
    std::mt19937 gen;
    gen.seed(seed);
    std::uniform_int_distribution<> dis(1, 500);
    std::vector<int> res;
    for(int i=0; i<size;i++)
      res.push_back(dis(gen));
    return res;
}

std::vector<int> generateRandomVector(int seed, int size, std::string distributionType)
{
  if(distributionType==std::string(_NORMAL_DIST))
    {
      std::mt19937 gen;
      gen.seed(seed);
      std::uniform_int_distribution<> dis(1, 500);
      std::vector<int> res;
      for(int i=0; i<size;i++)
	res.push_back(dis(gen));
      return res;
    }
    else if(distributionType==std::string(_SORTED_ARRAY))
    {
      std::vector<int> res;
      for( int i=0; i<size; i++)
	res.push_back(i);
      return res;
    }
    else if(distributionType==std::string(_REVERSE_SORTED_ARRAY))
    {
      std::vector<int> res;
      for( int i=size; i>0; i--)
	res.push_back(i);
      return res;
    }
    else if(distributionType== std::string(_SCRAMBLED_ARRAY))
    {
      std::mt19937 gen;
      gen.seed(seed);
      std::uniform_int_distribution<> dis(0, size);
      std::vector<int> res(size);
      for (int i=1; i<=size; i++)
      {
	int idx=dis(gen);
	while(res[idx]!=0)
	{
	  idx++;
	  if(idx>=size)
	    idx=0;
	}
	res[idx]=i;
      }
      return res;
    }
    else if(distributionType== std::string(_REPEATED_ELEMENTS_ARRAY))
    {
      std::mt19937 gen;
      gen.seed(seed);
      std::uniform_int_distribution<> dis(0, size);
      std::vector<int> res(size);
      for (int i=1; i<=size; i++)
      {
	  int cnt=dis(gen);
	  if(i+cnt>size)
	      cnt=size-i;
	  for (int j=0; j<cnt;j++)
	  {
	    int idx=dis(gen);
	    while(res[idx]!=0)
	    {
	      idx++;
	      if(idx>=size)
		idx=0;
	    }
	    res[idx]=i;
	  }
	  i+=cnt;
      }
      return res;
    }
    std::mt19937 gen;
    gen.seed(seed);
    std::uniform_int_distribution<> dis(1, 500);
    std::vector<int> res;
    for(int i=0; i<size;i++)
      res.push_back(dis(gen));
    return res;
}

std::string generateFileName(std::string MeasurementType, std::string attributes, std::string algorithm)
{
  std::string res=MeasurementType+"_"+attributes+"_"+algorithm;
  return res;
  
}
#endif

#ifdef TEST_RUN
TEST (SortingPerformanceTest,sortingFunctionsTime)
{
    int batchNumber=_MAX_VECTOR_SIZE_NUMBER;
    int testPerBatch=10;
    std::vector<std::string> arrayTypes;
    arrayTypes.push_back(std::string(_NORMAL_DIST));
    arrayTypes.push_back(std::string(_SORTED_ARRAY));
    arrayTypes.push_back(std::string(_REVERSE_SORTED_ARRAY));
    arrayTypes.push_back(std::string(_SCRAMBLED_ARRAY));
    arrayTypes.push_back(std::string(_REPEATED_ELEMENTS_ARRAY));
    for(int k=0; k<static_cast<int>(arrayTypes.size()); k++)
    {
      stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,arrayTypes[k],"InsertionSort"));
      for (int i=0; i<batchNumber; i++)
      {
	std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	  for(int j=0;j<testPerBatch;j++)
	  {
	    SortableArray<int> sa(toSort);
	    sa.setElementsAreComparable();
	      stopWatchDataDump->StartMeter();
	      sa.InsertionSort();
	      stopWatchDataDump->StopMeter();
	  }
	  stopWatchDataDump->StoreBatch();
      }
      stopWatchDataDump->dumpData();
      stopWatchDataDump->resetData();
      
      stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,arrayTypes[k],"quicksort"));
      for (int i=0; i<batchNumber; i++)
      {
	std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	  for(int j=0;j<testPerBatch;j++)
	  {
	    SortableArray<int> sa(toSort);
	    sa.setElementsAreComparable();
	      stopWatchDataDump->StartMeter();
	      sa.QuickSort();
	      stopWatchDataDump->StopMeter();
	  }
	  stopWatchDataDump->StoreBatch();
      }
      stopWatchDataDump->dumpData();
      stopWatchDataDump->resetData();
      
      stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,arrayTypes[k],"mergesort"));
      for (int i=0; i<batchNumber; i++)
      {
	std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	  for(int j=0;j<testPerBatch;j++)
	  {
	    SortableArray<int> sa(toSort);
	    sa.setElementsAreComparable();
	      stopWatchDataDump->StartMeter();
	      sa.Mergesort();
	      stopWatchDataDump->StopMeter();
	  }
	  stopWatchDataDump->StoreBatch();
      }
      stopWatchDataDump->dumpData();
      stopWatchDataDump->resetData();
    }
}
#endif

#ifdef TEST_RUN
TEST (SortingPerformanceTest,sortingFunctionsCPUCycles)
{
  int batchNumber=_MAX_VECTOR_SIZE_NUMBER;
  int testPerBatch=10;
  std::vector<std::string> arrayTypes;
  arrayTypes.push_back(std::string(_NORMAL_DIST));
  arrayTypes.push_back(std::string(_SORTED_ARRAY));
  arrayTypes.push_back(std::string(_REVERSE_SORTED_ARRAY));
  arrayTypes.push_back(std::string(_SCRAMBLED_ARRAY));
  arrayTypes.push_back(std::string(_REPEATED_ELEMENTS_ARRAY));
  for(int k=0; k<static_cast<int>(arrayTypes.size()); k++)
  {
   cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,arrayTypes[k],"InsertionSort"));
    for (int i=0; i<batchNumber; i++)
    {
      std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
        for(int j=0;j<testPerBatch;j++)
	{
	  SortableArray<int> sa(toSort);
	  sa.setElementsAreComparable();
            cpuCycleDataDump->StartMeter();
            sa.InsertionSort();
            cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
    
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,arrayTypes[k],"quicksort"));
    for (int i=0; i<batchNumber; i++)
    {
      std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
        for(int j=0;j<testPerBatch;j++)
	{
	  SortableArray<int> sa(toSort);
	  sa.setElementsAreComparable();
            cpuCycleDataDump->StartMeter();
            sa.QuickSort();
            cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
    
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,arrayTypes[k],"mergesort"));
    for (int i=0; i<batchNumber; i++)
    {
      std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
        for(int j=0;j<testPerBatch;j++)
	{
	  SortableArray<int> sa(toSort);
	  sa.setElementsAreComparable();
            cpuCycleDataDump->StartMeter();
            sa.Mergesort();
            cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
  }
}
#endif

///As for the moment I need to include the cpp, it became necessary to move the functional tests here
#ifdef TEST_RUN
TEST (SortableArrayTest,InsertionSortFunctionalTest)
{
  std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,0);
  SortableArray<int> sa(toSort);
  sa.setElementsAreComparable();
  sa.InsertionSort();
  std::vector<int> v(sa.GetVector());
  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,1);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.InsertionSort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,2);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.InsertionSort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,3);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.InsertionSort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,10);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.InsertionSort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
}
#endif // TEST_RUN


#ifdef TEST_RUN
TEST (SortableArrayTest,QuickSortFunctionalTest)
{
  std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,0);
  SortableArray<int> sa(toSort);
  sa.setElementsAreComparable();

  sa.QuickSort();
  std::vector<int> v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,1);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.QuickSort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,2);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.QuickSort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,3);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.QuickSort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,10);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.QuickSort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
}
#endif // TEST_RUN

#ifdef TEST_RUN
TEST (SortableArrayTest,MergeSortFunctionalTest)
{
  std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,0);
  SortableArray<int> sa(toSort);
  sa.setElementsAreComparable();
  sa.Mergesort();
  std::vector<int> v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,1);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.Mergesort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,2);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.Mergesort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,3);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.Mergesort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,10);
  sa=SortableArray<int>(toSort);
  sa.setElementsAreComparable();
  sa.Mergesort();
  v=sa.GetVector();

  for (int i=0;i<static_cast<int>(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);
  

}
#endif // TEST_RUN
