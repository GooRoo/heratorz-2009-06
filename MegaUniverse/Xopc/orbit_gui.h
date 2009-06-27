#ifndef _ORBIT_GUI_H_
#define _ORBIT_GUI_H_

#include "abstract_gui.h"
#include <windows.h>


class OrbitGui
	: public AbstractGui
{
public:
	OrbitGui();
	~OrbitGui();

	void update();

private:
	double m_x, m_y, m_score, m_fuel, m_targetRad;


	//Window
   static ATOM RegisterWndClass();
	bool Init();
   static WCHAR m_szWindowClass[20];
   static WCHAR m_szTitle[20];

   static HWND m_hwnd;   
   static HINSTANCE m_hInstance;
   static DWORD WINAPI MainLoop(LPVOID p);

};

LRESULT CALLBACK WndProc(HWND g_hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif // _ORBIT_GUI_H_