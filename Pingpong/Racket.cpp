#include <string>
#include "Racket.h"
#include "setting.h"

using namespace std;

bool Racket::TeeOff() const
{
	return _iTeeOff > 0;
}

void Racket::SetTeeOff()
{
	_iTeeOff = 2;
}

Racket::Racket(double x,double y) :Obj("Racket", shp_rectangle)
{
	SetX(x);
	SetY(y);
	SetWidth(RACKETBASEWIDTH);
	SetHeight(RACKETBASEHEIGHT);
	SetResname("Racket");
	_iTeeOff = 0;
}

void Racket::UpdateLocalData()
{
	if (_iTeeOff > 0) --_iTeeOff;
	static int TeeOff_tick = 0;
	defUpdateLocalData();
	CollisionFunc();
}

void Racket::SetType(int type)
{
	if (type > 5) return;
	Entity::SetType(type);

	SetWidth(40 + type * 16);
	if (left() < 0) SetLeft(0);
	if (right() >= GAMEWINDOW_LOGICALWIDTH) SetRight(GAMEWINDOW_LOGICALWIDTH - 1);
}

void Racket::CollisionAObj(CollisionData data)
{
	if (data.pObj2->ClassName()!="Wall") return;
	SetX(data.loc1.x);
	SetSpeed(0);
}

