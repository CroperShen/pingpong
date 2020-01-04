#pragma once
#ifndef RACKET_H
#define RACKET_H

#include "Obj.h"

const double RACKETBASEWIDTH = 40;
const double RACKETBASEHEIGHT = 6;
 
class Racket :public Obj {
	int _iTeeOff; //·¢Çò×´Ì¬
public:
	bool TeeOff() const;
	void SetTeeOff();

	Racket(double x, double y);
	void UpdateLocalData();
	void SetType(int type);
	void CollisionAObj(CollisionData data);
};

#endif