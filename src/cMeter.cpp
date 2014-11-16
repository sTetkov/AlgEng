#ifdef TEST_RUN
#include "gtest/gtest.h"
#endif

#include "cMeter.h"

/** @brief getUnitSymbol returns the measurement unit used by the meter
  *
  *
  */
template <typename T>
std::string cMeter<T>::getUnitSymbol()
{
    return _unit;
}

/** @brief getUnitName returns the measurement type of the meter (time, cycles ...)
  *
  */
template <typename T>
std::string cMeter<T>::getUnitName()
{
    return _dataType;
}

/** @brief setUnitSymbol sets the measurement unit used by the meter
  *
  */
template <typename T>
void cMeter<T>::setUnitSymbol(std::string unitSymbol)
{
    _unit=unitSymbol;
}

/** @brief setUnitName sets the measurement type of the meter (time, cycles ...)
  *
  */
template <typename T>
void cMeter<T>::setUnitName(std::string unitName)
{
    _dataType=unitName;
}

/** @brief peek returns the current measurement or the last registerd measure
  *
  */
template <typename T>
T cMeter<T>::peek()
{
    if(_running)
        return _meterFunction()-_snap;
    return _snap;
}

/** @brief stop stops the measurement and saves the diff between now and start
  *
  */
template <typename T>
void cMeter<T>::stop()
{
    _snap=_meterFunction()-_snap;
    _running=false;
}

/** @brief start starts the measurement and saves now
  *
  */
template <typename T>
void cMeter<T>::start()
{
    _running=true;
    _snap=_meterFunction();
}

/** @brief cMeter constructor with 3 parameters
  *
  */
template <typename T>
 cMeter<T>::cMeter(func_t func,std::string unitName,std::string unitSymbol)
{
    _unit=unitSymbol;
    _dataType=unitName;
    _running=false;
    _meterFunction=func;
}

/** @brief cMeter constructor
  *
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

/** @brief setMeterFunction sets the function used for measurements
  *
  */
template <typename T>
 void cMeter<T>::setMeterFunction(func_t func)
 {
     _meterFunction=func;
 }
