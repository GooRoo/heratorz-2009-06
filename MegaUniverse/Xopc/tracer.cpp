#include "tracer.h"

#include <iostream>

Tracer::Tracer()
{
    init();
}

Tracer::~Tracer()
{

}

void Tracer::init()
{
    // пока что юзаю стандартные потоки, но потом надо заменить на файлы
    bin = &std::cout;
    control = &std::cout;

    (*bin) << "Addr.\tCommand" << std::endl;
}

Tracer & Tracer::inst()
{
    static Tracer tr;
    return tr;
}

void Tracer::traceBin(addr _address, std::string _comm, double _data)
{
    (*bin) << _address << "\t" << _comm << "\t" << _data << std::endl;
}

void Tracer::traceBin(addr _address, std::string _comm)
{
    (*bin) << _address << "\t" << _comm << std::endl;
}
