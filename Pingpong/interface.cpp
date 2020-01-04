#include "interface.h"
#include "setting.h"
#include "ResManager.h"

using namespace std;

vector<Interface*> Interface::_EveryInterface;

MyMsg Interface::GetMsg()
{
	MyMsg Msg;
	unsigned int time = GetTickCount();
	while (!_MsgQue.empty()) {
		Msg = _MsgQue.front();
		_MsgQue.pop();
		if (time - Msg.time <= OPRT_DELAY_TOLERANCE) return Msg;
	}
	Msg.msg = 0;
	return Msg;
}

vector<Interface*> Interface::EveryInterface()
{
	return _EveryInterface;
}

Interface * Interface::UpTop()
{
	for (auto iter = _EveryInterface.rbegin(); iter != _EveryInterface.rend();++iter) {
		if ((**iter).visible()) return (*iter);
	}
	return nullptr;
}

Interface::Interface(double width, double height,Entity* parent):Entity(parent)
{
	SetWidth(width);
	SetHeight(height);
	_ProcFunc = nullptr;
	_UpdateFunc = nullptr;
	_EveryInterface.push_back(this);
}
Interface::~Interface()
{
	for (auto iter = _EveryInterface.begin(); iter != _EveryInterface.end();++iter) {
		if (*iter == this) {
			_EveryInterface.erase(iter);
			return;
		}
	}
}


void Interface::UpdateLocalData()
{
	if (_ProcFunc != nullptr) {
		MyMsg msg;
		while ((msg = GetMsg()).msg != 0) {
			_ProcFunc(this,msg);
		}
	}
	if (_UpdateFunc != nullptr) {
		_UpdateFunc(this);
	}
}

void Interface::printvisible(HDC hdc, location loc)
{
	location lefttop;
	lefttop.x = loc.x-width() / 2;
	lefttop.y = loc.y-height() / 2;
	Res* res = ResManager::GetRes(resname());
	if (res!=nullptr) res->print(lefttop);
	if (_PrintFunc!=nullptr) _PrintFunc(this,hdc, lefttop);

	if (res == nullptr && _PrintFunc == nullptr)
	{
		HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		PrintFrame(loc, 1.0);
		SelectObject(hdc, hOldBrush);
	};
}

void Interface::SetUpTop()
{
	for (auto iter = _EveryInterface.begin(); iter != _EveryInterface.end(); ++iter) {
		if (*iter == this) {
			_EveryInterface.erase(iter);
			_EveryInterface.push_back(this);
			return;
		}
	}
}

void Interface::SetProcFunc(Interface_ProcFunc ProcFunc)
{
	_ProcFunc = ProcFunc;
}

void Interface::SetUpdateFunc(Interface_UpdateFunc UpdateFunc)
{
	_UpdateFunc = UpdateFunc;
}

void Interface::SetPrintFunc(Interface_PrintFunc PrintFunc)
{
	_PrintFunc = PrintFunc;
}


#include "System.h"

void Interface::SendMsg(MyMsg msg)
{
	msg.pInterface = this;
	msg.time = GetTickCount();
	_MsgQue.push(msg);
}

void Interface::ClearMsgQue()
{
	while (!_MsgQue.empty()) _MsgQue.pop();
}


