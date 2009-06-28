#include "virtualmachine.h"
#include "controller.h"
#include "abstract_gui.h"
#include "tracer.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

VirtualMachine::VirtualMachine()
    : mStatus(false),
      mCommandCounter(0),
	  mBinaryEdge(0),
	  mTickCounter(0),
      mOutput(NULL),
      mController(NULL),
      mGui(NULL),
      mBinary(NULL)
{
    mInput = new PortsList(3);
	mMemory = new Memory();
	mOutput = new PortsList(18);
}


VirtualMachine::~VirtualMachine()
{
    delete mMemory;
    delete mInput;
    delete mOutput;
}


double VirtualMachine::readPort(size_t _num) const
{
    return mOutput->at(_num);
}


void VirtualMachine::writePort(size_t _num, double _val)
{
    switch (_num)
    {
        case 0x2:
            (*mInput)[0] = _val;
            break;

        case 0x3:
            (*mInput)[1] = _val;
            break;

        case 0x3e80:
            (*mInput)[2] = _val;
            break;

        default:
            throw std::invalid_argument("wrong port address specified");
    }
}


bool VirtualMachine::getStatus() const
{
    return mStatus;
}


void VirtualMachine::loadBinary(const std::string _filename)
{
    mBinaryEdge = mMemory->loadFile(_filename);

    if (mBinaryEdge != 0)
    {
        if (mBinary != NULL)
            delete mBinary;

        mBinary = new Binary(this, mBinaryEdge);
    }
    else
    {
        throw std::runtime_error("Unable to load binary");
    }

    for (size_t i = 0; i < mBinaryEdge; i++)
    {
        mBinary->parseCommand(i, mMemory->getCommand(i));
    }
}


void VirtualMachine::connect(AbstractController *_controller)
{
    mController = _controller;
	mController->connect(this);
}


void VirtualMachine::setGui(AbstractGui * _gui)
{
    mGui = _gui;
    mGui->connect(this);
}


void VirtualMachine::run()
{
	if (mMemory == NULL || mController == NULL)
        return;

    do 
    {
        std::clog << "Tick: " << mTickCounter << std::endl;
		checkInputPorts();

		do 
		{
			mBinary->executeCommand(mCommandCounter);
            mCommandCounter++;
		} while (mCommandCounter < mBinaryEdge);

		updateSensors();
		
		mCommandCounter = 0;
		mTickCounter++;

    } while (mTickCounter < 3000);

    mTickCounter = 0;
}

void VirtualMachine::checkInputPorts()
{
    // wait until controller writes into ports
	if (mController)
		mController->OnActuatorsWork();

    // ...
    // check if some updates were done and update the model
    // ...    
}


void VirtualMachine::updateSensors()
{
    // ...
    // updating values on output
    // ...

    // notify controller about it
	if (mController)
		mController->OnSensorsWork();

    if (mGui && mTickCounter % 50 == 0)
        mGui->update();
}

void VirtualMachine::onAdd(addr _r1, addr _r2)
{
    mMemory->setData(
            mCommandCounter,
            mMemory->getData(_r1) + mMemory->getData(_r2)
        );
}


void VirtualMachine::onSub(addr _r1, addr _r2)
{
    mMemory->setData(
            mCommandCounter,
            mMemory->getData(_r1) - mMemory->getData(_r2)
        );
}


void VirtualMachine::onMult(addr _r1, addr _r2)
{
    mMemory->setData(
            mCommandCounter,
            mMemory->getData(_r1) * mMemory->getData(_r2)
        );
}


void VirtualMachine::onDiv(addr _r1, addr _r2)
{
    mMemory->setData(
            mCommandCounter,
            mMemory->getData(_r1) / mMemory->getData(_r2)
        );
}


void VirtualMachine::onOutput(addr _r1, addr _r2)
{
    mOutput->at(_r1) = mMemory->getData(_r2);
}


void VirtualMachine::onPhi(addr _r1, addr _r2)
{
    mMemory->setData(
            mCommandCounter,
            (mStatus) ? mMemory->getData(_r1) : mMemory->getData(_r2)
        );
}

bool less0(double _val) { return _val < 0; }
bool less_equal0(double _val) { return _val <= 0; }
bool equal_to0(double _val) { return _val == 0; }
bool greater_equal0(double _val) { return _val >= 0; }
bool greater0(double _val) { return _val > 0; }

void VirtualMachine::onCmpz(Binary::ImmMode _mode, addr _r)
{
    static bool (* predicates[])(double) = {
        &less0, &less_equal0, &equal_to0, &greater0, &greater_equal0
    };

    if (_mode < Binary::LTZ || _mode > Binary::GTZ)
        throw std::invalid_argument("Invalid immediate mode argument");

    mStatus = (*(predicates[_mode]))(mMemory->getData(_r));
}

void VirtualMachine::onSqrt(addr _r)
{
    mMemory->setData(
            mCommandCounter,
            sqrt(mMemory->getData(_r))
        );
}

