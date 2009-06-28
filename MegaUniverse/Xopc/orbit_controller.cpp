#include "virtualmachine.h"
#include "orbit_controller.h"

void OrbitController::OnActuatorsWork()
{
	static bool f = true;

	if(f)
	{
		mVM->writePort(0x2, 50);
		mVM->writePort(0x3, -50);
	}
	else
	{
		mVM->writePort(0x2, 0);
		mVM->writePort(0x3, 0);
	}
	mVM->writePort(0x3E80, 1001);
	f = false;
}

void OrbitController::OnSensorsWork()
{

}


