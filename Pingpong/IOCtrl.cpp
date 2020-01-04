#include "IOCtrl.h"
#include "setting.h"
#include "Interface.h"

double IOCtrl::loc_cvrt::radio;
double IOCtrl::loc_cvrt::xoffset, IOCtrl::loc_cvrt::yoffset;

HWND IOCtrl::_m_hwnd;
int IOCtrl::cxClient, IOCtrl::cyClient;
HDC IOCtrl::_hdc;

void IOCtrl::clear()
{
	PatBlt(_hdc, 0, 0, CXINSIDE, CYINSIDE, WHITENESS);
}

void IOCtrl::Init(HWND hwnd)
{
	HDC m_hdc = GetDC(hwnd);
	_hdc = CreateCompatibleDC(m_hdc);
	HBITMAP hbm = CreateCompatibleBitmap(m_hdc, CXINSIDE, CYINSIDE);
	SelectObject(_hdc, hbm);
	ReleaseDC(hwnd, m_hdc);
	_m_hwnd = hwnd;

	HFONT hfont = CreateFont(0, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, FF_DONTCARE, "ËÎÌו");
	SelectObject(_hdc, hfont);
	HBRUSH hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	SelectObject(_hdc, hbrush);
}

LRESULT CALLBACK IOCtrl::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static bool CursorShow = true;
	switch (msg)
	{
	case WM_LBUTTONDOWN: {
		Interface* top = Interface::UpTop();
		if (top != nullptr && LocalCursorLoc(*top).x > 0) {
			MyMsg msg;
			msg.msg = WM_LBUTTONDOWN;
			msg.Data= std::make_shared<location>(LocalCursorLoc(*top));
			top->SendMsg(msg);
		}
		return 0;
	}
	case WM_SHOWCURSOR: {
		if (CursorShow == false) {
			ShowCursor(true);
			CursorShow = true;
		}
		return 0;
	}
	case WM_HIDECURSOR: {
		if (CursorShow) {
			ShowCursor(false);
			CursorShow =false;
		}
		return 0;
	}
	case WM_KEYDOWN: {
		switch (wparam)
		{
		case VK_ESCAPE:{
			Interface* top = Interface::UpTop();
			MyMsg msg;
			msg.msg = WM_KEYDOWN;
			msg.Data = std::make_shared<int>(VK_ESCAPE);
			top->SendMsg(msg);
			return 0;
		}
		}
		return 0;
	}
	case WM_SIZE: {
		cxClient = LOWORD(lparam);
		cyClient = HIWORD(lparam);
		loc_cvrt::resize();
		return 0;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		IOCtrl::Print(hdc);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd,msg,wparam,lparam);
}

void IOCtrl::MessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void IOCtrl::Print(HDC m_hdc)
{
	bool ishdc0 = (m_hdc == 0);
	if (ishdc0) m_hdc = GetDC(_m_hwnd);
	int ox = floor(loc_cvrt::xoffset + 0.5);
	int oy = floor(loc_cvrt::yoffset + 0.5);
	int ocx = floor(CXINSIDE*loc_cvrt::radio + 0.5);
	int ocy= floor(CYINSIDE*loc_cvrt::radio + 0.5);
	StretchBlt(m_hdc,ox,oy,ocx,ocy,_hdc, 0, 0, CXINSIDE, CYINSIDE, SRCCOPY);
	if (ishdc0) ReleaseDC(_m_hwnd, m_hdc);
}

HDC IOCtrl::hdc()
{
	return _hdc;
}

HWND IOCtrl::m_hwnd()
{
	return _m_hwnd;
}

location IOCtrl::LocalCursorLoc(const Entity & en)
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(_m_hwnd, &point);
	return loc_cvrt::to_local(&en, point);
}

void IOCtrl::QuitGame()
{
	SendMessage(_m_hwnd, WM_DESTROY, 0, 0);
}

location IOCtrl::GetCursorLoc()
{
	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(_m_hwnd, &pos);
	return loc_cvrt::to_inside(pos);
}

void IOCtrl::SetCursorLoc(location loc)
{
	POINT pos = loc_cvrt::to_outside(loc);
	ClientToScreen(_m_hwnd, &pos);
	SetCursorPos(pos.x, pos.y);
}

void IOCtrl::StringOut(location loc, std::string sz)
{
	int x = (int)floor(loc.x+0.5);
	int y = (int)floor(loc.y + 0.5);
	TextOut(_hdc, x, y, sz.c_str(), sz.size());
}



void IOCtrl::loc_cvrt::resize()
{
	double xradio = (double)cxClient / CXINSIDE;
	double yradio = (double)cyClient / CYINSIDE;
	radio = min(xradio, yradio);
	xoffset = (cxClient - CXINSIDE * radio) / 2;
	yoffset = (cyClient - CYINSIDE * radio) / 2;
}

location IOCtrl::loc_cvrt::to_inside(POINT clientpos)
{
	location loc(clientpos.x, clientpos.y);
	loc.x -= xoffset;
	loc.y -= yoffset;
	loc.x /= radio;
	loc.y /= radio;
	if (loc.x >= 0 && loc.x < CXINSIDE && loc.y >= 0 && loc.y < CYINSIDE) {
		return loc;
	}
	return location(-1, -1);
}



location IOCtrl::loc_cvrt::to_inside(const Entity * en, location localloc)
{
	double xplus = 0;
	double yplus = 0;
	while (en!= nullptr) {
		xplus += en->left();
		yplus += en->top();
		en = en->parent();
	}

	localloc.x += xplus;
	localloc.y += yplus;
	return localloc;
}

POINT IOCtrl::loc_cvrt::to_outside(location insideloc)
{
	insideloc.x = insideloc.x*radio + xoffset;
	insideloc.y = insideloc.y*radio + yoffset;
	return {(int)insideloc.x,(int)insideloc.y };
}

POINT IOCtrl::loc_cvrt::to_outside(const Entity * p, location locallocation)
{
	return to_outside(to_inside(p, locallocation));
}

location IOCtrl::loc_cvrt::to_local(const Entity* p, location insideloc)
{
	double xplus = 0;
	double yplus = 0;
	double xmax = p == nullptr ? CXINSIDE : p->width();
	double ymax = p == nullptr ? CYINSIDE : p->height();
	while (p != nullptr){
		xplus += p->left();
		yplus += p->top();
		p = p->parent();
	};

	insideloc.x -= xplus;
	insideloc.y -= yplus;

	if (insideloc.x >= 0 && insideloc.y <xmax && insideloc.y >= 0 && insideloc.y <ymax) {
		return insideloc;
	}
	return location(-1, -1);
}

location IOCtrl::loc_cvrt::to_local(const Entity *p, POINT clientpos)
{
	return to_local(p, to_inside(clientpos));
}


