#pragma comment(lib,"msimg32.lib")
#include "ResManager.h"
using namespace std;

HDC Res::_m_hdc;
unordered_map<string, Res*> ResManager::_resmap;
Res::Res(string path0,int reswidth,int resheight):path(path0)
{
	HBITMAP hbitmap = (HBITMAP)LoadImage(nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	_hdcmem = CreateCompatibleDC(_m_hdc);
	SelectObject(_hdcmem, hbitmap);
	_reswidth = reswidth;
	_resheight = resheight;
}

void Res::print(int x, int y, int type, ResAlign Align,int cx,int cy)
{
	const UINT TransparentColor = RGB(255, 0, 255);
	if (cx == -1 || (Align & (RA_HSCALE_FALSE | RA_HSCALE_TRUE)) == RA_HSCALE_FALSE) cx = _reswidth;
	if (cy == -1 || (Align & (RA_VSCALE_FALSE | RA_VSCALE_TRUE)) == RA_VSCALE_FALSE) cy = _resheight;

	switch (Align & (RA_LEFT | RA_CENTER | RA_RIGHT)) {
	case RA_CENTER:
		x -= cx / 2;
		break;
	case RA_RIGHT:
		x -= cx;
	}

	switch (Align & (RA_TOP | RA_BASELINE | RA_BOTTOM)) {
	case RA_BASELINE:
		y -= cy / 2;
		break;
	case RA_BOTTOM:
		y -= cy;
	}

	TransparentBlt(_m_hdc, x, y,cx,cy, _hdcmem, 0, type*_resheight,_reswidth, _resheight, TransparentColor);
}

void Res::print(location loc, int type, ResAlign Align, double cx, double cy)
{
	int x1 = (int)floor(loc.x + 0.5);
	int y1 = (int)floor(loc.y + 0.5);
	int cx1 = (int)floor(cx + 0.5);
	int cy1 = (int)floor(cy + 0.5);
	print(x1, y1, type, Align, cx1, cy1);
}

void ResManager::Init(HDC hdc)
{
	Res::_m_hdc = hdc;
}

bool ResManager::LoadResource(std::string respath, std::string resname, int reswidth, int resheight)
{
	auto p = _resmap[resname];
	if (_resmap[resname] != nullptr) return false;
	_resmap[resname] = new Res(respath,reswidth,resheight);
	return true;
}

Res * ResManager::GetRes(string name)
{
	return _resmap[name];
}
