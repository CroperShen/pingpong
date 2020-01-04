#pragma once
#include "interface.h"
#include <string>
#include <vector>

const double MENUSELECTOR_BASE_SPEED = 180;
class GameMenu;

class MenuItem:public Entity {
private:
	std::string _caption;
	const int _id;
public:
	MenuItem(std::string caption,int uid,GameMenu* parent);

	std::string caption() const;
	int uid() const;

	void SetCaption(std::string sz);
	void printvisible(HDC hdc, location loc);
	void UpdateLocalData();
};

class MenuSelector :public Entity {
private:
	MenuItem *_SelectedItem;
public:
	MenuSelector(GameMenu* parent);
	void printvisible(HDC hdc, location loc);
	void SetItem(MenuItem *Item);
	void UpdateLocalData();

	MenuItem* Item();
};

class GameMenu :public Interface {
private:
	MenuSelector _p;
	std::vector<MenuItem*> Items;
public:
	GameMenu(double width, double height);
	MenuItem* AddItem(std::string caption, int uid,int x,int y);
	void UpdateLocalData();

	std::string SelectedItemCaption();
};
