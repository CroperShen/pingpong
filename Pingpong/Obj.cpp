#define _USE_MATH_DEFINES
#include <math.h>

#include "Obj.h"
#include "System.h"
#include "setting.h"
std::set < Obj*,lpObjCmp> Obj::_EveryObj;
Entity* Obj::_GameMainWindow;


_typeGetObjClassOrder GetObjClassOrder;

using namespace std;
void Obj::SetMainWindow(Entity * p)
{
	_GameMainWindow = p;
}
std::set<Obj*, lpObjCmp> Obj::EveryObj()
{
	return _EveryObj;
}
Obj::Obj(const char * lpszClassName, ObjShape shp):Entity(_GameMainWindow),_shp(shp),_ClassName(lpszClassName)
{
	_EveryObj.insert(this);
}

Obj::~Obj(){
	_EveryObj.erase(this);

	MyMsg msg;
	msg.msg = WM_OBJUNLOAD;
	msg.Data=make_shared<string>(ClassName());
	System::GetInterface("GameWindow")->SendMsg(msg);
}

void Obj::CollisionFunc()
{
	std::vector<CollisionData> vec= DetectAllCollision();
	for (auto data : vec) {
		CollisionAObj(data);
	}
}

std::vector<CollisionData> Obj::DetectAllCollision()
{
	std::vector<CollisionData> ans;
	for (auto o : EveryObj()) {
		if (o == this) continue;
		CollisionData data = GetCollisionData(*o);
		if (data.isCollided == true) ans.push_back(data);
	}
	return ans;
}

CollisionData Obj::GetCollisionData_rr(Obj & obj1, Obj & obj2)
{
	CollisionData Data;
	Data.pObj1 = &obj1;
	Data.pObj2 = &obj2;
	double rspdx = obj2.GetSpeedX() - obj1.GetSpeedX();
	double rspdy = obj2.GetSpeedY() - obj1.GetSpeedY();
	if (rspdx*rspdx + rspdy * rspdy < 0.01) {
		Data.isCollided = false;
		return Data;                //速度完全一致，即使接触也不算有碰撞
	}

	double xCross = min(obj1.right(), obj2.right()) - max(obj1.left(), obj2.left());
	double yCross = min(obj1.bottom(), obj2.bottom()) - max(obj1.top(), obj2.top());
	if (xCross <= 0 || yCross <= 0) {
		Data.isCollided = false;     //没有接触的话没有碰撞
		return Data;
	}

	double tx = 1.E10;
	if (rspdx > 0) {
		tx = (obj2.right() - obj1.left()) / rspdx;
	}
	else if (rspdx < 0) {
		tx = (obj2.left() - obj1.right()) / rspdx;
	}
	double ty = 1.E10;
	if (rspdy > 0) {
		ty = (obj2.bottom() - obj1.top()) / rspdy;
	}
	else if (rspdy < 0) {
		ty = (obj2.top() - obj1.bottom()) / rspdy;
	}

	double t = min(tx, ty);
	if (tx < ty) {
		if (rspdx > 0) {
			Data.border2 = brd_right;
			Data.border1 = brd_left;
		}
		else{
			Data.border2 = brd_left;
			Data.border1 = brd_right;
		}
	}
	else {
		if (rspdy > 0) {
			Data.border2 = brd_bottom;
			Data.border1 = brd_top;
		}
		else {
			Data.border2 = brd_top;
			Data.border1 = brd_bottom;
		}
	}
	Data.isCollided = true;
	Data.loc1 = { obj1.xCenter() - obj1.GetSpeedX()*t,obj1.yCenter() - obj1.GetSpeedY()*t };
	Data.loc2=  { obj2.xCenter() - obj1.GetSpeedX()*t,obj2.yCenter() - obj2.GetSpeedY()*t };
	Data.RAngle = atan2(Data.loc2.y - Data.loc1.y, Data.loc2.x - Data.loc1.x);
	return Data;
}

