#include "Wall.h"

using namespace std;
set<Wall*> Wall::_EveryWall;

std::set<Wall*> Wall::EveryWall()
{
	return _EveryWall;
}

Wall::Wall(double left,double top,double right,double bottom):Obj("Wall",shp_rectangle)
{
	SetLeft(left, true);
	SetTop(top, true);
	SetRight(right, true);
	SetBottom(bottom, true);
	_EveryWall.insert(this);
}

Wall::~Wall()
{
	_EveryWall.erase(this);
}

void Wall::UpdateLocalData()
{
	//do nothing;
}

void Wall::printvisible(HDC hdc, location loc)
{
	//do nothing;
}
