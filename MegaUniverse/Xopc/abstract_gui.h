#ifndef _ABSTRACT_GUI_H_
#define _ABSTRACT_GUI_H_

class VirtualMachine;

class AbstractGui
{
public:
	virtual ~AbstractGui() {};

	virtual void update() = 0;

	virtual void setVirtualMachine(VirtualMachine * vm) { m_vm = vm; };

private:
	VirtualMachine * m_vm;

};


#endif // _ABSTRACT_GUI_H_