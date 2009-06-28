#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

class VirtualMachine;

class AbstractController
{
public:

	virtual ~AbstractController() {};

	virtual void connect(VirtualMachine * vm) { mVM = vm; };

    virtual void OnActuatorsWork() = 0;
    virtual void OnSensorsWork() = 0;

protected:
	VirtualMachine * mVM;
};

#endif // _CONTROLLER_H_