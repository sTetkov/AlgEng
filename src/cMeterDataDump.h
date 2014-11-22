#ifndef METER_DATA_DUMP_H
#define METER_DATA_DUMP_H

#include "cMeter.h"
#include <string>
#include <functional>
#include <vector>

template <typename T>
class cMeterDataDump
{
    public:
    cMeterDataDump(cMeter<T>*);
    ~cMeterDataDump();

    void StartMeter();
    void StopMeter();
    void StoreBatch();
    void setConversionFunction(std::function<T(T)>);
    void setFilename(std::string);
    void dumpData();
    void resetData();

    private:
    bool _needsConversion;
    std::string _filename;
    std::function<T(T)> _conversionFunction;
    cMeter<T>* _meterFunc;
    std::vector<T>* _currentBatch;
    std::vector<std::vector<T>* >* _data;

    T min;
    T max;
    double mean;
    double sd;

    T* processData(std::vector<T>);
};
#endif
