#ifndef MATRIX_H
#define MATRIX_H

#ifdef TEST_RUN
#include "gtest/gtest.h"
#endif

#include <cstddef>

struct strcMatrix
{
    public:

        strcMatrix();
        //strcMatrix(strcMatrix const&);
        virtual ~strcMatrix();

        static strcMatrix* getMultiplyIdentityMatrix();

        int add(strcMatrix);
        int multiply(strcMatrix,strcMatrix);
        int multiply(unsigned long);

        void initValues(unsigned long val);

        unsigned long* operator()(unsigned int,unsigned int);

    private:
        unsigned long** _matrix;
};
#endif // MATRIX_H
