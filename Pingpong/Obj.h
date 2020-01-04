//Obj.h
//实体
//游戏中的实体，只会在主界面下存在，（能够）移动，（可能）发生碰撞；

#ifndef OBJ_H
#define OBJ_H

#include <set>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include "location.h"
#include "Entity.h"

enum ObjShape{shp_round,shp_rectangle};
enum border{brd_top,brd_right,brd_bottom,brd_left};
//Object形状：shp_round为圆形 shp_rectangle为方形

//重载less以排序
class Obj;
struct lpObjCmp {
	bool operator()(Obj* p1, Obj* p2) const;
};


struct CollisionData {               //碰撞记录；
	bool isCollided;				 //是否发生碰撞；
	Obj* pObj1, *pObj2;              //碰撞物体；
	location loc1, loc2;             //碰撞时位置；
	double RAngle;                   //碰撞时的相对角度，
	border border1;                  //碰撞发生在Obj1的哪条边上
	border border2;                  //碰撞发生在Obj2的哪条边上

	CollisionData swap() const;
};

class Obj:public Entity {
private:
	const ObjShape _shp;     //形状
	const char* _ClassName;  //类型名称

	static std::set<Obj*, lpObjCmp> _EveryObj;   //存储所有Obj
	static Entity* _GameMainWindow;
public:
	static void SetMainWindow(Entity* p);
	static std::set<Obj*, lpObjCmp> EveryObj();
    //=========================================
	//构造和析构函数

	Obj(const char* lpszClassName,ObjShape shp);
	Obj(const Obj&) = delete;
	virtual ~Obj();
	//======================================
	//获得属性

	const char* ClassName() const;
	ObjShape Shape() const;
	//设置属性
	//========================================
//碰撞相关
private:
	virtual std::vector<CollisionData> DetectAllCollision();   
	static CollisionData GetCollisionData_rr(Obj& obj1, Obj& obj2);  
	static CollisionData GetCollisionData_ro(Obj& obj1, Obj& obj2);
	static CollisionData GetCollisionData_oo(Obj& obj1, Obj& obj2);
public:
	static Entity* GameWindow();
	CollisionData GetCollisionData(Obj& Obj2);
	virtual void CollisionAObj(CollisionData data);       
	void CollisionFunc();                 //检测与所有物体的碰撞并执行相应的碰撞函数
};

struct _typeGetObjClassOrder{
private:
	std::unordered_map<const char*, unsigned int> order;
public:
	void SetOrder(const char* lpszClassName, int iClassOrder);
	unsigned int operator()(const char* lpszClassName);
};
#endif