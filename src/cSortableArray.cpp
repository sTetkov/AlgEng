
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
void SortableArray<T>::Internal_InsertionSort()
{
  m_vInternalVector=InsertionSort(m_vInternalVector);
}

template<typename T>
void SortableArray<T>::Internal_QuickSort()
{
  m_vInternalVector=Quicksort(m_vInternalVector,0,m_vInternalVector.size());
}

/*
 * brief: Mergesorting of the array
 *
 * Isolated the mergesort function, to stop someone to use it on a vector
 * of the wrong type.
 *
 */
template <typename T>
void SortableArray<T>::Internal_Mergesort()
{
  m_vInternalVector=MergeSort(m_vInternalVector);	
}
	
template <typename T>
std::vector<T> SortableArray<T>::merge(std::vector<T> left, std::vector<T> right)
{
  std::vector<T> res;
  int i=0;
  int j=0;
  for(int n=0;n<left.size()+right.size();n++)
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

template <typename T>
std::vector<T> SortableArray<T>::Quicksort(std::vector<T> array)
{
  Quicksort(array,0,array.size());
  return array;
}

template <typename T>
std::vector<T> SortableArray<T>::Quicksort(std::vector<T> &array,int low,int high)
{
  int pivotPos=0;
  if(low<high)
  {
    pivotPos= qsPartition(array,low,high);
    Quicksort(array,low,pivotPos);
    Quicksort(array,pivotPos+1,high);
  }
  return array;
}

template<typename T>
int SortableArray<T>::qsPartition(std::vector<T> &array,int i, int k)
{
  int pivot=array[i];
  int left=i;
  
  for(int j=i+1;j<k;j++)
  {
    if(array[j]<pivot)
    {
      left++;
      swap(array[j],array[left]);
    }
  }
  swap(array[i],array[left]);
  return left;
}

template <typename T>
std::vector<T> SortableArray<T>::Quicksort_opt(std::vector<T> array)
{
  if(array.size()< _SMALL_ARRAY_SIZE)
    return InsertionSort(array);
  return Quicksort_opt(array,0,array.size());
}

template <typename T>
std::vector<T> SortableArray<T>::Quicksort_opt(std::vector<T> &array,int low,int high)
{
  int size=high-low;
  if(size < _SMALL_ARRAY_SIZE)
  {
    InsertionSort(array,low,high);
    return array;
  }
  std::pair<int,int> pivotPos;
  if(low<high)
  {
    pivotPos= qsPartition_opt(array,low,high);
    Quicksort_opt(array,low,std::get<0>(pivotPos));
    Quicksort_opt(array,std::get<1>(pivotPos),high);
  }
  return array;
}

template<typename T>
std::pair<int,int> SortableArray<T>::qsPartition_opt(std::vector<T> &array,int i, int k)
{
  int pivotIdx=i+(k-i)/2;
  T pivot=array[pivotIdx];
  int left=i;
  int right=k-1;
  int aux;
  for(int j=i;j<=right;j++)
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
std::vector<T> SortableArray<T>::MergeSort(std::vector<T> array)
{
  if(static_cast<unsigned int>(array.size())==0)
  {
    std::vector<T> res;
    return res;
  }
  if(static_cast<unsigned int>(array.size())==1)
    return array;
  unsigned int mid=static_cast<unsigned int>(array.size()/2);
    
  std::vector<T> leftVec(&array[0],&array[mid]);
  std::vector<T> rightVec(&array[mid],&array[array.size()]);

  leftVec=MergeSort(leftVec);
  rightVec=MergeSort(rightVec);
  return merge(leftVec,rightVec);
}

template <typename T>
std::vector<T> SortableArray<T>::InsertionSort(std::vector<T> array)
{
  for(int i=0; i<array.size(); i++)
  {
    T key=array[i];
    int j=i;
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
void SortableArray<T>::swap(T& a, T& b)
{
  T aux=a;
  a=b;
  b=aux;
}
template <typename T>
void SortableArray<T>::InsertionSort(std::vector<T> &array,int low,int high)
{
  for(int i=low; i<high; i++)
  {
    T key=array[i];
    int j=i;
    while ((j > 0) && m_fLThan(key,array[j-1]))
    {
      array[j]=array[j-1];
      j=j-1;
    }
    array[j]=key;
  }
}