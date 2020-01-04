#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include "setting.h"
#include "Ball.h"
#include "Racket.h"
#include "interface.h"
#include "System.h"
#include "Effect.h"
#include "Brick.h"

using namespace std;

set<Ball*> Ball::_EveryBall;


set<Ball*> Ball::EveryBall()
{
	return _EveryBall;
}

Ball::Ball(Racket * R, double x,double y) :Obj("Ball", shp_round)
{
	if (_EveryBall.size() >= BALL_NUM_LIMIT) {
		DelThis();
		return;
	}
	SetX(x);
	SetY(y);
	SetWidth(BALLBASEVOL);
	SetHeight(BALLBASEVOL);
	SetSpeed(0);
	SetSpeedFacing(0.6);
	SetResname("Ball");
	_racket = R;

	if (R != NULL) {
		SetRacket(R);
	}

	_EveryBall.insert(this);
}

Ball::~Ball()
{
	_EveryBall.erase(this);
}

void Ball::SetRacket(Racket* R)
{
	_racket = R;
	if (R == NULL) return;
	_RacketX = xCenter() - R->xCenter();
	if (_RacketX < -R->width() / 2) _RacketX = -R->width() / 2;
	if (_RacketX > R->width() / 2) _RacketX = R->width() / 2;
	SetX(R->xCenter() + _RacketX);
	SetBottom(R->top());
}



void Ball::UpdateLocalData()
{
	if (yCenter() > GAMEWINDOW_LOGICALHEIGHT + 30) {
		DelThis();
		return;
	}

	if (_racket != NULL && _racket->TeeOff()==true){
		SetSpeed(BALLBASESPEED/BASEFPS);
		SetSpeedFacing(-0.6);
		_racket = NULL;
	}

	if (_racket != NULL) {
		SetX(_racket->xCenter() + _RacketX);
		return;
	}

	if (type() == 1) {
		location loc = IOCtrl::loc_cvrt::to_inside(Obj::GameWindow(), Center());
		Effect::CreateEffect(loc.x, loc.y , "Spark1");
	}
	
	defUpdateLocalData();
	CollisionFunc();
}


void Ball::CollisionAObj(CollisionData data)
{
	const char* O2name = data.pObj2->ClassName();
	if (O2name =="Wall" || O2name == "Racket" || O2name=="Brick") {
		SetX(data.loc1.x);
		SetY(data.loc1.y);
		switch (data.border2) {
		case brd_left:
			SetSpeedX(-abs(GetSpeedX()));
			break;
		case brd_right:
			SetSpeedX(abs(GetSpeedX()));
			break;
		case brd_top:
			SetSpeedY(-abs(GetSpeedY()));
			break;
		case brd_bottom:
			SetSpeedY(abs(GetSpeedY()));
			break;
		}
	}

	if (O2name == "Brick") {
		location loc = IOCtrl::loc_cvrt::to_inside(Obj::GameWindow(), Center());
		if (type() == 1) {
			for (Brick* b:Brick::EveryBrick()) {
				if (b == data.pObj2) continue;
				double r = Center().GetDistance(b->Center());
				if (r <= 23) {
					CollisionData data2;
					data2.pObj2 = this;
					b->CollisionAObj(data2);
				}
			}
			Effect::CreateEffect(loc.x, loc.y, "Explosion1");
		}
		else {
			Effect::CreateEffect(loc.x, loc.y, "little Explosion");
		}
	}

	if(O2name == "Racket") {
		double xspeed = GetSpeedX();
		double yspeed = GetSpeedY();
		xspeed = xspeed + FRICTION_FACTOR * (data.pObj2->GetSpeedX());
		//Speed = sqrt(xspeed*xspeed + yspeed * yspeed);
		double facing = atan2(yspeed, xspeed);
		if (facing > 0 && facing < M_PI / 6) facing = M_PI / 6;
		if (facing < 0 && facing > -M_PI / 6) facing = -M_PI / 6;
		if (facing > 5 * M_PI / 6) facing = 5 * M_PI / 6;
		if (facing < -5 * M_PI / 6) facing = -5 * M_PI / 6;
		SetSpeedFacing(facing);
	}

	if (O2name == "Brick") {
		data.pObj2->CollisionAObj(data.swap());
	}
}

void Ball::SetType(int i)
{
	if (i == 1 && type()==0) SetSpeed(GetSpeed()*1.3);
	Entity::SetType(i);
}

#ifndef DEBUGMODE

#include "System.h"
#include "Brick.h"
#include "Wall.h"

std::vector<CollisionData> Ball::DetectAllCollision()
{
	std::vector<CollisionData> ans;

	CollisionData data0;
	data0.isCollided = false;
	double d0 = 0;

	for (auto o : Brick::EveryBrick()) {
		CollisionData data = GetCollisionData(*o);
		if (data.isCollided == true) {
			double d = Center().GetDistance(data.loc1);
			if (d > d0) {
				data0 = data;
				d = d0;
			}
		}
	}

	if (data0.isCollided) ans.push_back(data0);
 
	for (auto o : Wall::EveryWall()) {
		CollisionData data = GetCollisionData(*o);
		if (data.isCollided == true) ans.push_back(data);
	}
	CollisionData data = GetCollisionData(*System::MainRacket);
	if (data.isCollided == true) ans.push_back(data);
	return ans;
}

#endif
