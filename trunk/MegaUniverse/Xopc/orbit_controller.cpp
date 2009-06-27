#include "virtualmachine.h"
#include "orbit_controller.h"

void OrbitController::OnActuatorsWork()
{
	static bool f = true;

	if(f)
	{
		m_vm->writePort(0x2, 50);
		m_vm->writePort(0x3, -50);
	}
	else
	{
		m_vm->writePort(0x2, 0);
		m_vm->writePort(0x3, 0);
	}
	m_vm->writePort(0x3E80, 1001);
	f=false;
}

void OrbitController::OnSensorsWork()
{

}


