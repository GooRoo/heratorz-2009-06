#ifndef _ORBIT_GUI_H_
#define _ORBIT_GUI_H_

#include "abstract_gui.h"


class OrbitGui
	: public AbstractGui
{
public:
	OrbitGui();
	~OrbitGui();

	void update();

private:
	double m_x, m_y, m_score, m_fuel, m_targetRad;

};

#endif // _ORBIT_GUI_H_