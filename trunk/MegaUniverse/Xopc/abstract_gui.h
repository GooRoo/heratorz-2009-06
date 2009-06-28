#ifndef _ABSTRACT_GUI_H_
#define _ABSTRACT_GUI_H_

class VirtualMachine;

class AbstractGui
{
public:
	virtual ~AbstractGui() {};

	virtual void update() = 0;

	virtual void connect(VirtualMachine * _vm) { mVM = _vm; };

protected:
	VirtualMachine * mVM;
};


#endif // _ABSTRACT_GUI_H_