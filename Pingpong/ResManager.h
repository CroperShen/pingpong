#ifndef RESMANAGER_H
#define RESMANAGER_H

#include <string>
#include <unordered_map>
#include <windows.h>
#include "location.h"

enum ResAlign{
	RA_DEFAULT=0,
	RA_LEFT=0,RA_CENTER=1,RA_RIGHT=2,      //水平对齐方式
	RA_TOP=0,RA_BASELINE=4,RA_BOTTOM=8,	   //垂直对齐方式
	RA_HSCALE_FALSE=0,RA_HSCALE_TRUE=16,   //水平是否缩放
	RA_VSCALE_FALSE=0,RA_VSCALE_TRUE=32,   //垂直是否缩放
	RA_ALLCENTER=5,
};

class Res {
	friend class ResManager;
private:
	static HDC _m_hdc;
	HDC _hdcmem;
	int _resheight, _reswidth;

	void print(int x, int y, int type = 0, ResAlign Align = (ResAlign)0, int cx = -1, int cy = -1);
public:
	const std::string path;

	Res(std::string path0,int reswidth,int resheight);
	Res(const Res&) = delete;


	void print(location loc, int type = 0, ResAlign Align = (ResAlign)0, double cx = -1., double cy = -1.);
};

class ResManager {
private:
	static std::unordered_map<std::string,Res*> _resmap;
public:
	static void Init(HDC hdc);
	static bool LoadResource(std::string respath, std::string resname,int reswidth,int resheight);
	static Res* GetRes(std::string name);
};

#endif