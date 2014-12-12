#ifndef _SORTING_ALGORITHM
#define _SORTING_ALGORITHM

#include <vector>
#include <functional>
#include <utility>

static const size_t _SMALL_ARRAY_SIZE = 80;

template <typename T>
std::vector<T> Mergesort(std::vector<T>, std::function<bool(T,T)> m_fLThan=[](T a, T b){return a<b;});

template <typename T>
std::vector<T> Insertionsort(std::vector<T>,std::function<bool(T,T)> m_fLThan=[](T a, T b){return a<b;});

template <typename T>
std::vector<T> Quicksort(std::vector<T>,std::function<bool(T,T)> m_fLThan=[](T a, T b){return a<b;});

template <typename T>
std::vector<T> Quicksort_opt(std::vector<T>,std::function<bool(T,T)> m_fLThan=[](T a, T b){return a<b;});

template <typename T>
void swap(T&, T&);

template <typename T>
void Insertionsort(std::vector<T> &,size_t ,size_t , std::function<bool(T,T)> );

template <typename T>
std::vector<T> merge(std::vector<T> , std::vector<T> , std::function<bool(T,T)> );

template<typename T>
size_t qsPartition(std::vector<T> &,size_t, size_t, std::function<bool(T,T)> );

template <typename T>
std::vector<T> Quicksort(std::vector<T> &,size_t ,size_t , std::function<bool(T,T)> );

template<typename T>
std::pair<size_t,size_t> qsPartition_opt(std::vector<T> &,size_t , size_t , std::function<bool(T,T)> );

template <typename T>
std::vector<T> Quicksort_opt(std::vector<T>&,size_t,size_t, std::function<bool(T,T)>);

#endif