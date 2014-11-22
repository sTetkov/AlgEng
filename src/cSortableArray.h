#ifndef CLASS_SORTABLE_ARRAY
#define CLASS_SORTABLE_ARRAY

#include <vector>
#include <functional>

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
   
  void InsertionSort();
  void QuickSort();
  
private:
  void quicksort(int,int);
  int qsPartition(int,int);
  
  std::vector<T> m_vInternalVector;

  bool m_bLTFunAvailable;
  bool m_bLETFunAvailable;
  bool m_bETFunAvailable;
  
  std::function<bool(T,T)> m_fLThan;
  std::function<bool(T,T)> m_fLEThan;
  std::function<bool(T,T)> m_fCompare;
  
};

#endif