void VirtualMachine::onCopy(addr _r)
{
    mMemory->setData(mCommandCounter, mMemory->getData(_r));
}

void VirtualMachine::onInput(addr _r)
{
    switch (_r)
    {
        case 0x2:
            mMemory->setData(mCommandCounter, (*mInput)[0]);
            break;

        case 0x3:
            mMemory->setData(mCommandCounter, (*mInput)[1]);
            break;

        case 0x3e80:
            mMemory->setData(mCommandCounter, (*mInput)[2]);
            break;

        default:
            throw std::invalid_argument("Invalid input port address");
    }
}

void VirtualMachine::onNoop(addr)
{
    // Nothing to do...
}


//////////////////////////////////////////////////////////////////////////
// VirtualMachine::Binary
//////////////////////////////////////////////////////////////////////////


VirtualMachine::Binary::Binary(VirtualMachine *_vm, size_t _size)
    : mSize(_size), mVM(_vm)
{
}

VirtualMachine::Binary::~Binary()
{
}

void VirtualMachine::Binary::parseCommand(addr _address, command _comm)
{
    std::stringstream debugStr;
    BinCommand newCommand;
    if (_comm.s_command.flag == 0x0)
    {
        addr r = _comm.s_command.r1;
        newCommand.Single.code = static_cast<S_Instructions>(_comm.s_command.op);
        newCommand.Single.r = r;
        switch (_comm.s_command.op)
        {
            case NOOP:
                debugStr << "Noop";
                break;

            case CMPZ:
                debugStr << "Cmpz " <<  r;

                newCommand.Single.imm = static_cast<ImmMode>(_comm.s_command.cmp_type);
                switch (_comm.s_command.cmp_type)
                {
                    case LTZ:
                        debugStr << " < 0";
                        break;

                    case LEZ:
                        debugStr << " <= 0";
                        break;

                    case EQZ:
                        debugStr << " == 0";
                        break;

                    case GEZ:
                        debugStr << " >= 0";
                        break;

                    case GTZ:
                        debugStr << " > 0";
                        break;

                    default:
                        std::cerr << "Binary file is corrupted or we have crooked hands" << std::endl;
                }
                break;

            case SQRT:
                debugStr << "Sqrt ";
                break;

            case COPY:
                debugStr << "Copy ";
                break;

            case INPUT:
                debugStr << "Input ";
                break;

            default:
                std::cerr << "Binary file is corrupted or we have crooked hands" << std::endl;
        }
        if (_comm.s_command.op != CMPZ && _comm.s_command.op != NOOP)
            debugStr << r;

        mCommands[_address] = std::make_pair(true, newCommand);

    }
    else
    {
        addr r1 = _comm.d_command.r1, r2 = _comm.d_command.r2;

        newCommand.Double.r1 = r1;
        newCommand.Double.r2 = r2;
        newCommand.Double.code = static_cast<D_Instructions>(_comm.d_command.op);

        switch (_comm.d_command.op)
        {
            case ADD:
                debugStr << "Add ";
                break;

            case SUB:
                debugStr << "Sub ";
                break;

            case MULT:
                debugStr << "Mult ";
                break;

            case DIV:
                debugStr << "Div ";
                break;

            case OUTPUT:
                debugStr << "Output ";
                break;

            case PHI:
                debugStr << "Phi ";
                break;

            default:
                std::cerr << "Binary file is corrupted or we have crooked hands" << std::endl;
        }
        debugStr << r1 << " " << r2;

        mCommands[_address] = std::make_pair(false, newCommand);

    }

    BinTracer::inst().trace(_address, debugStr.str());
}

void VirtualMachine::Binary::executeCommand(addr _address)
{
    if (mCommands[_address].first)
    {
        static void (VirtualMachine::*sFuncs[])(addr) = {
            &VirtualMachine::onNoop,
            NULL,
            &VirtualMachine::onSqrt,
            &VirtualMachine::onCopy,
            &VirtualMachine::onInput
        };
        
        S_Instructions code = mCommands[_address].second.Single.code;
        addr r = mCommands[_address].second.Single.r;

        if (code == CMPZ)
            mVM->onCmpz(mCommands[_address].second.Single.imm, r);
        else
            (mVM->*sFuncs[code])(r);
    }
    else
    {
        static void (VirtualMachine::*dFuncs[])(addr, addr) = {
            &VirtualMachine::onAdd,
            &VirtualMachine::onSub,
            &VirtualMachine::onMult,
            &VirtualMachine::onDiv,
            &VirtualMachine::onOutput,
            &VirtualMachine::onPhi
        };

        D_Instructions code = mCommands[_address].second.Double.code;
        addr r1 = mCommands[_address].second.Double.r1,
            r2 = mCommands[_address].second.Double.r2;

        (mVM->*dFuncs[code - ADD])(r1, r2);
    }
}
