//Ball.h
//����࣬�̳���Obj

#ifndef BALL_H
#define BALL_H

#include "Obj.h"
extern class Racket;

const double BALLBASEVOL = 7;
const double BALLBASESPEED = 150;
const double FRICTION_FACTOR = 0.3;

const int BALL_NUM_LIMIT = 64;

class Ball :public Obj {
private:
	static std::set<Ball*> _EveryBall;
	Racket* _racket;
	double _RacketX;
public:
	static std::set<Ball*> EveryBall();

	Ball(Racket* baseracket,double x,double y);
	~Ball();
	void SetRacket(Racket*);
	void UpdateLocalData();
	void CollisionAObj(CollisionData data);

	void SetType(int i);

	virtual std::vector<CollisionData> DetectAllCollision();  //���������Ч�ʣ�����Ҫinclude system.h�� brick.h
};

#endif

