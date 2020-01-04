#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include "Item.h"
#include "Ball.h"
#include "setting.h"

using namespace std;

Item::Item(location loc,int type) :Obj("Item", shp_round)
{
	SetType(type);
	SetX(loc.x);
	SetY(loc.y);
	SetWidth(ITEM_VOL);
	SetHeight(ITEM_VOL);
	SetSpeed(ITEM_FALL_SPEED/BASEFPS);
	SetSpeedFacing(M_PI/2);
	SetResname("Item");
}

void Item::UpdateLocalData()
{
	if (yCenter() > GAMEWINDOW_LOGICALHEIGHT + 30) DelThis();
	defUpdateLocalData();
	CollisionFunc();
}
#include "System.h"

void Item::CollisionAObj(CollisionData data)
{
	if (data.pObj2->ClassName() == "Racket") {
		switch (type()) {
		case 0:
			data.pObj2->SetType(data.pObj2->type() + 1);
			break;
		case 1: {
			int n = Ball::EveryBall().size();
			for (Ball* p:Ball::EveryBall()){
				if (n <3) {
					Ball* newball = new Ball(nullptr, p->xCenter(), p->yCenter());
					newball->SetType(p->type());
					newball->SetSpeed(p->GetSpeed());
					newball->SetSpeedFacing(p->GetSpeedFacing() + 0.2);
					newball = new Ball(nullptr, p->xCenter(), p->yCenter());
					newball->SetType(p->type());
					newball->SetSpeed(p->GetSpeed());
					newball->SetSpeedFacing(p->GetSpeedFacing() - 0.2);
				}
				else  {
					Ball* newball = new Ball(nullptr, p->xCenter(), p->yCenter());
					newball->SetType(p->type());
					newball->SetSpeed(p->GetSpeed());
					newball->SetSpeedFacing(p->GetSpeedFacing() + 0.15);
					p->SetSpeedFacing(p->GetSpeedFacing() - 0.15);
				}
			}
			break;
		}
		case 2: {
			for (Ball* p : Ball::EveryBall()) {
				if (p->type() == 0) {
					p->SetType(1);
					break;
				}
			}
			break;
		}
		case 3: {
			System::Score += 50;
			break;
		}
		};
		this->DelThis();
	}
}

#include "System.h"
std::vector<CollisionData> Item::DetectAllCollision()
{
	std::vector<CollisionData> ans;
	CollisionData data = GetCollisionData(*System::MainRacket);
	if (data.isCollided == true) ans.push_back(data);
	return ans;
}
