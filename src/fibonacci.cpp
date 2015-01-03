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
#include "sorting.h"
#include "sorting.cpp"
#include "matrix.h"

///Ugly but at the moment necessary to allow the compilation of templates
#include "cMeter.cpp"
#include "cMeterDataDump.cpp"
#include <chrono>
#include <algorithm>

#include <random>

#ifdef TEST_RUN
#include "gtest/gtest.h"
#endif

#define _MAX_FIB_NUMBER 93
#define _MAX_FIB_NUMBER_DERIVED_FUNCTION 76
#define _RND_NUMBER_GEN_SEED 42
#define _MAX_VECTOR_SIZE_NUMBER 8000
#define _MAX_VECTOR_STEP_INCREASE 100
#define _VECTOR_TEST_PER_BATCH 5
#define _CPU_CYCLES_MEASUREMENT "CPU_cycles"
#define _TIME_UNITS_MEASUREMENT "Time_nanosecs"
#define _NORMAL_DIST "normal_distribution"
#define _SORTED_ARRAY "sorted_array"
#define _REVERSE_SORTED_ARRAY "reverse_sorted_array"
#define _SCRAMBLED_ARRAY "scrambled_array"
#define _REPEATED_ELEMENTS_ARRAY "array_with_repeated_elements"
#define _EMPTY_STRING ""
///
///Prototypes
///
unsigned long fibonacciRet(unsigned long);
unsigned long fibonacciArray(unsigned long);
unsigned long fibonacciConst(unsigned long);
strcMatrix expBySquaring(unsigned long);
unsigned long fibonacciExpBySquare(unsigned long);
unsigned long fibonacciDerivedFormula(unsigned long);
unsigned long fibonacciExpBySquareConsMem(unsigned long);
unsigned long fibonacciExpBySquareConsMemSimplified(unsigned long);
void initializeMeterFunctions();

void generateFibonacciLookUpTable(unsigned long);
unsigned long getLookUpTableFibonacciValue(unsigned long);
unsigned long fibLookupTable[94];

#ifdef TEST_RUN
std::vector<int> generateRandomVector(int, int);
std::vector<int> generateRandomVector(int, int, std::string);
std::string generateFileName(std::string, std::string, std::string);
#endif

