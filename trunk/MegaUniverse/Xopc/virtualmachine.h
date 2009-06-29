#ifndef _VIRTUALMACHINE_H_
#define _VIRTUALMACHINE_H_

#include <map>
#include <string>
#include <vector>
#include <QtCore/QThread>
#include "memory.h"

class AbstractController;
class AbstractGui;

class VirtualMachine
    : public QThread
{
    Q_OBJECT

public:
    // constructors & destructor
	VirtualMachine();
	~VirtualMachine();

    // service methods
    void loadBinary(const std::string _filename);
    void connect(AbstractController * _controller);
    void setGui(AbstractGui * _gui);

    // I/O methods
    double readPort(size_t _num) const;
    void writePort(size_t _num, double _val);

    // Access methods
    bool getStatus() const;

    class Binary
    {
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

    public:
    	Binary(VirtualMachine * _vm, size_t _size);
    	~Binary();

        void parseCommand(addr _address, command _comm);
        void executeCommand(addr _address);
    	
        typedef union {
            struct {
                D_Instructions code;
                addr r1, r2;
            } Double;

            struct {
                S_Instructions code;
                ImmMode imm;
                addr r;
            } Single;
        } BinCommand;
        typedef std::map<addr, std::pair<bool, BinCommand> > BinaryCommands;

        friend class VirtualMachine;

    private:
        VirtualMachine * mVM;
        size_t mSize;
        BinaryCommands mCommands;
    };

    typedef std::vector<double> PortsList;

protected:
    void run();

private:
    // service methods
    void checkInputPorts();
    void updateSensors();

    // commands
    void onAdd(addr _r1, addr _r2);
    void onSub(addr _r1, addr _r2);
    void onMult(addr _r1, addr _r2);
    void onDiv(addr _r1, addr _r2);
    void onOutput(addr _r1, addr _r2);
    void onPhi(addr _r1, addr _r2);
    void onCmpz(Binary::ImmMode _mode, addr _r);
    void onSqrt(addr _r);
    void onCopy(addr _r);
    void onInput(addr _r);
    void onNoop(addr);

    // links
    AbstractController * mController;
    AbstractGui * mGui;

    // data
    Memory * mMemory;
    PortsList * mInput;
    PortsList * mInputOld;
    PortsList * mOutput;
    bool mStatus;

    unsigned short mCommandCounter;
    unsigned int mTickCounter;

    size_t mBinaryEdge;
    Binary * mBinary;
};


#endif // _VIRTUALMACHINE_H_
