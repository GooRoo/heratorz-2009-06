#include "virtualmachine.h"


VirtualMachine::VirtualMachine()
    : mStatus(false), mCommandCounter(0)
{

}


VirtualMachine::getStatus() const
{
    return mStatus;
}