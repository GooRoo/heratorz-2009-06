#include "tracer.h"

#include <fstream>
#include <iostream>
#include <sstream>

BinTracer::BinTracer()
    : bin(std::clog)
{
    bin << "Addr.\tCommand" << std::endl;
}

BinTracer & BinTracer::inst()
{
    static BinTracer tr;
    return tr;
}

void BinTracer::trace(addr _address, std::string _comm, double _data)
{
    bin << _address << "\t" << _comm << "\t" << _data << std::endl;
}

void BinTracer::trace(addr _address, std::string _comm)
{
    bin << _address << "\t" << _comm << std::endl;
}


//////////////////////////////////////////////////////////////////////////


ControlTracer::ControlTracer(size_t _scenarioID)
    : mHeader(_scenarioID), mInitialized(true)
{
    init();
}

ControlTracer::~ControlTracer()
{
    if (!mErrorFlag)
    {
        (static_cast<std::ofstream *>(os))->close();
    }
}

ControlTracer & ControlTracer::inst(size_t _scenarioID)
{
    static ControlTracer ct(_scenarioID);
    if (_scenarioID != ct.mHeader.scenarioID)
    {
        ct.setScenarioID(_scenarioID);
    }
    return ct;
}

void ControlTracer::init()
{
    mErrorFlag = false;
    std::stringstream filename;
    filename << "trace" << mHeader.scenarioID << ".osf";

    os = new std::ofstream(filename.str().c_str(), std::ios::out | std::ios::binary);

    if (!os->good())
    {
        std::cerr << "Unable to open file for writing.";
        mErrorFlag = true;
        os = &std::clog;
    }

    std::ostream & ros = *os;
    
    ros << mHeader << std::flush;
}

void ControlTracer::setScenarioID(size_t _newID)
{
    mHeader.scenarioID = _newID;

    if (!mErrorFlag)
    {
        (static_cast<std::ofstream *>(os))->close();
    }

    init();
}

std::ostream & operator<<(std::ostream & _os, const ControlTracer::Header & _header)
{
    _os.write(reinterpret_cast<const char*>(&_header.magicNumber), 4);
    _os.write(reinterpret_cast<const char*>(&_header.teamID), 4);
    _os.write(reinterpret_cast<const char*>(&_header.scenarioID), 4);
    return _os;
}
