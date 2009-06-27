#include "virtualmachine.h"
#include "controller.h"
#include "tracer.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

VirtualMachine::VirtualMachine()
    : mStatus(false),
      mCommandCounter(0),
	  mBinaryEdge(0),
	  mTickCounter(0),
      mOutput(NULL),
      mController(NULL)
{

    mInput = new PortsList(PORT_MEMORY_SIZE);
	mMemory = new Memory();
	mOutput = new PortsList(PORT_MEMORY_SIZE);
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
    mInput->at(_num) = _val;
}


bool VirtualMachine::getStatus() const
{
    return mStatus;
}


void VirtualMachine::loadBinary(const std::string _filename)
{
    mBinaryEdge = mMemory->loadFile(_filename);
}


void VirtualMachine::connect(AbstractController *_controller)
{
    mController = _controller;
	mController->setVirtualMachine(this);
}


void VirtualMachine::run()
{
	if (mMemory == NULL || mController == NULL)
        return;

    do 
    {
		checkInputPorts();

		do 
		{
			executeCommand(mCommandCounter);
			mCommandCounter++;
		} while (mCommandCounter < mBinaryEdge);

		updateSensors();
		
		mCommandCounter = 0;
		mTickCounter++;

    } while (mTickCounter < 20);
	mTickCounter = 0;

}

void VirtualMachine::checkInputPorts()
{
    // wait until controller writes into ports
	if(mController)
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
	if(mController)
		mController->OnSensorsWork();
}

void VirtualMachine::executeCommand(addr _address)
{
    command comm = mMemory->getCommand(_address);
    double  data = mMemory->getData(_address);

    std::string debugStr = "";
    if (comm.s_command.flag == 0x0)
    {
        addr r = comm.s_command.r1;
        switch (comm.s_command.op)
        {
            case NOOP:
                debugStr += "Noop";
                onNoop();
                break;

            case CMPZ:
                debugStr += "Cmpz ";
                //debugStr += r;    // @todo преобразовать число в строку
				switch (comm.s_command.cmp_type)
                {
                    case LTZ:
                        debugStr += " < 0";
                        break;

                    case LEZ:
                        debugStr += " <= 0";
                        break;

                    case EQZ:
                        debugStr += " == 0";
                        break;

                    case GEZ:
                        debugStr += " >= 0";
                        break;

                    case GTZ:
                        debugStr += " > 0";
                        break;

                    default:
                        std::cerr << "Binary file is corrupted or we have crooked hands" << std::endl;
                }
                onCmpz(static_cast<VirtualMachine::ImmMode>(comm.s_command.cmp_type), r);
                break;

            case SQRT:
                debugStr += "Sqrt ";
                onSqrt(r);
                break;

            case COPY:
                debugStr += "Copy ";
                onCopy(r);
                break;

            case INPUT:
                debugStr += "Input ";
                onInput(r);
                break;

            default:
                std::cerr << "Binary file is corrupted or we have crooked hands" << std::endl;
        }
        //if (comm.s_command.op != CMPZ && comm.s_command.op != NOOP)
            //debugStr += r;    // @todo преобразовать число в строку
    }
    else
    {
        addr r1 = comm.d_command.r1, r2 = comm.d_command.r2;
        switch (comm.d_command.op)
        {
            case ADD:
                debugStr += "Add ";
                onAdd(r1, r2);
                break;

            case SUB:
                debugStr += "Sub ";
                onSub(r1, r2);
                break;

            case MULT:
                debugStr += "Mult ";
                onMult(r1, r2);
                break;

            case DIV:
                debugStr += "Div ";
                onDiv(r1, r2);
                break;

            case OUTPUT:
                debugStr += "Output ";
                onOutput(r1, r2);
                break;

            case PHI:
                debugStr += "Phi ";
                onPhi(r1, r2);
                break;

            default:
                std::cerr << "Binary file is corrupted or we have crooked hands" << std::endl;
        }
        //debugStr += r1; debugStr += " "; debugStr += r2;    // @todo преобразовать число в строку
    }

    Tracer::inst().traceBin(_address, debugStr, data);
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
	if(( _r1 >= 0 ) && ( _r1 < PORT_MEMORY_SIZE ) && ( _r2 >= 0 ) && ( _r2 < mMemory->getSize() ))
	{
       	if(mOutput)
			mOutput->at(_r1) = mMemory->getData(_r2);
	}
	else
            throw std::invalid_argument("Invalid input port address");


}


void VirtualMachine::onPhi(addr _r1, addr _r2)
{
    mMemory->setData(
            mCommandCounter,
            (mStatus) ? mMemory->getData(_r1) : mMemory->getData(_r2)
        );
}


void VirtualMachine::onCmpz(ImmMode _mode, addr _r)
{
	switch(_mode)
	{
	case LTZ:
		mStatus = (mMemory->getData(_r) < 0);
		break;
	case LEZ:
		mStatus = (mMemory->getData(_r) <= 0);
		break;
	case EQZ:
		mStatus = (mMemory->getData(_r) == 0);
		break;
	case GEZ:
		mStatus = (mMemory->getData(_r) >= 0);
		break;
	case GTZ:
		mStatus = (mMemory->getData(_r) > 0);
		break;
	default:
		mStatus = false;
		throw std::invalid_argument("Invalid immediate mode argument");
	}
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
	if( ( _r >= 0 ) && ( _r < PORT_MEMORY_SIZE ) )
            mMemory->setData(mCommandCounter, (*mInput)[_r]);
	else
            throw std::invalid_argument("Invalid input port address");

}

void VirtualMachine::onNoop()
{
    // Nothing to do...
}
