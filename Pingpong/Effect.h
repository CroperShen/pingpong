#ifndef _EFFECT_H
#define _EFFECT_H

#include <set>
#include <string>
#include <unordered_map>

#include "IOCtrl.h" 
#include "location.h"
#include "ResManager.h"


const int EFFECT_MAX_NUM = 1024;

class Effect;

typedef void (*EffectInitFunc)(Effect*);

class EffectType {
	friend class Effect;
private:
	Res *_res;
	int _reslowbound, _reshighbound;
	double _defanmspd;
	DWORD _timelimit;
	EffectInitFunc _InitFunc;

	static std::unordered_map<std::string, EffectType*> _map;

	EffectType(std::string resname, int resupbound, int reslowbound, DWORD timelimit,double defanmspd = 1.0, EffectInitFunc InitFunc = nullptr);
	EffectType(const EffectType&) = delete;
	EffectType(const EffectType&&) = delete;
public:

	void SetAnmationSpeed(double anmspd);
	void SetInitFunc(EffectInitFunc _InitFunc);

	static EffectType* GetEffectTypeByName(std::string tname);
	static EffectType* RegisterEffect(std::string name, std::string resname, int reslowbound,int reshighbound,DWORD timelimit,double defanmspd = 1.0, EffectInitFunc InitFunc = nullptr);
};


class Effect{
private:
	DWORD _ExistTime;
	double _x, _y;
	double _spdx, _spdy;
	double _accx, _accy;
	double _anmspd;
	EffectType *_type;

	static std::set<Effect*> _EveryEffect;

	Effect(double x,double y, std::string type);
	~Effect();
public:


	double GetX() const;
	double GetY() const;
	location Getloc() const;

	double speed() const;
	double speedX() const;
	double speedY() const;
	double speedfacing() const;

	double acceleration() const;
	double accelerationX() const;
	double accelerationY() const;
	double accelerationfacing() const;

	void SetX(double x);
	void SetY(double y);
	void SetLoc(location loc);

	void SetSpeed(double spd);
	void SetSpeedX(double spdx);
	void SetSpeedY(double spdy);
	void SetSpeedFacing(double spdfacing);

	void SetAcceleration(double a);
	void SetAccelerationX(double ax);
	void SetAccelerationY(double ay);
	void SetAccelerationFacing(double af);

	void print(DWORD timedelta);

	static std::set<Effect*> EveryEffect();
	static void ShowEffect();
	static Effect* CreateEffect(double x, double y, std::string type);
};

#endif;