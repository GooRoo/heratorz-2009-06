#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

class AbstractController
{
public:
    virtual ~AbstractController() {}

    virtual void OnActuatorsWork() = 0;
    virtual void OnSensorsWork() = 0;

    virtual size_t getNumberOfSensorSlots() const = 0;
};

#endif // _CONTROLLER_H_