cMeter<uint64_t>* stopWatch;
std::function<uint64_t()> stopWatchFunc;
cMeterDataDump<uint64_t>* stopWatchDataDump;
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
unsigned long fibonacciRet(unsigned long n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value" << n << "was passed to fibonacciRet" ;
    #endif
    if(n<2)
        return n;
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
unsigned long fibonacciArray(unsigned long n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value" << n << "was passed to fibonacciArray";
    #endif
    if(n<2)
        return n;
    unsigned long *piVec=new unsigned long[n];
    piVec[0]=0;
    piVec[1]=1;
    for (unsigned int i=2;i<n;i++)
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
unsigned long fibonacciConst(unsigned long n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value" << n << "was passed to fibonacciConst";
    #endif
    if (n<2)
        return n;
    unsigned long fib1=0;
    unsigned long fib2=1;
    unsigned long ret=0;
    for (unsigned int i=2;i<n+1;i++)
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
strcMatrix expBySquaring(unsigned long n)
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

unsigned long fibonacciExpBySquare(unsigned long n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value" << n << "was passed to fibonacciExpBySquare";
    #endif
    if (n<2)
        return n;
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

unsigned long fibonacciDerivedFormula(unsigned long n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER_DERIVED_FUNCTION)<< "A excessive value"<< n << "was passed to fibonacciDerivedFormula";
    #endif
    if (n<2)
        return n;
    unsigned long res=floor( (1/sqrt(5))*(pow(((1+sqrt(5))/2),n))+0.5f );
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
constexpr unsigned long fibonacciCompileTime (unsigned long n)
{

    return n > 1 ? fibonacciConst(n) : n;
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

void generateFibonacciLookUpTable(unsigned long n)
{
  for (unsigned int i=0;i<(n+1);i++)
    fibLookupTable[i]=fibonacciConst(i);
}

unsigned long getLookUpTableFibonacciValue(unsigned long n)
{
  #ifdef TEST_RUN
  EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value"<< n << "was passed to getLookUpTableFibonacciValue";
  #endif
  return fibLookupTable[n];
}

/// \brief: fibonacci number Exponentiation By Squaring using constant memory
///
unsigned long fibonacciExpBySquareConsMem(unsigned long n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value" << n << "was passed to fibonacciExpBySquareConsMem";
    #endif
    if (n<2)
        return n;

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

/// \brief: fibonacci number Exponentiation By Squaring using constant memory
///
unsigned long fibonacciExpBySquareConsMemSimplified(unsigned long n)
{
    #ifdef TEST_RUN
    EXPECT_LE(n,_MAX_FIB_NUMBER)<< "A excessive value" << n << "was passed to fibonacciExpBySquareConsMem";
    #endif
    if (n<2)
        return n;

   bool useOdd=false;
   bool useEven=false;
   bool oddFirst=false;
   bool oddExp=false;
   
   simpleMatrix res;
   res.init(0,1,1,1);
   
   simpleMatrix even;
   even.init(0,1,1,1);
   
   simpleMatrix odd;
   odd.init(0,1,1,1);
   
   while(n>1)
   {    
     if(n%2==0)
     {
	useEven=true;//There was some even squaring
       	even=Multiply(even,even);
	n/=2;
     }
     else
     {
       if(!useEven)//if the first exponent was odd I need to multiply an odd number of Matrixes
	  oddFirst=true;
       else
       {
	 oddExp=true; //if a later exponent was odd, I need to squar the odd number of matrixes
	 odd=Multiply(odd,odd);
       }
       useOdd=true;
       n--;
     }
   }
   
   if(oddFirst&&oddExp)//I multiply the odd matrixes with the base value [[0,1][1,1]] only if there was squaring, otherwise the oddMatrix already has the right value
   {
     simpleMatrix baseM;
     baseM.init(0,1,1,1);
     
    odd=Multiply(baseM,odd);
   }
  
   if(useOdd&&useEven)
     res=Multiply(odd,even);
   else if(useOdd)
   {
     res=odd;
   }
   else if(useEven)
   {
     res=even;
   }
   
   return res._data[0][1];  
}

#ifdef TEST_RUN
TEST (FibonacciTest,fibonacciExpByConstMemSquareSimplifiedFunction)
{
        ASSERT_EQ(fibonacciExpBySquareConsMemSimplified(0),0)<< "fibonacciExpBySquareConsMemSimplified returned "<<fibonacciExpBySquareConsMemSimplified(0)<< " instead of "<<0;
        ASSERT_EQ(fibonacciExpBySquareConsMemSimplified(1),1)<< "fibonacciExpBySquareConsMemSimplified returned "<<fibonacciExpBySquareConsMemSimplified(1)<< " instead of "<<1;
        ASSERT_EQ(fibonacciExpBySquareConsMemSimplified(2),1)<< "fibonacciExpBySquareConsMemSimplified returned "<<fibonacciExpBySquareConsMemSimplified(2)<< " instead of "<<1;
        ASSERT_EQ(fibonacciExpBySquareConsMemSimplified(3),2)<< "fibonacciExpBySquareConsMemSimplified returned "<<fibonacciExpBySquareConsMemSimplified(3)<< " instead of "<<2;
        ASSERT_EQ(fibonacciExpBySquareConsMemSimplified(4),3)<< "fibonacciExpBySquareConsMemSimplified returned "<<fibonacciExpBySquareConsMemSimplified(4)<< " instead of "<<3;
        ASSERT_EQ(fibonacciExpBySquareConsMemSimplified(5),5)<< "fibonacciExpBySquareConsMemSimplified returned "<<fibonacciExpBySquareConsMemSimplified(5)<< " instead of "<<5;
        ASSERT_EQ(fibonacciExpBySquareConsMemSimplified(6),8)<< "fibonacciExpBySquareConsMemSimplified returned "<<fibonacciExpBySquareConsMemSimplified(6)<< " instead of "<<8;
        ASSERT_EQ(fibonacciExpBySquareConsMemSimplified(7),13)<< "fibonacciExpBySquareConsMemSimplified returned "<<fibonacciExpBySquareConsMemSimplified(7)<< " instead of "<<13;
        ASSERT_EQ(fibonacciExpBySquareConsMemSimplified(8),21)<< "fibonacciExpBySquareConsMemSimplified returned "<<fibonacciExpBySquareConsMemSimplified(8)<< " instead of "<<21;
        ASSERT_EQ(fibonacciExpBySquareConsMemSimplified(9),34)<< "fibonacciExpBySquareConsMemSimplified returned "<<fibonacciExpBySquareConsMemSimplified(9)<< " instead of "<<34;
        ASSERT_EQ(fibonacciExpBySquareConsMemSimplified(10),55)<< "fibonacciExpBySquareConsMemSimplified returned "<<fibonacciExpBySquareConsMemSimplified(10)<< " instead of "<<55;
}
#endif


void initializeMeterFunctions()
{
  
    stopWatchFunc =[] (){
      auto now = std::chrono::high_resolution_clock::now();
      uint64_t nanos = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count());
      return nanos;
      
    };
    stopWatch=new cMeter<uint64_t>(stopWatchFunc);
    stopWatch->setUnitName("time_us");
    stopWatch->setUnitSymbol("ns");
    stopWatchDataDump=new cMeterDataDump<uint64_t>(stopWatch);
    
    startCPUMeterFunc=startRDTSC;
    if(isRDTSCPsupported())
      stopCPUMeterFunc=stopRDTSCP;
    else
      stopCPUMeterFunc=stopRDTSC;
    cpuMeter=new cMeter<uint64_t>(startCPUMeterFunc);
    cpuMeter->setSpecificStopFunction(stopCPUMeterFunc);
    /*startCPUMeterFunc=RDTSC;
    cpuMeter=new cMeter<uint64_t>(startCPUMeterFunc);*/
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
      int genRoof= (size<100) ? size/10 : 10;
      std::uniform_int_distribution<> dis(0, genRoof);
      std::vector<int> res(size);
      for(int i=0; i<size;i++)
	res.push_back(dis(gen));
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

///As for the moment I need to include the cpp, it became necessary to move the functional tests here
#ifdef TEST_RUN

void SortedVectorAssert(std::vector<int> v)
{
  if(v.size()==0)
    return;
  for (size_t i=0;i<(v.size()-1);i++)
    ASSERT_LE(v[i],v[i+1]);  
}

TEST (SortableArrayTest,InsertionSortFunctionalTest)
{
  std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,0);
  std::vector<int> v;

  v=Insertionsort(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,1);
  v=Insertionsort(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,2);
  v=Insertionsort(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,3);
  v=Insertionsort(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,10);
  v=Insertionsort(toSort);

  SortedVectorAssert(v);
}

TEST (SortableArrayTest,QuickSortFunctionalTest)
{
  std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,0);
  std::vector<int> v;
  
  v=Quicksort(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,1);
  v=Quicksort(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,2);
  v=Quicksort(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,3);
  v=Quicksort(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,10);
  v=Quicksort(toSort);

  SortedVectorAssert(v);
  
}

TEST (SortableArrayTest,QuickSortOptFunctionalTest)
{
  std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,0);
  std::vector<int> v;

  v=Quicksort_opt(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,1);
  v=Quicksort_opt(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,2);
  v=Quicksort_opt(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,3);
  v=Quicksort_opt(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,10);
  v=Quicksort_opt(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,100);
  v=Quicksort_opt(toSort);
  
  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,400);
  v=Quicksort_opt(toSort);
  
  SortedVectorAssert(v);
  
}

TEST (SortableArrayTest,MergeSortFunctionalTest)
{
  std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,0);
  std::vector<int> v;

  v=Mergesort(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,1);
  v=Mergesort(toSort);
  
  SortedVectorAssert(v);

  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,2);
  v=Mergesort(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,3);
  v=Mergesort(toSort);
  
  SortedVectorAssert(v);

  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,10);
  v=Mergesort(toSort);

  SortedVectorAssert(v);
}

