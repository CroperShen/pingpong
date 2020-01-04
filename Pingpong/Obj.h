//Obj.h
//ʵ��
//��Ϸ�е�ʵ�壬ֻ�����������´��ڣ����ܹ����ƶ��������ܣ�������ײ��

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
//Object��״��shp_roundΪԲ�� shp_rectangleΪ����

//����less������
class Obj;
struct lpObjCmp {
	bool operator()(Obj* p1, Obj* p2) const;
};


struct CollisionData {               //��ײ��¼��
	bool isCollided;				 //�Ƿ�����ײ��
	Obj* pObj1, *pObj2;              //��ײ���壻
	location loc1, loc2;             //��ײʱλ�ã�
	double RAngle;                   //��ײʱ����ԽǶȣ�
	border border1;                  //��ײ������Obj1����������
	border border2;                  //��ײ������Obj2����������

	CollisionData swap() const;
};

class Obj:public Entity {
private:
	const ObjShape _shp;     //��״
	const char* _ClassName;  //��������

	static std::set<Obj*, lpObjCmp> _EveryObj;   //�洢����Obj
	static Entity* _GameMainWindow;
public:
	static void SetMainWindow(Entity* p);
	static std::set<Obj*, lpObjCmp> EveryObj();
    //=========================================
	//�������������

	Obj(const char* lpszClassName,ObjShape shp);
	Obj(const Obj&) = delete;
	virtual ~Obj();
	//======================================
	//�������

	const char* ClassName() const;
	ObjShape Shape() const;
	//��������
	//========================================
//��ײ���
private:
	virtual std::vector<CollisionData> DetectAllCollision();   
	static CollisionData GetCollisionData_rr(Obj& obj1, Obj& obj2);  
	static CollisionData GetCollisionData_ro(Obj& obj1, Obj& obj2);
	static CollisionData GetCollisionData_oo(Obj& obj1, Obj& obj2);
public:
	static Entity* GameWindow();
	CollisionData GetCollisionData(Obj& Obj2);
	virtual void CollisionAObj(CollisionData data);       
	void CollisionFunc();                 //����������������ײ��ִ����Ӧ����ײ����
};

struct _typeGetObjClassOrder{
private:
	std::unordered_map<const char*, unsigned int> order;
public:
	void SetOrder(const char* lpszClassName, int iClassOrder);
	unsigned int operator()(const char* lpszClassName);
};
#endif