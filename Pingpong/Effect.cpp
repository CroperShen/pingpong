#include "Effect.h"
using namespace std;

unordered_map<std::string, EffectType*> EffectType::_map;
set<Effect*> Effect::_EveryEffect;
EffectType::EffectType(std::string resname, int reshighbound, int reslowbound, DWORD timelimit, double defanmspd, EffectInitFunc InitFunc)
{
	_res = ResManager::GetRes(resname);
	_reslowbound = reslowbound;
	_reshighbound = reshighbound;
	_timelimit = timelimit;
	_defanmspd = defanmspd;
	_InitFunc = InitFunc;
}

void EffectType::SetAnmationSpeed(double anmspd)
{
	_defanmspd;
}

void EffectType::SetInitFunc(EffectInitFunc _InitFunc)
{
	_InitFunc;
}

EffectType * EffectType::GetEffectTypeByName(std::string tname)
{
	return _map[tname];
}

EffectType* EffectType::RegisterEffect(std::string name, std::string resname, int reslowbound, int reshighbound,DWORD timelimit,double defanmspd, EffectInitFunc InitFunc)
{
	EffectType* e = new EffectType(resname, reshighbound, reslowbound,timelimit, defanmspd, InitFunc);
	_map[name] = e;
	return e;
}

Effect::Effect(double x, double y, std::string type)
{
	_x = x;
	_y = y;
	_spdx = 0;
	_spdy = 0;
	_accx = 0;
	_accy = 0;
	_anmspd = 1;
	_ExistTime = 0;
	_type = EffectType::GetEffectTypeByName(type);
	_type->_InitFunc(this);
	_EveryEffect.insert(this);
}

Effect::~Effect()
{
	_EveryEffect.erase(this);
}

double Effect::GetX() const
{
	return _x;
}

double Effect::GetY() const
{
	return _y;
}

location Effect::Getloc() const
{
	return location(_x, _y);
}

double Effect::speed() const
{
	return sqrt(pow(_spdx, 2) + pow(_spdy, 2));
}

double Effect::speedX() const
{
	return _spdx;
}

double Effect::speedY() const
{
	return _spdy;
}

double Effect::speedfacing() const
{
	return atan2(_spdy, _spdx);
}

double Effect::acceleration() const
{
	return sqrt(pow(_accx, 2) + pow(_accy, 2));
}

double Effect::accelerationX() const
{
	return _accx;
}

double Effect::accelerationY() const
{
	return _accy;
}

double Effect::accelerationfacing() const
{
	return atan2(_accy, _accx);
}

void Effect::SetX(double x)
{
	_x = x;
}

void Effect::SetY(double y)
{
	_y = y;
}

void Effect::SetLoc(location loc)
{
	_x = loc.x;
	_y = loc.y;
}

void Effect::SetSpeed(double spd)
{
	double spdf = speedfacing();
	_spdx = spd * cos(spdf);
	_spdy = spd * sin(spdf);
}

void Effect::SetSpeedX(double spdx)
{
	_spdx = spdx;
}

void Effect::SetSpeedY(double spdy)
{
	_spdy = spdy;
}

void Effect::SetSpeedFacing(double spdfacing)
{
	double spd = speed();
	_spdx = cos(spdfacing)*spd;
	_spdy = sin(spdfacing)*spd;
}

void Effect::SetAcceleration(double a)
{
	double af = accelerationfacing();
	_accx = a * cos(af);
	_accy = a * sin(af);
}

void Effect::SetAccelerationX(double ax)
{
	_accx = ax;
}

void Effect::SetAccelerationY(double ay)
{
	_accy = ay;
}

void Effect::SetAccelerationFacing(double af)
{
	double acc = acceleration();
	_accx = acc * cos(af);
	_accy = acc * sin(af);
}

void Effect::print(DWORD timedelta)
{
	_ExistTime += timedelta * _anmspd;
	if (_ExistTime >=_type->_timelimit) {
		delete this;
		return;
	}

	double ftimedelta=(double)timedelta / 1000;
	_x += _spdx * ftimedelta + 0.5*_accx*pow(ftimedelta, 2);
	_y += _spdy * ftimedelta + 0.5*_accy*pow(ftimedelta, 2);
	_spdx += _accx * ftimedelta;
	_spdy += _accy * ftimedelta;
	int CurrentAnmTick = (int)(_ExistTime*_type->_defanmspd) % (_type->_reshighbound - _type->_reslowbound+1) + _type->_reslowbound;
	_type->_res->print({ _x,_y }, CurrentAnmTick, RA_ALLCENTER);
}

std::set<Effect*> Effect::EveryEffect()
{
	return _EveryEffect;
}

void Effect::ShowEffect()
{
	static DWORD lasttime = GetTickCount();
	DWORD time = GetTickCount();
	
	for (Effect* e : EveryEffect()) {
		e->print(time - lasttime);
	}
	lasttime = time;
}

Effect * Effect::CreateEffect(double x, double y, std::string type)
{
	int n = _EveryEffect.size();
	if (n >= EFFECT_MAX_NUM) return nullptr;
	return new Effect(x, y, type);
}
