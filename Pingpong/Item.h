#ifndef ITEM_H
#define ITEM_H



#include "Obj.h"

const double ITEM_FALL_SPEED = 100;
const double ITEM_VOL = 15;

class Item :public Obj {
private:
public:
	Item(location loc,int type);
	void UpdateLocalData();
	void CollisionAObj(CollisionData data);

	virtual std::vector<CollisionData> DetectAllCollision();  //���������Ч�ʣ�����Ҫinclude system.h
};

#endif