#include "Brick.h"
#include <string>
#include <random>
#include "setting.h"
#include "item.h"
#include "MyMessage.h"
#include "interface.h"
using namespace std;

set<Brick*> Brick::_EveryBrick;

std::set<Brick*> Brick::EveryBrick()
{
	return _EveryBrick;
}

Brick::Brick(double x,double y,int type):Obj("Brick",shp_rectangle)
{
	SetX(x);
	SetY(y);
	SetWidth(BRICKBASEWIDTH);
	SetHeight(BRICKBASEHEIGHT);
	SetResname("Brick");
	_EveryBrick.insert(this);
	SetType(type);

	if (type < 12) _hp = 1;
	else _hp = 20;
}

Brick::~Brick()
{
	_EveryBrick.erase(this);
}

void Brick::UpdateLocalData()
{
}

void Brick::CollisionAObj(CollisionData data)
{
	if (data.pObj2->ClassName() == "Ball") {
		_hp--;
		if (_hp<=0)
			DeathFunc();
		MyMsg msg;
		msg.Data = make_shared<int>(type());
		msg.msg = WM_BRICKHIT;
		((Interface*)Obj::GameWindow())->SendMsg(msg);
	}
}

void Brick::DeathFunc()
{
	if (type() < 6) {
		if (rand() % 100 < 120) {
			Item *item = new Item(Center(), rand() % 4);
		}
	}
	else if (type() < 12) {
		Brick* b = new Brick(xCenter(), yCenter(), type() - 6);
	}
	
	DelThis();
}


