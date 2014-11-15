/// \brief This is a implementation of three different algorithms
///        for the calculation of the Fibonacci number.
///
///This program implements three different algorithms for the calculation of the fibonacci number
/// f(n) where f(n) = f(n-1) + f(n-2) and f(0) = 0 and f(1) = 1.
///
///

#include "strcMatrix.h"
#include <cmath>

#ifdef TEST_RUN
#include "gtest/gtest.h"
#endif

///
///Prototypes
///
unsigned long fibonacciRet(int);
unsigned long fibonacciArray(int);
unsigned long fibonacciConst(int);
strcMatrix expBySquaring(int);
unsigned long fibonacciExpBySquare(int);
unsigned long fibonacciDerivedFormula(int);


int main(
         #ifdef TEST_RUN
         int argc, char **argv
         #endif
         )
{
    #ifdef TEST_RUN
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    #else
    return 0;
    #endif // TEST_RUN
}

/// \brief This implements the classic recursive algorithm for the calculation of the fibonacci number.
///
///This algorithm, while quite straightforward, is extremely inefficient, having a runtime of 2^n and a similar
///memory usage.
unsigned long fibonacciRet(int n)
{
    #ifdef TEST_RUN
    EXPECT_GE(n,0)<< "A negative value" << n << "was passed to fibonacciRet" ;
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
unsigned long fibonacciArray(int n)
{
    #ifdef TEST_RUN
    EXPECT_GE(n,0)<< "A negative value" << n << "was passed to fibonacciArray";
    #endif
    if(n<2)
        return n;
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
unsigned long fibonacciConst(int n)
{
    #ifdef TEST_RUN
    EXPECT_GE(n,0)<< "A negative value" << n << "was passed to fibonacciConst";
    #endif
    if (n<2)
        return n;
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
strcMatrix expBySquaring(int n)
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

unsigned long fibonacciExpBySquare(int n)
{
    #ifdef TEST_RUN
    EXPECT_GE(n,0)<< "A negative value" << n << "was passed to fibonacciExpBySquare";
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

unsigned long fibonacciDerivedFormula(int n)
{
    #ifdef TEST_RUN
    EXPECT_GE(n,0)<< "A negative value" << n << "was passed to fibonacciDerivedFormula";
    #endif
    if (n<2)
        return n;
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
