#pragma once
#include "Obj.h"

const double BRICKBASEWIDTH = 20;
const double BRICKBASEHEIGHT = 8;

class Brick :public Obj {
private:
	static std::set<Brick*> _EveryBrick;
	double _hp;
public:
	static std::set<Brick*> EveryBrick();
	Brick(double x, double y,int hp);
	~Brick();
	void UpdateLocalData();
	void CollisionAObj(CollisionData data);
	void DeathFunc();
};