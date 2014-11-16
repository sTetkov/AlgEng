#ifndef CLASS_METER_H
#define CLASS_METER_H

#include <cstddef>
#include <functional>
#include <string>



template <typename T>
class cMeter
{
    public:
    typedef std::function<T(void)> func_t;

    cMeter();
    cMeter(func_t);
    cMeter(func_t,std::string,std::string);
    virtual ~cMeter();

    void setMeterFunction(func_t);

    void start();
    void stop();
    T peek();

    void setUnitName(std::string);
    void setUnitSymbol(std::string);

    std::string getUnitName();
    std::string getUnitSymbol();

    private:

    func_t _meterFunction;

    T _snap;
    bool _running;
    std::string _dataType;
    std::string _unit;
};
#endif
