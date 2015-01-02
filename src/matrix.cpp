#include "matrix.h"

simpleMatrix Multiply(simpleMatrix a,unsigned long b)
{
  simpleMatrix res;
  res._data[0][0]=a._data[0][0]+b;
  res._data[0][1]=a._data[0][1]+b;
  res._data[1][0]=a._data[1][0]+b;
  res._data[1][1]=a._data[1][1]+b;
  return res;
}

simpleMatrix Multiply(simpleMatrix a,simpleMatrix b)
{
  simpleMatrix res;
  res._data[0][0]=a._data[0][0]*b._data[0][0]+a._data[0][1]*b._data[1][0];
  res._data[0][1]=a._data[0][0]*b._data[1][0]+a._data[0][1]*b._data[1][1];
  res._data[1][0]=a._data[1][0]*b._data[0][0]+a._data[1][1]*b._data[1][0];
  res._data[1][1]=a._data[1][0]*b._data[1][0]+a._data[1][1]*b._data[1][1];
  return res;
}

simpleMatrix Add(simpleMatrix a,simpleMatrix b)
{
  simpleMatrix res;
  res._data[0][0]=a._data[0][0]+b._data[0][0];
  res._data[0][1]=a._data[0][1]+b._data[0][1];
  res._data[1][0]=a._data[1][0]+b._data[1][0];
  res._data[1][1]=a._data[1][1]+b._data[1][1];
  return res;
}
