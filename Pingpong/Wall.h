#pragma once
#include <typeinfo>
#include "Obj.h"
class Wall :public Obj {
	static std::set<Wall*> _EveryWall;
public:
	static std::set<Wall*> EveryWall();
	Wall(double left, double top, double right, double bottom);
	~Wall();
	void UpdateLocalData();
	void printvisible(HDC hdc,location loc);
};