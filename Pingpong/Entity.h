#ifndef ENTITY_H
#define ENTITY_H


#include <windows.h>
#include <string.h>
#include <set>
#include "location.h"
//Entity.h
//��ʾĳ�ִ���
//ӵ�д�С��λ�ã�(�ܹ����ƶ������ܹ�����ʾ����



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

	//�������ĵ�X
	virtual void SetX(double x);

	//�������ĵ�Y
	virtual void SetY(double y);

	//�������ĵ�XY
	virtual void SetXY(double x, double y);

	//�������ĵ�
	virtual void SetLoc(location loc);

	//changewidht��ture=>����right���䣻false=>����width����
	virtual void SetLeft(double left,bool changewidth =false);

	//changewidht��ture=>����left���䣻false=>����width����
	virtual void SetRight(double right,bool changewidth = false);

	//changeheight��ture=>����bottom���䣻false=>����height����
	virtual void SetTop(double top,bool changeheight = false);

	//changeheight��ture=>����top���䣻false=>����height����
	virtual void SetBottom(double bottom,bool changeheight = false);

	//�������ĵ���Թ̶�
	virtual void SetWidth(double width);

	//�������ĵ���Թ̶�
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