#ifndef _VIRTUALMACHINE_H_
#define _VIRTUALMACHINE_H_


class VirtualMachine
{
public:
	VirtualMachine();
	~VirtualMachine();


    // Access methods
    bool getStatus() const;
	
protected:

private:
    size_t mCommandCounter;
    bool mStatus;
};


#endif // _VIRTUALMACHINE_H_
#endif