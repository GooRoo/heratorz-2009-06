#include "virtualmachine.h"
#include "orbit_controller.h"
#include <math.h>
#include <iostream>

void ortvector(double &vx, double &vy, double x, double y, double dv, bool sign)
{

	vx =  dv * sqrt( 1 / ( ((x/y)*(x/y)) + 1) );

	if(!sign)
		vx = -vx;

	vy = - x*vx/y;
}

void OrbitController::OnActuatorsWork()
{
	//const double mu = 6.0e24*6.627428e-11;
	const double mu = 398600.4418e9;
	static int f = 0;
	static double r1 = 0;
	static int th = 0;
	double x = mVM->readPort(0x2);
	double y = mVM->readPort(0x3);
	double r2 = mVM->readPort(0x4);

	switch(f)
	{
	case 0:
		mVM->writePort(0x2, 0);
		mVM->writePort(0x3, 0);
		f++;
		break;
	case 1:	
		{
		r1 = sqrt(x*x + y*y);

		double dv = sqrt(mu/r1)*(sqrt(2*r2/(r1+r2))-1)+8;
		const float    pi = atan(1.0f)*4;

//		th = int(pi*sqrt((r1+r2)*(r1+r2)*(r1+r2)/(8*mu)));

	double vx, vy;
	ortvector(vx, vy, x, y, dv, false);

		mVM->writePort(0x2, vx);
		mVM->writePort(0x3, vy);
		f++;
		}
		break;
	case 2:
		{
			double currad = sqrt(x*x+y*y);
		if( currad >= r2 - 1000  )
		{

			double dv1 = sqrt(mu/r2)*(1 - sqrt(2*r1/(r1+r2)));

			double vx, vy;
			ortvector(vx, vy, x, y, dv1, true);

			mVM->writePort(0x2, vx);
			mVM->writePort(0x3, vy);
			f++;
		}
		else
		{
			mVM->writePort(0x2, 0);
			mVM->writePort(0x3, 0);
			
			std::cout<<"Target rad:  "<< r2<< std::endl;
			std::cout<<"Current rad: "<< sqrt(x*x+y*y)<< std::endl;
		}
		}
		break;
	default:
			mVM->writePort(0x2, 0);
			mVM->writePort(0x3, 0);
			std::cout<< std::endl<< std::endl<< std::endl;
			std::cout<<"Target rad:  "<< r2<< std::endl;
			std::cout<<"Current rad: "<< sqrt(x*x+y*y)<< std::endl;

		break;
	}
	mVM->writePort(0x3E80, 1002);
}

void OrbitController::OnSensorsWork()
{

}


