#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

class VirtualMachine;

class AbstractController
{
public:

	virtual ~AbstractController() {};

	virtual void setVirtualMachine(VirtualMachine * vm) { m_vm = vm; };

    virtual void OnActuatorsWork() = 0;
    virtual void OnSensorsWork() = 0;

	VirtualMachine * m_vm;
};

#endif // _CONTROLLER_H_