//Interface.h
//界面，拥有自己的消息循环；

#ifndef INTERFACE_H
#define INTERFACE_H

#include <windows.h>
#include <vector>
#include <queue>
#include "Entity.h"
#include "MyMessage.h"


typedef int(*Interface_ProcFunc)(Interface* that,MyMsg);
typedef int(*Interface_UpdateFunc)(Interface* that);
typedef int(*Interface_PrintFunc)(Interface* that,HDC hdc, location loc);

class Interface:public Entity {
	static std::vector<Interface*> _EveryInterface;
	std::queue<MyMsg> _MsgQue;
	Interface_ProcFunc _ProcFunc;
	Interface_UpdateFunc _UpdateFunc;
	Interface_PrintFunc _PrintFunc;
	MyMsg GetMsg();
public:
	static std::vector<Interface*> EveryInterface();
	static Interface* UpTop();

	Interface(double width,double height,Entity* parent =  nullptr);
	virtual ~Interface();
	Interface(const Interface&) = delete;
	void operator=(const Interface&) = delete;
	
	void UpdateLocalData();
	void printvisible(HDC hdc,location loc);
	void SetUpTop();
	void SetProcFunc(Interface_ProcFunc ProcFunc);
	void SetUpdateFunc(Interface_UpdateFunc UpdateFunc);
	void SetPrintFunc(Interface_PrintFunc PrintFunc);
	void SendMsg(MyMsg msg);
	void ClearMsgQue();
};

#endif