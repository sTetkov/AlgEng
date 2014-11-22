#ifdef TEST_RUN
#include "gtest/gtest.h"
#endif

#include "cMeter.h"
#include <ctime>

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
  *While meter running this function is slower. If it uses different functions to start
  *and stop measurmentes peek is not possible while meter is running, as it would create
  *too much overhead.
  */
template <typename T>
T cMeter<T>::peek()
{
    if(_running)
      {
	if(_useStopFunc)
	  return(T(0));
	else
	  return _meterFunction()-_snap;
      }
    return _snap;
}

/** @brief stop stops the measurement and saves the diff between now and start
  *
  */
template <typename T>
void cMeter<T>::stop()
{
  if(_useStopFunc)
    _snap=_stopFunction()-_snap;
  else
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
    _useStopFunc=false;
    _meterFunction=func;
}

/** @brief cMeter constructor
  *
  */
template <typename T>
 cMeter<T>::cMeter(func_t func)
{
  _useStopFunc=false;
  _running=false;
  _meterFunction=func;
}

template <typename T>
 cMeter<T>::cMeter()
{
  _useStopFunc=false;
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

/** @brief setSpecificStopFunction sets the function used for measurementsto stop measurements
  *
  */
template <typename T>
 void cMeter<T>::setSpecificStopFunction(func_t func)
 {
   _stopFunction=func;
   _useStopFunc=true;
 }

