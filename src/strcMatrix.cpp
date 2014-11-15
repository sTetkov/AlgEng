#include "strcMatrix.h"

#ifdef TEST_RUN
#include "gtest/gtest.h"
#endif

/** @brief Easy access to the matrix values
  *
  * Operator (unsigned int row, unsigned int col) returns the value of
  * of the matrix at the coordinates [row][col] as a pointer to int.
  * If row or col are out of bound coordinates returns NULL
  */
unsigned long* strcMatrix::operator()(unsigned int row,unsigned int col)
{
    if((row>1) | (col>1))
        return NULL;
    return &_matrix[row][col];
}

/** @brief Multiplies this with an int
  *
  * Matrix multiplication, multiplies each element of this with integer val
  */
int strcMatrix::multiply(unsigned long val)
{
    for (unsigned int i=0; i<2; i++)
        for(unsigned int j=0; j<2; j++)
            _matrix[i][j]*=val;
    return 0;
}
/** @brief Initializes each element of the matrix to val
  *
  *
  */
void strcMatrix::initValues(unsigned long val)
{
    for (int i=0; i<2; i++)
        for(int j=0; j<2; j++)
            _matrix[i][j]=val;
}

/** @brief Multiplies two matrixes and stores the result in this
  *
  * Multiplies two matrixes m1, m2 and stores the result in this. Initializes this with 0, so
  * this.multiply(this, matrix) or this.multiply(matrix, this) will get incorrect results.
  */
int strcMatrix::multiply(strcMatrix m1,strcMatrix m2)
{
    initValues(0);
    for (unsigned int i=0; i<2; i++)
        for(unsigned int j=0; j<2; j++)
            for(unsigned int k=0; k<2; k++)
                _matrix[i][j]+=*m1(i,k) * *m2(k,j);
    return 0;
}
#ifdef TEST_RUN
TEST (MatrixTest,strcMatrix_multiply)
{
    strcMatrix m1;
    strcMatrix m2;
    strcMatrix m3;

    *m1(0,0)=0;
    *m1(0,1)=1;
    *m1(1,0)=1;
    *m1(1,1)=1;


    *m2(0,0)=0;
    *m2(0,1)=1;
    *m2(1,0)=1;
    *m2(1,1)=1;

    m3.multiply(m1,m2);
    ASSERT_EQ(*m3(0,0),1)<<"first multiplication got in (0,0) "<<*m3(0,0)<< " instead of "<< 1;
    ASSERT_EQ(*m3(0,1),1)<<"first multiplication got in (0,1) "<<*m3(0,1)<< " instead of "<< 1;
    ASSERT_EQ(*m3(1,0),1)<<"first multiplication got in (1,0) "<<*m3(1,0)<< " instead of "<< 1;
    ASSERT_EQ(*m3(1,1),2)<<"first multiplication got in (1,1) "<<*m3(1,1)<< " instead of "<< 2;

    m2.multiply(m1,m3);
    ASSERT_EQ(*m2(0,0),1)<<"second multiplication got in (0,0) "<<*m2(0,0)<< " instead of "<< 1;
    ASSERT_EQ(*m2(0,1),2)<<"second multiplication got in (0,1) "<<*m2(0,1)<< " instead of "<< 2;
    ASSERT_EQ(*m2(1,0),2)<<"second multiplication got in (1,0) "<<*m2(1,0)<< " instead of "<< 2;
    ASSERT_EQ(*m2(1,1),3)<<"second multiplication got in (1,1) "<<*m2(1,1)<< " instead of "<< 3;

    m3.multiply(m1,m2);
    ASSERT_EQ(*m3(0,0),2)<<"third multiplication got in (0,0) "<<*m3(0,0)<< " instead of "<< 2;
    ASSERT_EQ(*m3(0,1),3)<<"third multiplication got in (0,1) "<<*m3(0,1)<< " instead of "<< 3;
    ASSERT_EQ(*m3(1,0),3)<<"third multiplication got in (1,0) "<<*m3(1,0)<< " instead of "<< 3;
    ASSERT_EQ(*m3(1,1),5)<<"third multiplication got in (1,1) "<<*m3(1,1)<< " instead of "<< 4;
}
#endif

/** @brief Adds a matrix to this
  *
  * Adds matrix m to this.
  */
int strcMatrix::add(strcMatrix m)
{
    for (unsigned int i=0; i<2; i++)
        for(unsigned int j=0; j<2; j++)
            _matrix[i][j]+=*m(i,j);
    return 0;
}

#ifdef TEST_RUN
TEST (MatrixTest,strcMatrix_add)
{
    strcMatrix m1;
    strcMatrix m2;
    *m2(0,0)=1;
    *m2(0,1)=1;
    *m2(1,0)=1;
    *m2(1,1)=1;

    m1.add(m2);
    ASSERT_EQ(*m1(0,0),1);
    ASSERT_EQ(*m1(0,1),1);
    ASSERT_EQ(*m1(1,0),1);
    ASSERT_EQ(*m1(1,1),1);

    m1.add(m2);
    ASSERT_EQ(*m1(0,0),2);
    ASSERT_EQ(*m1(0,1),2);
    ASSERT_EQ(*m1(1,0),2);
    ASSERT_EQ(*m1(1,1),2);

    m1.add(m2);
    ASSERT_EQ(*m1(0,0),3);
    ASSERT_EQ(*m1(0,1),3);
    ASSERT_EQ(*m1(1,0),3);
    ASSERT_EQ(*m1(1,1),3);

    m1.add(m2);
    ASSERT_EQ(*m1(0,0),4);
    ASSERT_EQ(*m1(0,1),4);
    ASSERT_EQ(*m1(1,0),4);
    ASSERT_EQ(*m1(1,1),4);

    m1.add(m2);
    ASSERT_EQ(*m1(0,0),5);
    ASSERT_EQ(*m1(0,1),5);
    ASSERT_EQ(*m1(1,0),5);
    ASSERT_EQ(*m1(1,1),5);

    m1.add(m2);
    ASSERT_EQ(*m1(0,0),6);
    ASSERT_EQ(*m1(0,1),6);
    ASSERT_EQ(*m1(1,0),6);
    ASSERT_EQ(*m1(1,1),6);
}
#endif


/** @brief Destructor
  *
  * As strcMatrix is currently allocated inframes and not on the heap, implementing the destructor
  * results in " double free or corruption". Needs to differentiate destructor for strcMatrix
  * allocated on the heap or allocated on frames.
  */
 strcMatrix::~strcMatrix()
{
 //   for(unsigned int i=0; i<2; i++)
 //       delete[] _matrix[i];
 //   delete[] _matrix;
}

/** @brief Constructor
  *
  * Creates a 2x2 matrix and initializes it to 0
  */
 strcMatrix::strcMatrix()
{
    _matrix= new unsigned long*[2];
    for(unsigned int i=0; i<2; i++)
        _matrix[i]=new unsigned long[2];
	initValues(0);
}

/*
strcMatrix::strcMatrix(strcMatrix const& copy)
{
    _matrix= new unsigned long*[2];
    for(unsigned int i=0; i<2; i++)
        _matrix[i]=new unsigned long[2];
	for (unsigned int i=0; i<2; i++)
        for (unsigned int j=0; j<2; j++)
            _matrix[i][j]=*copy(i,j);
}*/

