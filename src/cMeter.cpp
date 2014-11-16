#ifdef TEST_RUN
#include "gtest/gtest.h"
#endif

#include "cMeter.h"

/** @brief getUnitSymbol
  *
  * @todo: document this function
  */
template <typename T>
std::string cMeter<T>::getUnitSymbol()
{
    return _unit;
}

/** @brief getUnitName
  *
  * @todo: document this function
  */
template <typename T>
std::string cMeter<T>::getUnitName()
{
    return _dataType;
}

/** @brief setUnitSymbol
  *
  * @todo: document this function
  */
template <typename T>
void cMeter<T>::setUnitSymbol(std::string unitSymbol)
{
    _unit=unitSymbol;
}

/** @brief setUnitName
  *
  * @todo: document this function
  */
template <typename T>
void cMeter<T>::setUnitName(std::string unitName)
{
    _dataType=unitName;
}

/** @brief peek
  *
  * @todo: document this function
  */
template <typename T>
T cMeter<T>::peek()
{
    if(_running)
        return _meterFunction()-_snap;
    return _snap;
}

/** @brief stop
  *
  * @todo: document this function
  */
template <typename T>
void cMeter<T>::stop()
{
    _snap=_meterFunction()-_snap;
    _running=false;
}

/** @brief start
  *
  * @todo: document this function
  */
template <typename T>
void cMeter<T>::start()
{
    _running=true;
    _snap=_meterFunction();
}

/** @brief ~cMeter
  *
  * @todo: document this function
  */
template <typename T>
 cMeter<T>::~cMeter()
{

}

/** @brief cMeter
  *
  * @todo: document this function
  */
template <typename T>
 cMeter<T>::cMeter(func_t func,std::string unitName,std::string unitSymbol)
{
    _unit=unitSymbol;
    _dataType=unitName;
    _running=false;
    _meterFunction=func;
}

/** @brief cMeter
  *
  * @todo: document this function
  */
template <typename T>
 cMeter<T>::cMeter(func_t func)
{
    _running=false;
    _meterFunction=func;
}

template <typename T>
 cMeter<T>::cMeter()
{
    _running=false;
}

template <typename T>
 void cMeter<T>::setMeterFunction(func_t func)
 {
     _meterFunction=func;
 }
