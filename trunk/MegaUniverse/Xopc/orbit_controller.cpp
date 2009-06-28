#include "virtualmachine.h"
#include "orbit_controller.h"
#include <math.h>

void OrbitController::OnActuatorsWork()
{
	static int f = 0;
	static double r1 = 0;
	static int th = 0;
	switch(f)
	{
	case 0:
		mVM->writePort(0x2, 0);
		mVM->writePort(0x3, 0);
		f++;
		break;
	case 1:	
		{
		double r2 = mVM->readPort(0x4);
		double x = mVM->readPort(0x2);
		double y = mVM->readPort(0x3);

		r1 = sqrt(x*x + y*y);

		double mu = 6.0e24*6.627428e-11;

		double dv1 = sqrt(mu/r1)*(sqrt(2*r2/(r1+r2))-1);
		const float    pi = atan(1.0f)*4;

		th = int(pi*sqrt((r1+r2)*(r1+r2)*(r1+r2)/(8*mu)));

		mVM->writePort(0x2, 0);
		mVM->writePort(0x3, -dv1);
		f++;
		}
		break;
	case 2:
		{
			double x = mVM->readPort(0x2);
			double y = mVM->readPort(0x3);
			double r2 = mVM->readPort(0x4);

		//if(r2 == sqrt(x*x + y*y))
		if(!th)
		{


			double mu = 6.0e24*6.627428e-11;

			double dv1 = sqrt(mu/r2)*(1 - sqrt(2*r1/(r1+r2)));

			mVM->writePort(0x2, 0);
			mVM->writePort(0x3, dv1);
			f++;
		}
		else
		{
			mVM->writePort(0x2, 0);
			mVM->writePort(0x3, 0);
			th--;
		}
		}
		break;
	default:
			mVM->writePort(0x2, 0);
			mVM->writePort(0x3, 0);
		break;
	}
	mVM->writePort(0x3E80, 1001);
}

void OrbitController::OnSensorsWork()
{

}


