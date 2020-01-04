#include "GameMenu.h"
#include "IOCtrl.h"
#include "setting.h"

MenuItem::MenuItem(std::string caption, int uid, GameMenu * parent) :Entity(parent), _id(uid)
{
	_caption = caption;
	SetHeight(30);
	SetWidth(120);
}


std::string MenuItem::caption() const
{
	return _caption;
}

int MenuItem::uid() const
{
	return _id;
}

void MenuItem::SetCaption(std::string sz)
{
	_caption = sz;
}

void MenuItem::printvisible(HDC hdc, location loc)
{
	loc.y += 3.0;
	SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
	IOCtrl::StringOut(loc, _caption);
}

void MenuItem::UpdateLocalData()
{
}

MenuSelector::MenuSelector(GameMenu * parent):Entity(parent)
{
	_SelectedItem = nullptr;
	SetSpeed(5);
}

void MenuSelector::printvisible(HDC hdc, location loc)
{
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	PrintFrame(loc);
	SelectObject(hdc, hOldBrush);
}

void MenuSelector::SetItem(MenuItem * Item)
{
	if (Item == nullptr) {
		SetVisible(false);
		_SelectedItem = Item;
	}
	else {
		if (_SelectedItem == nullptr) {
			SetTop(Item->top(),true);
			SetLeft(Item->left(),true);
			SetRight(Item->right(),true);
			SetBottom(Item->bottom(), true);
			SetVisible(true);
		}
		_SelectedItem = Item;
	}
}

inline void MenuSelector::UpdateLocalData()
{
	if (_SelectedItem != nullptr) {
		double d = Center().GetDistance(_SelectedItem->Center());
		double f = Center().GetFacingAngle(_SelectedItem->Center());
		if (d > MENUSELECTOR_BASE_SPEED / BASEFPS) {
			SetSpeed(MENUSELECTOR_BASE_SPEED/BASEFPS);
			SetSpeedFacing(f);
			double d2 = max(0, d - GetSpeed());
			double p = d2 / d;
			double q = (d - d2) / d;
			SetWidth(width()*p + _SelectedItem->width()*q);
			SetHeight(height()*p + _SelectedItem->height()*q);
			defUpdateLocalData();
		}
		else {
			SetSpeed(0);
			SetX(_SelectedItem->xCenter());
			SetY(_SelectedItem->yCenter());
			SetWidth(_SelectedItem->width());
			SetHeight(_SelectedItem->height());
		}
	}
}

MenuItem * MenuSelector::Item()
{
	return _SelectedItem;
}

GameMenu::GameMenu(double cx,double cy) :Interface(cx, cy, nullptr), _p(this)
{
}


MenuItem * GameMenu::AddItem(std::string caption, int uid, int x, int y)
{
	MenuItem* p= new MenuItem(caption, uid, this);
	p->SetXY(x, y);
	Items.push_back(p);
	return p;
}

void GameMenu::UpdateLocalData()
{
	location CursorLoc = IOCtrl::LocalCursorLoc(*this);

	if (CursorLoc.x < 0) return;
	for (MenuItem* item : Items) {
		if (CursorLoc.InRect(item->left(), item->top(), item->right(), item->bottom())) {
			_p.SetItem(item);
		}
	}
	Interface::UpdateLocalData();
}

std::string GameMenu::SelectedItemCaption()
{
	if (_p.Item() != nullptr) {
		return (_p.Item()->caption());
	}
	return "";
}
