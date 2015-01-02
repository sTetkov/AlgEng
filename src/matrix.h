#ifndef SIMPLE_MATRIX_H
#define SIMPLE_MATRIX_H

struct simpleMatrix{
  unsigned long _data[2][2];
  void init(unsigned long a,unsigned long b,unsigned long c,unsigned long d)
  {
    _data[0][0]=a;
    _data[0][1]=b;
    _data[1][0]=c;
    _data[1][1]=d;
  }
};

simpleMatrix Multiply(simpleMatrix,unsigned long);
simpleMatrix Multiply(simpleMatrix,simpleMatrix);
simpleMatrix Add(simpleMatrix,simpleMatrix);

#endif