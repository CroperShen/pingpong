#ifndef ENTITY_H
#define ENTITY_H


#include <windows.h>
#include <string.h>
#include <set>
#include "location.h"
//Entity.h
//表示某种存在
//拥有大小和位置，(能够）移动，（能够）显示自身；



class Entity {
private:
	bool _del;
	double _x, _y, _cx, _cy;
	double _spd, _spdf;
	bool _visible;
	int _type;
	std::string _resname;
	Entity* const _parent;
	std::set <Entity*> _children;
public:
	bool using_predictfunc;
	Entity(Entity* parent=NULL);
	virtual ~Entity();

	double left() const;
	double right() const;
	double top() const;
	double bottom() const;
	double xCenter() const;
	double yCenter() const;
	location Center() const;
	double width() const;
	double height() const;
	int type() const;
	std::string resname() const;

	double GetSpeed() const;
	double GetSpeedX() const;
	double GetSpeedY() const;
	double GetSpeedFacing() const;
	void DelThis();

	//==================================================
	bool NoMove() const;               

	bool visible() const;

	//设置中心点X
	virtual void SetX(double x);

	//设置中心点Y
	virtual void SetY(double y);

	//设置中心点XY
	virtual void SetXY(double x, double y);

	//设置中心点
	virtual void SetLoc(location loc);

	//changewidht：ture=>保持right不变；false=>保持width不变
	virtual void SetLeft(double left,bool changewidth =false);

	//changewidht：ture=>保持left不变；false=>保持width不变
	virtual void SetRight(double right,bool changewidth = false);

	//changeheight：ture=>保持bottom不变；false=>保持height不变
	virtual void SetTop(double top,bool changeheight = false);

	//changeheight：ture=>保持top不变；false=>保持height不变
	virtual void SetBottom(double bottom,bool changeheight = false);

	//保持中心点相对固定
	virtual void SetWidth(double width);

	//保持中心点相对固定
	virtual void SetHeight(double height);

	virtual void SetSpeed(double speed);
	virtual void SetSpeedX(double speedx);
	virtual void SetSpeedY(double speedy);
	virtual void SetSpeedXY(double speedx, double speedy);
	virtual void SetSpeedFacing(double facing);

	void SetVisible(bool visible);
	virtual void SetType(int type);
	void SetResname(std::string resname);

	virtual void print(HDC hdc, double fracframe = 0);
	virtual void printvisible(HDC hdc,location loc);

	void UpdateData();
	void defUpdateLocalData();
	virtual void UpdateLocalData();

	Entity* parent() const;
	std::set<Entity*> children();

	void PrintText(location loc, std::string sz);
	void PrintFrame(location loc, double extend = 0.0);
};

#endif