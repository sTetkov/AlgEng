#ifndef CLASS_SORTABLE_ARRAY
#define CLASS_SORTABLE_ARRAY

#define _SMALL_ARRAY_SIZE 80

#include <vector>
#include <functional>
#include <utility>


template<typename T>
class SortableArray
{
public:
  SortableArray();
  SortableArray(std::vector<T>);

  void setElementsAreComparable();
  
  void AddElement(T elem);
  void AddElement(std::vector<T> elem);
  
  void AddLessThanFunction(std::function<bool(T,T)>);
  void AddLessOrEqualThanFunction(std::function<bool(T,T)>);
  void AddEqualFunction(std::function<bool(T,T)>);
  
  std::vector<T> GetVector();
  T* GetArray();
   
  void Internal_InsertionSort();
  void Internal_QuickSort();
  void Internal_QuickSort_opt();//quicksort optimized for small arrays, repeated elements and sorted (straight and reversed) arrays
  void Internal_Mergesort();
  
  std::vector<T> Quicksort(std::vector<T>);//usable on not member vectors
  std::vector<T> Quicksort_opt(std::vector<T>);//usable on not member vectors, optimized
  std::vector<T> MergeSort(std::vector<T>);//usable on not member vectors
  std::vector<T> InsertionSort(std::vector<T>);//usable on not member vectors 
    
private:
 
  void swap(T&,T&);
  
  std::vector<T> merge(std::vector<T>, std::vector<T>);

  std::vector<T> m_vInternalVector;


  std::vector<T> Quicksort(std::vector<T>&,int,int);//usable on not member vectors
  int qsPartition(std::vector<T>&, int,int);
  std::vector<T> Quicksort_opt(std::vector<T>&,int,int);//usable on not member vectors, optimized
  std::pair<int, int> qsPartition_opt(std::vector<T>&, int,int);
  void InsertionSort(std::vector<T>&,int,int);
  

  bool m_bLTFunAvailable;
  bool m_bLETFunAvailable;
  bool m_bETFunAvailable;
  
  std::function<bool(T,T)> m_fLThan;
  std::function<bool(T,T)> m_fLEThan;
  std::function<bool(T,T)> m_fCompare;
  
};

#endif