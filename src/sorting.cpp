#include "sorting.h"

template <typename T>
void swap(T& a, T& b)
{
  T aux=a;
  a=b;
  b=aux;
}

template <typename T>
void Insertionsort(std::vector<T> &array,size_t low,size_t high, std::function<bool(T,T)> m_fLThan)
{
  for(size_t i=low; i<high; i++)
  {
    T key=array[i];
    size_t j=i;
    while ((j > 0) && m_fLThan(key,array[j-1]))
    {
      array[j]=array[j-1];
      j=j-1;
    }
    array[j]=key;
  }
}

template <typename T>
std::vector<T> merge(std::vector<T> left, std::vector<T> right, std::function<bool(T,T)> m_fLThan)
{
  std::vector<T> res;
  size_t i=0;
  size_t j=0;
  for(size_t n=0;n<left.size()+right.size();n++)
  {
    if(i>=left.size())
      res.push_back(right[j++]);
    else if(j>=right.size())
      res.push_back(left[i++]);
    else if (m_fLThan(left[i],right[j]))
      res.push_back(left[i++]);
    else
      res.push_back(right[j++]);
  }
  return res;	
}

template<typename T>
size_t qsPartition(std::vector<T> &array,size_t i, size_t k, std::function<bool(T,T)> m_fLThan)
{
  T pivot=array[i];
  size_t left=i;
  
  for(size_t j=i+1;j<k;j++)
  {
    if(m_fLThan(array[j],pivot))
    {
      left++;
      swap(array[j],array[left]);
    }
  }
  swap(array[i],array[left]);
  return left;
}

template <typename T>
std::vector<T> Quicksort(std::vector<T> &array,size_t low,size_t high, std::function<bool(T,T)> m_fLThan)
{
  size_t pivotPos=0;
  if(low<high)
  {
    pivotPos= qsPartition(array,low,high,m_fLThan);
    Quicksort(array,low,pivotPos,m_fLThan);
    Quicksort(array,pivotPos+1,high,m_fLThan);
  }
  return array;
}

template<typename T>
std::pair<size_t,size_t> qsPartition_opt(std::vector<T> &array,size_t i, size_t k, std::function<bool(T,T)> m_fLThan)
{
  size_t pivotIdx=i+(k-i)/2;
  T pivot=array[pivotIdx];
  size_t left=i;
  size_t right=k-1;
  for(size_t j=i;j<=right;j++)
  {
    if(m_fLThan(array[j],pivot))
    {
      if(j>left)
      {
	swap(array[j],array[left++]);
	j--;
      }
      else
	left++;
      
    }
    else if(m_fLThan(pivot,array[j]))
    {
      swap(array[j],array[right]);
      right--;
      j--;
    }
  }
  return std::pair<int,int>(left,right);
}


template <typename T>
std::vector<T> Quicksort_opt(std::vector<T> &array,size_t low,size_t high, std::function<bool(T,T)> m_fLThan)
{
  size_t size=high-low;
  if(size < _SMALL_ARRAY_SIZE)
  {
    Insertionsort(array,low,high,m_fLThan);
    return array;
  }
  std::pair<size_t,size_t> pivotPos;
  if(low<high)
  {
    pivotPos= qsPartition_opt(array,low,high,m_fLThan);
    Quicksort_opt(array,low,std::get<0>(pivotPos),m_fLThan);
    Quicksort_opt(array,std::get<1>(pivotPos),high,m_fLThan);
  }
  return array;
}

template <typename T>
std::vector<T> Quicksort_opt(std::vector<T> array, std::function<bool(T,T)> m_fLThan)
{
  if(array.size()< _SMALL_ARRAY_SIZE)
    return Insertionsort(array,m_fLThan);
  return Quicksort_opt(array,0,array.size(),m_fLThan);
}

template <typename T>
std::vector<T> Mergesort(std::vector<T> array, std::function<bool(T,T)> m_fLThan)
{
  if(array.size()==0)
  {
    std::vector<T> res;
    return res;
  }
  if(array.size()==1)
    return array;
  size_t mid=array.size()/2;
    
  std::vector<T> leftVec(&array[0],&array[mid]);
  std::vector<T> rightVec(&array[mid],&array[array.size()]);

  leftVec=Mergesort(leftVec,m_fLThan);
  rightVec=Mergesort(rightVec,m_fLThan);
  return merge(leftVec,rightVec,m_fLThan);
}

template <typename T>
std::vector<T> Insertionsort(std::vector<T> array, std::function<bool(T,T)> m_fLThan)
{
  for(size_t i=0; i<array.size(); i++)
  {
    T key=array[i];
    size_t j=i;
    while ((j > 0) && m_fLThan(key,array[j-1]))
    {
      array[j]=array[j-1];
      j=j-1;
    }
    array[j]=key;
  }
  return array;
}

template <typename T>
std::vector<T> Quicksort(std::vector<T> array, std::function<bool(T,T)> m_fLThan)
{
  Quicksort(array,0,array.size(),m_fLThan);
  return array;
}
