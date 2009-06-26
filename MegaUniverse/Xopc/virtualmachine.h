#ifndef _VIRTUALMACHINE_H_
#define _VIRTUALMACHINE_H_

#include <list>

class VirtualMachine
{
public:
	VirtualMachine();
	~VirtualMachine();


    // Access methods
    bool getStatus() const;
	
private:

    size_t mCommandCounter;
    bool mStatus;
};


#endif // _VIRTUALMACHINE_H_
#endif