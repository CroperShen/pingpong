#define _USE_MATH_DEFINES
#include <math.h>
#include <stack>
#include "Entity.h"
#include "IOCtrl.h"
#include "ResManager.h"

#define DEBUGMODE
#ifdef DEBUGMODE
#include "System.h"
#endif

using namespace std;


Entity::Entity(Entity* parent) :_parent(parent)
{
	if (_parent != NULL) {
		_parent->_children.insert(this);
	}
	_x = _y = _cx = _cy = 0;
	_spd = _spdf = 0;
	_visible = true;
	_del = false;
	using_predictfunc = true;
}

Entity::~Entity()
{

	for (Entity* p:children()) {
		delete p;
	}
	if (_parent!=nullptr) _parent->_children.erase(this);
}

double Entity::left() const
{
	return _x - _cx / 2;
}

double Entity::right() const
{
	return _x + _cx / 2;
}

double Entity::top() const
{
	return _y - _cy / 2;
}

double Entity::bottom() const
{
	return _y + _cy / 2;
}

double Entity::xCenter() const
{
	return _x;
}

double Entity::yCenter() const
{
	return _y;
}

location Entity::Center() const
{
	return location(_x, _y);
}

double Entity::width() const
{
	return _cx;
}

double Entity::height() const
{
	return _cy;
}

int Entity::type() const
{
	return _type;
}

string Entity::resname() const
{
	return _resname;
}

double Entity::GetSpeed() const
{
	return _spd;
}

double Entity::GetSpeedX() const
{
	return _spd * cos(_spdf);
}

double Entity::GetSpeedY() const
{
	return _spd * sin(_spdf);
}

double Entity::GetSpeedFacing() const
{
	return _spdf;
}

void Entity::DelThis()
{
	for (auto en :_children) {
		en->DelThis();
	}
	_del = true;
}

bool Entity::NoMove() const
{
	return _spd < 0.001;
}

bool Entity::visible() const
{
	return _visible;
}

void Entity::SetX(double x)
{
	_x = x;
}

void Entity::SetY(double y)
{
	_y = y;
}

void Entity::SetXY(double x, double y)
{
	_x = x;
	_y = y;
}

void Entity::SetLoc(location loc)
{
	_x = loc.x;
	_y = loc.y;
}

void Entity::SetLeft(double left, bool changewidth)
{
	if (changewidth) {
		double right = _x + _cx / 2;
		_x = (left + right) / 2;
		_cx = right - left;
	}
	else {
		_x = left + _cx / 2;
	}
}

void Entity::SetRight(double right, bool changewidth)
{
	if (changewidth) {
		double left = _x - _cx / 2;
		_x = (left + right) / 2;
		_cx = right - left;
	}
	else {
		_x = right - _cx / 2;
	}
}

void Entity::SetTop(double top, bool changeheight)
{
	if (changeheight) {
		double bottom = _y + _cy / 2;
		_y = (top + bottom) / 2;
		_cy = bottom - top;
	}
	else {
		_y = top + _cy / 2;
	}
}

void Entity::SetBottom(double bottom, bool changeheight)
{
	if (changeheight) {
		double top = _y - _cy / 2;
		_y = (top + bottom) / 2;
		_cy = bottom - top;
	} {
		_y = bottom - _cy / 2;
	}
}

void Entity::SetWidth(double width)
{
	_cx = width;
}

void Entity::SetHeight(double height)
{
	_cy = height;
}

void Entity::SetSpeed(double speed)
{
	if (speed < 0) {
		_spd = -speed;
		_spdf = -_spdf;
	}
	else {
		_spd = speed;
	}
}

void Entity::SetSpeedX(double speedx)
{
	double speedy = GetSpeedY();
	_spd = sqrt(pow(speedx, 2) + pow(speedy, 2));
	if (_spd != 0.){
		_spdf = atan2(speedy, speedx);
	}
}

void Entity::SetSpeedY(double speedy)
{
	double speedx = GetSpeedX();
	_spd = sqrt(pow(speedx, 2) + pow(speedy, 2));
	if (_spd != 0.) {
		_spdf = atan2(speedy, speedx);
	}
}

void Entity::SetSpeedXY(double speedx, double speedy)
{
	_spd = sqrt(pow(speedx, 2) + pow(speedy, 2));
	if (_spd != 0.) {
		_spdf = atan2(speedy, speedx);
	}
}

void Entity::SetSpeedFacing(double facing)
{
	_spdf = facing;
}

void Entity::SetVisible(bool visible)
{
	_visible = visible;
}

void Entity::SetType(int type)
{
	_type = type;
}

void Entity::SetResname(string resname)
{
	_resname = resname;
}

void Entity::print(HDC hdc, double fracframe)
{
	if (_visible) {
		location loc;
		loc.x = width() / 2;
		loc.y = height()/2 ;
		if (using_predictfunc) {
			loc.x += fracframe * GetSpeedX();
			loc.y += fracframe * GetSpeedY();
		}
		loc = IOCtrl::loc_cvrt::to_inside(this, loc);
		printvisible(hdc, loc);
		for (Entity* en : children()) {
			en->print(hdc, fracframe);
		}
	}
}

void Entity::printvisible(HDC hdc, location loc)
{
	Res* res = ResManager::GetRes(_resname);
	if (res == nullptr) return;
	res->print(loc,_type,RA_ALLCENTER);
}

void Entity::UpdateData()
{
	if (_del == true) {
		delete this;
		return;
	}
	UpdateLocalData();
	for (Entity* en : children()) {
		en->UpdateData();
	}
}

void Entity::defUpdateLocalData()
{
	if (NoMove()) return;
	SetX(GetSpeedX()+xCenter());
	SetY(GetSpeedY()+yCenter());
}

void Entity::UpdateLocalData()
{
	//do nothing;
}

 

Entity * Entity::parent() const
{
	return _parent;
}

set<Entity*> Entity::children()
{
	return _children;
}

void Entity::PrintText(location localloc, string sz)
{
	location inloc = IOCtrl::loc_cvrt::to_inside(this, localloc);
	IOCtrl::StringOut(inloc, sz);
}

void Entity::PrintFrame(location loc,double extend)
{
	int x1 = (int)floor(loc.x-_cx/2-extend + 0.5);
	int y1 = (int)floor(loc.y-_cy/2-extend + 0.5);
	int x2 = (int)floor(loc.x+_cx/2+extend + 0.5);
	int y2 = (int)floor(loc.y+_cy/2 +extend + 0.5);
	Rectangle(IOCtrl::hdc(), x1, y1, x2, y2);
}
