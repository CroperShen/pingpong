#ifndef IOCTRL_H
#define IOCTRL_H

#include <windows.h>
#include "location.h"
#include "Entity.h"

class IOCtrl {
private:
	static HWND _m_hwnd;
	static int cxClient, cyClient;
	static HDC _hdc;

public:
	static void Print(HDC hdc = 0);

	struct loc_cvrt{
		friend  void IOCtrl::Print(HDC);
	private:
		static double radio;
		static double xoffset, yoffset;
	public:
		static void resize();
		static location to_inside(POINT clientpos);
		static location to_inside(const Entity* p,location locallocation);
		static POINT to_outside(location insideloc);
		static POINT to_outside(const Entity*p, location locallocation);
		static location to_local(const Entity* p, location insideloc);
		static location to_local(const Entity* p, POINT clientpos);
	};

	static void clear();
	static void Init(HWND hwnd);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static void MessageLoop();
	

	static HDC hdc();
	static HWND m_hwnd();
	static location LocalCursorLoc(const Entity& en);

	static void QuitGame();
	static location GetCursorLoc();
	static void SetCursorLoc(location loc);
	static void StringOut(location loc, std::string sz);
};

#endif