CollisionData Obj::GetCollisionData_ro(Obj & obj1, Obj & obj2)
{
	CollisionData Data;
	Data.pObj1 = &obj1;
	Data.pObj2 = &obj2;
	double rspdx = obj2.GetSpeedX() - obj1.GetSpeedX();
	double rspdy = obj2.GetSpeedY() - obj1.GetSpeedY();
	if (rspdx*rspdx + rspdy * rspdy < 0.01) {
		Data.isCollided = false;
		return Data;                //速度完全一致，即使接触也不算有碰撞
	}

	double xCross = min(obj1.right(), obj2.right()) - max(obj1.left(), obj2.left());
	double yCross = min(obj1.bottom(), obj2.bottom()) - max(obj1.top(), obj2.top());
	if (xCross <= 0 || yCross <= 0) {
		Data.isCollided = false;     //没有接触的话没有碰撞
		return Data;
	}

	if (obj2.right() <= obj1.left() || obj2.left() >= obj1.right() || obj2.bottom() < obj1.top() || obj2.top() >= obj1.bottom()) {
		Data.isCollided = false;
		return Data;
	}
	bool atCorner = true;
	if (obj2.xCenter() <= obj1.right() && obj2.xCenter() >= obj1.left() || obj2.yCenter() >= obj1.top() && obj2.yCenter() <= obj1.bottom()) atCorner = false;
	if (atCorner) {
		double xCorner;
		double yCorner;
		if (obj2.xCenter() > obj1.right()) xCorner = obj1.right();
		else xCorner = obj1.left();
		if (obj2.yCenter() > obj1.bottom()) yCorner = obj1.bottom();
		else yCorner = obj1.top();
		double l = pow(obj2.xCenter() - xCorner, 2) + pow(obj2.yCenter() - yCorner, 2);
		if (pow(obj2.width() / 2, 2) <= l) {
			Data.isCollided = false;
			return Data;
		}
	}
	//稍后完成：矩形和圆的碰撞检测
	return Data;
}



Entity * Obj::GameWindow()
{
	return _GameMainWindow;
}

CollisionData Obj::GetCollisionData(Obj &obj2)
{
	Obj& obj1 = *this;
	if (obj1.Shape() == shp_rectangle && obj2.Shape() == shp_rectangle) {
		return GetCollisionData_rr(obj1, obj2);
	}
	if (obj1.Shape() == shp_rectangle && obj2.Shape() == shp_round) {
		//return GetCollisionData_ro(obj1, obj2);
		//函数未完成，以以下函数代替
		return GetCollisionData_rr(obj1, obj2);
	}
	if (obj1.Shape() == shp_round && obj2.Shape() == shp_rectangle) {
		//return GetCollisionData_ro(obj2, obj1).swap();
		//函数未完成，以以下函数代替
		return GetCollisionData_rr(obj1, obj2);
	}
	if (obj1.Shape() == shp_round && obj2.Shape() == shp_rectangle) {
		//return GetCollisionData_ro(obj2, obj1);
		//函数未完成，以以下函数代替
		return GetCollisionData_rr(obj1, obj2);
	}
	return CollisionData();
}

void Obj::CollisionAObj(CollisionData data)
{
	//do nothing;
}

bool lpObjCmp::operator()(Obj* p1,Obj* p2) const
{
	if (p1 == NULL || p2 == NULL) return false;
	int t1 = GetObjClassOrder(p1->ClassName());
	int t2 = GetObjClassOrder(p1->ClassName());
	if (t1 == t2) {
		return ( p1 < p2);
	}
	return (t1 <t2);
}

const char * Obj::ClassName() const
{
	return _ClassName;
}

ObjShape Obj::Shape() const
{
	return _shp;
}


CollisionData CollisionData::swap() const
{
	CollisionData data2;
    
	data2.isCollided = isCollided;
	data2.pObj1 = pObj2;
	data2.pObj2 = pObj1;
	data2.loc1 = loc2;
	data2.loc2 = loc1;
	data2.RAngle = RAngle + M_PI;
	data2.border1 = border2;
	data2.border2 = border1;
	return data2;
}

void _typeGetObjClassOrder::SetOrder(const char * lpszClassName, int iClassOrder)
{
	order[lpszClassName] = iClassOrder;
}

unsigned int _typeGetObjClassOrder::operator()(const char * lpszClassName)
{
	auto iter=order.find(lpszClassName);
	if (iter == order.end()) return 0xFFFFFFFF;
	return iter->second;
}
