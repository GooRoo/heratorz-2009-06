#ifndef _VIRTUALMACHINE_H_
#define _VIRTUALMACHINE_H_

#include <vector>
#include <string>
#include "memory.h"

typedef std::vector<double> PortsList;

class AbstractController;

class VirtualMachine
{
public:
    // constructors & destructor
	VirtualMachine();
	~VirtualMachine();

    // service methods
    void loadBinary(const std::string _filename);
    void connect(AbstractController * _controller);
    void run();

    // I/O methods
    double readPort(size_t _num) const;
    void writePort(size_t _num, double _val);

    // Access methods
    bool getStatus() const;
	
private:
    // service declarations
    enum D_Instructions
    {
        ADD = 0x1,
        SUB,
        MULT,
        DIV,
        OUTPUT,
        PHI
    };

    enum S_Instructions
    {
        NOOP = 0x0,
        CMPZ,
        SQRT,
        COPY,
        INPUT
    };

    enum ImmMode
    {
        LTZ = 0x0,      // less than 0
        LEZ,            // less or equal to 0
        EQZ,            // equal to 0
        GEZ,            // greater or equal to 0
        GTZ             // greater than 0
    };

    // service methods
    void checkInputPorts();
    void updateSensors();
    void executeCommand(addr _address);

    // commands
    void onAdd(addr _r1, addr _r2);
    void onSub(addr _r1, addr _r2);
    void onMult(addr _r1, addr _r2);
    void onDiv(addr _r1, addr _r2);
    void onOutput(addr _r1, addr _r2);
    void onPhi(addr _r1, addr _r2);
    void onCmpz(ImmMode _mode, addr _r);
    void onSqrt(addr _r);
    void onCopy(addr _r);
    void onInput(addr _r);
    void onNoop();

    // links
    AbstractController * mController;

    // data
    Memory * mMemory;
    PortsList * mInput;
    PortsList * mOutput;
    unsigned short mCommandCounter;
    bool mStatus;

};


#endif // _VIRTUALMACHINE_H_
