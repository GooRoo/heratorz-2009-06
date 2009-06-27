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
      mMemory(NULL),
      mOutput(NULL),
      mController(NULL)
{
    mInput = new PortsList(3);
}


VirtualMachine::~VirtualMachine()
{
    delete [] mMemory;
    delete [] mInput;
    delete [] mOutput;
}


double VirtualMachine::readPort(size_t _num) const
{
    return mInput->at(_num);
}


void VirtualMachine::writePort(size_t _num, double _val)
{
    mOutput->at(_num) = _val;
}


bool VirtualMachine::getStatus() const
{
    return mStatus;
}


void VirtualMachine::loadBinary(const std::string _filename)
{
    mMemory->loadFile(_filename);
}


void VirtualMachine::connect(AbstractController *_controller)
{
    mController = _controller;

    if (mController != NULL)
    {
        delete [] mOutput;
        mOutput = NULL;
    }

    mOutput = new PortsList(mController->getNumberOfSensorSlots());
}


void VirtualMachine::run()
{
    if (mMemory == NULL || mController == NULL)
        return;

    do 
    {
        checkInputPorts();
        executeCommand(mCommandCounter);
        mCommandCounter++;
        updateSensors();
    } while (mCommandCounter < 3000000);
}


void VirtualMachine::checkInputPorts()
{
    // wait until controller writes into ports
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
                switch (comm.s_command.imm)
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
                onCmpz(static_cast<VirtualMachine::ImmMode>(comm.s_command.imm), r);
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
    mOutput->at(_r1) = mMemory->getData(_r2);
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
    mStatus = (_mode == LTZ) ? (_r < 0)
        : (_mode == LEZ) ? (_r <= 0)
        : (_mode == EQZ) ? (_r == 0)
        : (_mode == GEZ) ? (_r >= 0)
        : (_mode == GTZ) ? (_r > 0)
        : false, throw std::invalid_argument("Invalid immediate mode argument");
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

void VirtualMachine::onNoop()
{
    // Nothing to do...
}