TEST (SortableArrayTest,MergeSortCompactMemoryFunctionalTest)
{
  std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,0);
  std::vector<int> v;

  v=Mergesort_cMem(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,1);
  v=Mergesort_cMem(toSort);
  
  SortedVectorAssert(v);

  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,2);
  v=Mergesort_cMem(toSort);

  SortedVectorAssert(v);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,3);
  v=Mergesort_cMem(toSort);
  
  SortedVectorAssert(v);

  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,10);
  v=Mergesort_cMem(toSort);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,100);
  v=Mergesort_cMem(toSort);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,1000);
  v=Mergesort_cMem(toSort);

  SortedVectorAssert(v);
}

TEST (SortableArrayTest,HeapSortFunctionalTest)
{
  std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,0);
  std::vector<int> v;

  v=Heapsort(toSort);

  ASSERT_EQ(isHeap(v),true);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,1);
  v=Heapsort(toSort);
  
  ASSERT_EQ(isHeap(v),true);

  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,2);
  v=Heapsort(toSort);

  ASSERT_EQ(isHeap(v),true);
  
  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,3);
  v=Heapsort(toSort);
  
  ASSERT_EQ(isHeap(v),true);

  toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,10);
  v=Heapsort(toSort);

  ASSERT_EQ(isHeap(v),true);
}
#endif // TEST_RUN


