#ifndef _SORTING_ALGORITHM_IMPL
#define _SORTING_ALGORITHM_IMPL

#include <vector>
#include <functional>
#include <utility>
#include <assert.h>.
#include <random>
#include <cstdlib>

//this may be dependent on the single machine, but 80 seems to be a functional aproximation
static const size_t _SMALL_ARRAY_SIZE = 80;


template <typename T>
void swap(T& a, T& b)
{
  T aux=a; 
  a=b;
  b=aux;
}

/// \brief Insertion sort. m_fLThan is the function for < 
template <typename T>
std::vector<T> Insertionsort(std::vector<T> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
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

/// \brief It's insertion sort on the vector for index [low, high).
template <typename T>
void Insertionsort(std::vector<T> &array,size_t low,size_t high, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
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

/// \brief: Optimized partition function for quicksort. Uses random pivot and fat partitioning.
template<typename T>
std::pair<size_t,size_t> qsPartition_opt(std::vector<T> &array,size_t i, size_t k,std::mt19937 gen ,std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  std::uniform_int_distribution<size_t> dis(i, k-1);
  size_t pivotIdx=dis(gen);
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

/// \brief: Optimized Quicksort. 
///
///  Requires a high optimization level from the compiler to implement a tail recursion on the first recursive
///  call. This SOrts the vecotr segment defined by [low,high). Runs on an average case at O(nlogn) and requires O(1) Memory as it
///  works with constant memory usage.
template <typename T>
void Quicksort_opt(std::vector<T> &array, size_t low,size_t high, std::mt19937 gen, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  size_t size=high-low;
  if(size < _SMALL_ARRAY_SIZE)
  {
    Insertionsort(array,low,high,m_fLThan);
    return;
  }
  std::pair<size_t,size_t> pivotPos;
  if(low<high)
  {
    pivotPos= qsPartition_opt(array,low,high,gen,m_fLThan);
    Quicksort_opt(array,low,std::get<0>(pivotPos),gen,m_fLThan);
    Quicksort_opt(array,std::get<1>(pivotPos),high,gen,m_fLThan);
  }
}


/// \brief:Optimized Quicksort, this is the function to call on a whole vector.
template <typename T>
std::vector<T> Quicksort_opt(std::vector<T> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  if(array.size()< _SMALL_ARRAY_SIZE)
    return Insertionsort(array,m_fLThan);
  std::mt19937 gen;
  gen.seed(42);
  Quicksort_opt(array,0,array.size(),gen,m_fLThan);
  return array;
}

/// \brief: Merge function for merge sort.
template <typename T>
std::vector<T> merge(std::vector<T> left, std::vector<T> right, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
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

/// \brief: Mergesort. 
///
///  Plain mergesort, with recursion. Runs in O(nlogn) and requires O(nlogn) memory.
template <typename T>
std::vector<T> Mergesort(std::vector<T> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  if(array.size()<=1)
    return array;
  size_t mid=array.size()/2;
    
  std::vector<T> leftVec(&array[0],&array[mid]);
  std::vector<T> rightVec(&array[mid],&array[array.size()]);

  leftVec=Mergesort(leftVec,m_fLThan);
  rightVec=Mergesort(rightVec,m_fLThan);
  return merge(leftVec,rightVec,m_fLThan);
}

/// \brief: Partitioning function for Quicksort, uses the firste element as pivot
template<typename T>
size_t qsPartition(std::vector<T> &array,size_t i, size_t k, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
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

/// \brief: Plain quicksort that sorts the index space[i,k) of an array. Works on average in O(nlogn) and on worst case input (sorted arrays and arrays with a lot of repeated elements) in O(n^2).
/// Works with O(1) memory.
template <typename T>
std::vector<T> Quicksort(std::vector<T> &array,size_t low,size_t high, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
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

/// \brief: Quicksort that runs on a whole array.
template <typename T>
std::vector<T> Quicksort(std::vector<T> &array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  return Quicksort(array,0,array.size(),m_fLThan);
}

/// \brief: Gets parent Index for Heapsort
size_t pIndex(size_t i)
{
  if(i==0) return 0;
  return (i-1)/2;
}

/// \brief: Gets left child index for Heapsort
size_t lIndex(size_t i)
{
  return i*2+1;
}

/// \brief: gets right child index for Heap sort
size_t rIndex(size_t i)
{
  return i*2+2;
}

/// \brief: checks if an array is a sorted heap.
template <typename T>
bool isHeap(std::vector<T> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  if (array.size()==0) return true;
  for (size_t i=(array.size()-1);  i>0; i--)
    if( m_fLThan(array[i],array[pIndex(i)]) ) return false;
  return true;
}

/// \brief: heapifies an array.
template <typename T>
bool isHeap(std::vector<T> array,size_t end, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  if (array.size()<2) return true;
  for (size_t i=(end-1);  i>0; i--)
    if( m_fLThan(array[i],array[pIndex(i)]) ) return false;
  return true;
}

/// \brief: After a new element is added, this function sorts it in the right place.
template<typename T>
void siftUp(std::vector<T> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  assert(array.size()>0);
  assert(isHeap(array,array.size()-1,m_fLThan) );
  
  size_t newElementIdx=array.size()-1;
  while (true)
  {
    if(newElementIdx==0) break;
    size_t parentIdx= pIndex(newElementIdx);
    if(array[parentIdx]<=array[newElementIdx]) break;
    swap(array[parentIdx],array[newElementIdx]);
    newElementIdx=parentIdx;
  }
  
  assert(isHeap(array,m_fLThan));

}

/// \brief: when called on an array it will cause a rebalancing. 
template<typename T>
void siftDown(std::pair<T*,size_t> array,size_t start, size_t end, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  size_t pIdx=start;
  size_t swapIdx;
  while (lIndex(pIdx) < end) 
  {
    swapIdx = lIndex(pIdx);
    if(rIndex(pIdx)<end)
      if (m_fLThan(array.first[swapIdx],array.first[rIndex(pIdx)]))
	swapIdx=rIndex(pIdx);
    if (m_fLThan(array.first[pIdx],array.first[swapIdx]))
    {
      swap(array.first[pIdx],array.first[swapIdx]);
      pIdx = swapIdx;
    } 
    else 
      return;
  }
}

/// \brief: applies Heapsort to an array. Heapsort has a worst case runtime of O(nlogn) and a abest case between O(n) and O(nlogn). It requires
/// O(1) memory.
template <typename T>
void Heapify(std::pair<T*,size_t> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  for(long i=pIndex(array.second-1);i>=0;i--)
  {
    siftDown(array,static_cast<size_t>(i),array.second,m_fLThan);
  }
}

template <typename T>
void Heapsort(std::pair<T*,size_t> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{ 
  if(array.second<2) return;
  Heapify(array,m_fLThan);
  for(size_t i=array.second-1;i>0;i--)
  {
    swap(array.first[0],array.first[i]);
    siftDown(array,0,i,m_fLThan);
  }
}

template <typename T>
std::vector<T> Heapsort(std::vector<T> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  Heapsort(std::pair<T*,size_t>(&array[0],array.size()),m_fLThan);
  return array;
}

/// \brief: Slightly smarter merge on some cases. Still needs some tuning
template <typename T>
std::pair<T*,size_t> merge_cMem(std::pair<T*,size_t> left,std::pair<T*,size_t> right, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  size_t i=0;
  size_t j=0;

  T* aux=new T[left.second+right.second];
  for(size_t n=0;n<left.second+right.second;n++)
  {
    if(i>=left.second)
      aux[n]=right.first[j++];
    else if(j>=right.second)
      aux[n]=left.first[i++];
    else if (m_fLThan(left.first[i],right.first[j]))
      aux[n]=left.first[i++];
    else
      aux[n]=right.first[j++];
  }
  memcpy(left.first,aux,(left.second+right.second)*sizeof(T));
  delete[] aux;
  return std::pair<T*,size_t>(left.first,left.second+right.second);	
}

template <typename T>
std::pair<T*,size_t> Insertionsort(std::pair<T*,size_t> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  for(size_t i=0; i<array.second; i++)
  {
    T key=array.first[i];
    size_t j=i;
    while ((j > 0) && m_fLThan(key,array.first[j-1]))
    {
      array.first[j]=array.first[j-1];
      j=j-1;
    }
    array.first[j]=key;
  }
  return array;
}


template <typename T>
std::pair<T*,size_t> Mergesort_cMem(std::pair<T*,size_t> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  if (array.second<=_SMALL_ARRAY_SIZE)
  {
    return Insertionsort<T>(array,m_fLThan);
  }
  size_t mid=(array.second)/2;
    
  std::pair<T*,size_t> left=Mergesort_cMem(std::pair<T*,size_t>(array.first,mid),m_fLThan);
  std::pair<T*,size_t> right=Mergesort_cMem(std::pair<T*,size_t>(&array.first[mid],array.second-mid),m_fLThan);
  return merge_cMem(left,right,m_fLThan);
}


///\brief: Mergesort using less memory and being faster
///
/// Mergesort that uses O(n+1) memory (the constant being the size defined in _SMALL_ARRAY_SIZE) passing around 
/// Pairs of pointer+size. Runtime is still O(nlogn) but as it uses less memory allocation operations than the
/// basic version, it runs very fast (Almost as fast as std::sort in most situtions)
template <typename T>
std::vector<T> Mergesort_cMem(std::vector<T> &array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  if(array.size()<=1)
    return array;
  if (array.size()<=_SMALL_ARRAY_SIZE)
  {
    return Insertionsort<T>(array);
  }
  
  size_t mid=array.size()/2;
    
  std::pair<T*,size_t> left=Mergesort_cMem(std::pair<T*,size_t>(array.data(),mid),m_fLThan);
  std::pair<T*,size_t> right=Mergesort_cMem(std::pair<T*,size_t>(&array.data()[mid],array.size()-mid),m_fLThan);
  merge_cMem(left,right,m_fLThan);
  return array;
}

#include <cmath>
///\brief: Recursion deepth calculation
size_t MaxRecursionDepth(size_t arraySize)
{
  return static_cast<size_t>(floor(2*log2(arraySize)));
}

template <typename T>
void Insertionsort_pair(std::pair<T*,size_t> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  for(size_t i=0; i<array.second; i++)
  {
    T key=array.first[i];
    size_t j=i;
    while ((j > 0) && m_fLThan(key,array.first[j-1]))
    {
      array.first[j]=array.first[j-1];
      j=j-1;
    }
    array.first[j]=key;
  }
}

template <typename T>
std::pair<size_t,size_t> partitionPivot(std::pair<T*,size_t> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  size_t pivotIdx=std::rand()%array.second;
  T pivot=array.first[pivotIdx];
  size_t left=0;
  size_t right=array.second-1;
  for(size_t j=0;j<=right;j++)
  {
    if(m_fLThan(array.first[j],pivot))
    {
      if(j>left)
      {
	swap(array.first[j],array.first[left++]);
	j--;
      }
      else
	left++;
      
    }
    else if(m_fLThan(pivot,array.first[j]))
    {
      swap(array.first[j],array.first[right]);
      right--;
      j--;
    }
  }
  return std::pair<size_t,size_t>(left,right);
}

///\brief: Introsort
template<typename T>
void Introsort(std::pair<T*,size_t> array, size_t recDepth, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
  while(array.second>_SMALL_ARRAY_SIZE)
  {
    if(recDepth==0)
    {
      Heapsort(array,m_fLThan);
      return;
    }
		
    recDepth--;

    std::pair<size_t,size_t> pivotIndex=partitionPivot(array,m_fLThan);//fatpartition could be slower, need to check, probably slow
							//std::partition3 to see if faster; Partition can be improved
    std::pair<T*,size_t> left(array.first,pivotIndex.first); //pair of (pointer,size)
    std::pair<T*,size_t> right(&array.first[pivotIndex.second],array.second-pivotIndex.second);
    if(left.second<right.second) swap(left,right);

    Introsort(left,recDepth,m_fLThan);
    array=right;
  }
  Insertionsort_pair(array,m_fLThan);	

}


///\brief: Introsort
template<typename T>
std::vector<T> Introsort(std::vector<T> array, std::function<bool(T,T)> m_fLThan=[](T a,T b){return a<b;})
{
    Introsort(std::pair<T*,size_t>(&array[0],array.size()),MaxRecursionDepth(array.size()),m_fLThan);
    Insertionsort(std::pair<T*,size_t>(&array[0],array.size()),m_fLThan);
    return array;
}


#endif 