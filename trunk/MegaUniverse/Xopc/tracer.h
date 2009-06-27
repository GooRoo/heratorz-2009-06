#ifndef _TRACER_H_
#define _TRACER_H_

#include <iosfwd>
#include <string>

#include "memory.h"

class Tracer
{
public:
    ~Tracer();

    static Tracer & inst();

    void traceBin(addr _address, std::string _comm, double _data);
    void traceControl();
	
private:
    Tracer();
    Tracer(const Tracer &);
    Tracer & operator=(const Tracer &);

    void init();

    std::ostream * bin, * control;
};


#endif // _TRACER_H_