#ifdef TEST_RUN
int compare_ints(const void* a, const void* b)   // comparison function
{
    int arg1 = *reinterpret_cast<const int*>(a);
    int arg2 = *reinterpret_cast<const int*>(b);
    if(arg1 < arg2) return -1;
    if(arg1 > arg2) return 1;
    return 0;
}

TEST (SortingPerformanceTest,sortingFunctionsTime)
{
    int batchNumber=_MAX_VECTOR_SIZE_NUMBER;
    int testPerBatch=_VECTOR_TEST_PER_BATCH;
    std::vector<std::string> arrayTypes;
    arrayTypes.push_back(std::string(_NORMAL_DIST));
    arrayTypes.push_back(std::string(_SORTED_ARRAY));
    arrayTypes.push_back(std::string(_REVERSE_SORTED_ARRAY));
    arrayTypes.push_back(std::string(_SCRAMBLED_ARRAY));
    arrayTypes.push_back(std::string(_REPEATED_ELEMENTS_ARRAY));
    
    std::vector<int> res;
     
    for(size_t k=0; k<arrayTypes.size(); k++)
    {
      /*
      stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,arrayTypes[k],"InsertionSort"));
      for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
      {
	std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	
	  for(int j=0;j<testPerBatch;j++)
	  {
	      stopWatchDataDump->StartMeter();
	      res=Insertionsort(toSort);
	      stopWatchDataDump->StopMeter();
	  }
	  stopWatchDataDump->StoreBatch();
      }
      stopWatchDataDump->dumpData();
      stopWatchDataDump->resetData();
      
      stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,arrayTypes[k],"quicksort"));
      for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
      {
	std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	  for(int j=0;j<testPerBatch;j++)
	  {
	      stopWatchDataDump->StartMeter();
	      res=Quicksort(toSort);
	      stopWatchDataDump->StopMeter();
	  }
	  stopWatchDataDump->StoreBatch();
      }
      stopWatchDataDump->dumpData();
      stopWatchDataDump->resetData();
      */
      stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,arrayTypes[k],"quicksort_opt"));
      for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
      {
	std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	  for(int j=0;j<testPerBatch;j++)
	  {
	      stopWatchDataDump->StartMeter();
	      res=Quicksort_opt(toSort);
	      stopWatchDataDump->StopMeter();
	  }
	  stopWatchDataDump->StoreBatch();
      }
      stopWatchDataDump->dumpData();
      stopWatchDataDump->resetData();
      
      stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,arrayTypes[k],"mergesort"));
      for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
      {
	std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	  for(int j=0;j<testPerBatch;j++)
	  {
	      stopWatchDataDump->StartMeter();
	      res=Mergesort(toSort);
	      stopWatchDataDump->StopMeter();
	  }
	  stopWatchDataDump->StoreBatch();
      }
      stopWatchDataDump->dumpData();
      stopWatchDataDump->resetData();
      
            stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,arrayTypes[k],"mergesort_compactMem"));
      for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
      {
	std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	  for(int j=0;j<testPerBatch;j++)
	  {
	      stopWatchDataDump->StartMeter();
	      res=Mergesort_cMem(toSort);
	      stopWatchDataDump->StopMeter();
	  }
	  stopWatchDataDump->StoreBatch();
      }
      stopWatchDataDump->dumpData();
      stopWatchDataDump->resetData();
      
      stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,arrayTypes[k],"qsort"));
      for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
      {
	std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	  for(int j=0;j<testPerBatch;j++)
	  {
	      stopWatchDataDump->StartMeter();
	      std::qsort(toSort.data(),toSort.size(),sizeof(int),compare_ints);
	      stopWatchDataDump->StopMeter();
	  }
	  stopWatchDataDump->StoreBatch();
      }
      stopWatchDataDump->dumpData();
      stopWatchDataDump->resetData();
      
      stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,arrayTypes[k],"sort"));
      for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
      {
	std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	  for(int j=0;j<testPerBatch;j++)
	  {
	      stopWatchDataDump->StartMeter();
	      std::sort(toSort.begin(),toSort.end());
	      stopWatchDataDump->StopMeter();
	  }
	  stopWatchDataDump->StoreBatch();
      }
      stopWatchDataDump->dumpData();
      stopWatchDataDump->resetData();
      /*
      stopWatchDataDump->setFilename(generateFileName(_TIME_UNITS_MEASUREMENT,arrayTypes[k],"Heapsort"));
      for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
      {
	std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	  for(int j=0;j<testPerBatch;j++)
	  {
	      stopWatchDataDump->StartMeter();
	      res=Heapsort(toSort);
	      stopWatchDataDump->StopMeter();
	  }
	  stopWatchDataDump->StoreBatch();
      }
      stopWatchDataDump->dumpData();
      stopWatchDataDump->resetData();*/
      
    }
}
#endif

