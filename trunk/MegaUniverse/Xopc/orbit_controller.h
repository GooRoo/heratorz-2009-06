#ifndef _ORBIT_CONTROLLER_H_
#define _ORBIT_CONTROLLER_H_
#include "controller.h"

class OrbitController
	: public AbstractController
{
public:
    virtual ~OrbitController() {}

    virtual void OnActuatorsWork();
    virtual void OnSensorsWork();

private:
};

#endif // _ORBIT_CONTROLLER_H_