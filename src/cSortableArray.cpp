
#include "cSortableArray.h"

#ifdef TEST_RUN
#include "gtest/gtest.h"
#endif

template<typename T>
SortableArray<T>::SortableArray()
{
  m_bLTFunAvailable=false;
  m_bLETFunAvailable=false;
  m_bETFunAvailable=false;
}

template<typename T>
SortableArray<T>::SortableArray(std::vector<T> v)
{
  m_vInternalVector=v;
}

template<typename T>
void SortableArray<T>::setElementsAreComparable()
{
  m_bLTFunAvailable=true;
  m_bLETFunAvailable=true;
  m_bETFunAvailable=true;
  m_fLThan=[] (T a, T b){return a<b;};
  m_fLEThan=[] (T a, T b){return a<=b;};
  m_fCompare=[] (T a, T b){return a==b;};
}
  
template<typename T>
void SortableArray<T>::AddElement(T elem)
{
  m_vInternalVector.push_back(elem);
}

template<typename T>
void SortableArray<T>::AddElement(std::vector<T> elem)
{
  m_vInternalVector.insert(m_vInternalVector.end,elem);
}
  
template<typename T>
void SortableArray<T>::AddLessThanFunction(std::function<bool(T,T)> func) 
{
  m_fLThan=func;
  m_bLTFunAvailable=true;
}

template<typename T>
void SortableArray<T>::AddLessOrEqualThanFunction(std::function<bool(T,T)> func)
{
  m_fLEThan=func;
  m_bLETFunAvailable=true;
}

template<typename T>
void SortableArray<T>::AddEqualFunction(std::function<bool(T,T)> func)
{
  m_fCompare=func;
  m_bETFunAvailable=true;
}
  
template<typename T>
std::vector<T> SortableArray<T>::GetVector()
{
  return m_vInternalVector;
}
  
template<typename T>
T* SortableArray<T>::GetArray()
{
  T* ret= new T[m_vInternalVector.size()];
  for (int i=0; i<m_vInternalVector.size(); i++)
    ret[i]=m_vInternalVector[i];
  return ret;
}
   
template<typename T>
void SortableArray<T>::InsertionSort()
{
  for(int i=0; i<m_vInternalVector.size(); i++)
  {
    int key=m_vInternalVector[i];
    int j=i;
    while ((j > 0) && (m_vInternalVector[j-1] > key))
    {
      m_vInternalVector[j]=m_vInternalVector[j-1];
      j=j-1;
    }
    m_vInternalVector[j]=key;
  }
}

template<typename T>
void SortableArray<T>::QuickSort()
{
  quicksort(0,m_vInternalVector.size());
}

template<typename T>
void SortableArray<T>::quicksort(int i, int k)
{
  int pivotPos=0;
  if(i<k)
  {
    pivotPos= qsPartition(i,k);
    quicksort(i,pivotPos);
    quicksort(pivotPos+1,k);
  }
}

template<typename T>
int SortableArray<T>::qsPartition(int i, int k)
{
  int pivot=m_vInternalVector[i];
  int left=i;
  
  for(int j=i+1;j<k;j++)
  {
    if(m_vInternalVector[j]<pivot)
    {
      left++;
      int aux =m_vInternalVector[j];
      m_vInternalVector[j]=m_vInternalVector[left];
      m_vInternalVector[left]=aux;
    }
  }
  int aux =m_vInternalVector[i];
  m_vInternalVector[i]=m_vInternalVector[left];
  m_vInternalVector[left]=aux;
  return left;
}