#ifdef TEST_RUN
TEST (SortingPerformanceTest,sortingFunctionsCPUCycles)
{
  int batchNumber=_MAX_VECTOR_SIZE_NUMBER;
  int testPerBatch=_VECTOR_TEST_PER_BATCH;
  std::vector<std::string> arrayTypes;
  arrayTypes.push_back(std::string(_NORMAL_DIST));
  arrayTypes.push_back(std::string(_SORTED_ARRAY));
  arrayTypes.push_back(std::string(_REVERSE_SORTED_ARRAY));
  arrayTypes.push_back(std::string(_SCRAMBLED_ARRAY));
  arrayTypes.push_back(std::string(_REPEATED_ELEMENTS_ARRAY));

  std::vector<int> res;
  
  for(size_t k=0; k<arrayTypes.size(); k++)
  {
    /*
   cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,arrayTypes[k],"InsertionSort"));
    for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
    {
      std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
        for(int j=0;j<testPerBatch;j++)
	{
	      cpuCycleDataDump->StartMeter();
	      res=Insertionsort(toSort);
	      cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
    
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,arrayTypes[k],"quicksort"));
    for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
    {
      std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
        for(int j=0;j<testPerBatch;j++)
	{
	      cpuCycleDataDump->StartMeter();
	      res=Quicksort(toSort);
	      cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
    */
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,arrayTypes[k],"quicksort_opt"));
    for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
    {
      std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
        for(int j=0;j<testPerBatch;j++)
	{
	      cpuCycleDataDump->StartMeter();
	      res=Quicksort_opt(toSort);
	      cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
    
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,arrayTypes[k],"mergesort"));
    for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
    {
      std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
        for(int j=0;j<testPerBatch;j++)
	{
	      cpuCycleDataDump->StartMeter();
	      res=Mergesort(toSort);
	      cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
    
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,arrayTypes[k],"mergesort_compactMem"));
    for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
    {
      std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
        for(int j=0;j<testPerBatch;j++)
	{
	      cpuCycleDataDump->StartMeter();
	      res=Mergesort_cMem(toSort);
	      cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
    
    
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,arrayTypes[k],"qsort"));
    
    
    for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
    {
        for(int j=0;j<testPerBatch;j++)
	{
	    std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	    cpuCycleDataDump->StartMeter();
	    std::qsort(toSort.data(),toSort.size(),sizeof(int),compare_ints);
	    cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
    
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,arrayTypes[k],"sort"));
        
    for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
    {
        for(int j=0;j<testPerBatch;j++)
	{
	    std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	    cpuCycleDataDump->StartMeter();
	    std::sort(toSort.begin(),toSort.end());
	    cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();
    /*
    cpuCycleDataDump->setFilename(generateFileName(_CPU_CYCLES_MEASUREMENT,arrayTypes[k],"Heapsort"));
        
    for (int i=0; i<batchNumber; i+=_MAX_VECTOR_STEP_INCREASE)
    {
        for(int j=0;j<testPerBatch;j++)
	{
	    std::vector<int> toSort=generateRandomVector(_RND_NUMBER_GEN_SEED,i,arrayTypes[k]);
	    cpuCycleDataDump->StartMeter();
	    res=Heapsort(toSort);
	    cpuCycleDataDump->StopMeter();
        }
        cpuCycleDataDump->StoreBatch();
    }
    cpuCycleDataDump->dumpData();
    cpuCycleDataDump->resetData();*/
  }
}
#endif