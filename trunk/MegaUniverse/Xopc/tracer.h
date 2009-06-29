#ifndef _TRACER_H_
#define _TRACER_H_

#include <iosfwd>
#include <string>
#include <vector>

#include "memory.h"

class BinTracer
{
public:
    static BinTracer & inst();

    void trace(addr _address, std::string _comm, double _data);
    void trace(addr _address, std::string _comm);

private:
    BinTracer();
    BinTracer(const BinTracer &);
    BinTracer & operator=(const BinTracer &);

    std::ostream & bin;
};


class ControlTracer
{
public:

    typedef std::vector<std::pair<int, double> > PortMap;

    ~ControlTracer();

    static ControlTracer & inst(size_t _scenarioID);
    
    void setScenarioID(size_t _newID);

    void trace(unsigned short _tick, const PortMap & _ports);

    struct Header {
        static const size_t magicNumber = 0xCAFEBABE;
        static const size_t teamID = 263;
        size_t scenarioID;

        Header(size_t _scenarioID) : scenarioID(_scenarioID) {}
        friend std::ostream & operator<<(std::ostream &, const Header &);
    };

private:
    ControlTracer(size_t _scenarioID);
    ControlTracer(const ControlTracer &);
    ControlTracer & operator=(const ControlTracer &);

    void init();

    Header mHeader;

    std::ostream * os;
    bool mErrorFlag;
    bool mInitialized;
};

#endif // _TRACER_H_