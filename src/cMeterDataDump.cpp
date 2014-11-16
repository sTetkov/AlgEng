#include <stdio.h>
#include "cMeterDataDump.h"
/** @brief (one liner)
  *
  * (documentation goes here)
  */
template <typename T>
void cMeterDataDump<T>::dumpData()
{
    FILE* output;
    output=fopen(_filename.c_str(),"w");
    char col1[]="n#";
    char col2[21];
    char col3[21];
    char col4[21];
    char col5[21];
    char col6[31];
    char formatTitleString[119]="%7s %19s %19s %19s %19s %19s\n";
    char formatDataString[]=    "%7i %19i %19i %19i %19i     ";
    snprintf(col2,20,"min[%s]",_meterFunc->getUnitSymbol().c_str());
    snprintf(col3,20,"max[%s]",_meterFunc->getUnitSymbol().c_str());
    snprintf(col4,20,"mean[%s]",_meterFunc->getUnitSymbol().c_str());
    snprintf(col5,20,"sd[%s]",_meterFunc->getUnitSymbol().c_str());
    snprintf(col6,30,"measurements[%s]",_meterFunc->getUnitSymbol().c_str());
    fprintf(output,formatTitleString,col1,col2,col3,col4,col5,col6);

    if(_data->size() == 0)
        _data->push_back(_currentBatch);
    for (int i=0; i<_data->size(); i++)
    {
        T* elemArray=processData(*_data->at(i)); //Need an array to pass the data to the format string in fprintf
        fprintf(output,formatDataString,i,min,max,mean,sd);
        for (int j=0; j< _data->at(i)->size(); j++)
        {
            fprintf( output, "%i ",elemArray[j]);
        }
        fprintf(output,"\n");
        delete[] elemArray;
    }
    fclose(output);
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
template<typename T>
T* cMeterDataDump<T>::processData(std::vector<T> v)
{
    if(v.size()<=0)
        return NULL;
    T* retVec=new T[v.size()];
    if(_needsConversion)
        for(int i=0; i<v.size(); i++)
            retVec[i]=_conversionFunction(v[i]);
    min=v[0];
    max=T(0);
    T sum(0);
    sd=T(0);
    for (int i=0; i< v.size(); i++)
        {
            if(v[i]<min)
                min=v[i];
            if(v[i]>max)
                max=v[i];
            sum+=v[i];
        }
    mean=sum/v.size();
    return retVec;
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
template<typename T>
void cMeterDataDump<T>::resetData()
{
    delete _currentBatch;
    delete _data;
    _currentBatch=new std::vector<T>();
    _data=new std::vector<std::vector<T>* >();
}




/** @brief (one liner)
  *
  * (documentation goes here)
  */
template <typename T>
void cMeterDataDump<T>::setFilename(std::string filename)
{
    _filename=filename;
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
template <typename T>
void cMeterDataDump<T>::setConversionFunction(std::function<T(T)> func)
{
    _conversionFunction=func;
    _needsConversion=true;
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
template <typename T>
void cMeterDataDump<T>::StoreBatch()
{
    _data->push_back(_currentBatch);
    _currentBatch=new std::vector<T>();
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
template <typename T>
void cMeterDataDump<T>::StopMeter()
{
    _meterFunc->stop();
    _currentBatch->push_back(_meterFunc->peek());
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
template <typename T>
void cMeterDataDump<T>::StartMeter()
{
    _meterFunc->start();
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
 template <typename T>
 cMeterDataDump<T>::cMeterDataDump(cMeter<T>* meter)
{
    _meterFunc=meter;
    _filename=meter->getUnitName()+".dmp";
    _needsConversion=false;
    _data=new std::vector<std::vector<T>* >();
    _currentBatch=new std::vector<T>();
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
 template <typename T>
 cMeterDataDump<T>::~cMeterDataDump()
{
    delete _currentBatch;
    delete _data;
}
