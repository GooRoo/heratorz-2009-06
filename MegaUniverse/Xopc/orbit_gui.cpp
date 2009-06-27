#include "orbit_gui.h"



   WCHAR OrbitGui::m_szWindowClass[20]=TEXT("orbit1");
   WCHAR OrbitGui::m_szTitle[20]=TEXT("orbit1");
   HWND OrbitGui::m_hwnd=NULL;   
   HINSTANCE OrbitGui::m_hInstance=NULL;



OrbitGui::OrbitGui()
:m_fuel(0), m_x(0), m_y(0), m_score(0), m_targetRad(0)
{
	Init();

   HANDLE h= CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)MainLoop,NULL,NULL,NULL);
   

}

DWORD WINAPI OrbitGui::MainLoop(LPVOID p)
{
   MSG msg;
   while (GetMessage(&msg, m_hwnd, 0, 0)) 
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   return 0;
}

ATOM OrbitGui::RegisterWndClass()
{
   WNDCLASS wc;

   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = m_hInstance;
   wc.hIcon         = NULL;
   wc.hCursor       = 0;
   wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
   wc.lpszMenuName  = 0;
   wc.lpszClassName = m_szWindowClass;

   return RegisterClass(&wc);
}

bool OrbitGui::Init()
{
	RegisterWndClass();

   m_hwnd = CreateWindow(m_szWindowClass, m_szTitle, WS_VISIBLE, 0, 0,  500,  500, NULL, NULL, m_hInstance, NULL);

    if (!m_hwnd)
    {
        return false;
    }


    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
   return true;
}

LRESULT CALLBACK WndProc(HWND g_hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int x0=250,y0=250;
    switch (message) 
    {
        case WM_CREATE:
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

		case WM_PAINT:
			PAINTSTRUCT ps;
			hdc = BeginPaint(g_hWnd,&ps);

			Ellipse(hdc, x0-20,y0-20,x0+20,y0+20);
			EndPaint(g_hWnd,&ps);
			break;
      default:
            return DefWindowProc(g_hWnd, message, wParam, lParam);
    }
    return 0;
}



OrbitGui::~OrbitGui()
{
}

void OrbitGui::update()
{

}