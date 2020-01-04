#define _USE_MATH_DEFINES

#include "System.h"
#include "setting.h"
#include "IOCtrl.h"
#include "Obj.h"
#include "Wall.h"
#include "Ball.h"
#include "Racket.h"
#include "Brick.h"
#include "GameMenu.h"
#include "ResManager.h"

#include <windows.h>
#include <thread>
#include <time.h>
#include <math.h>

using namespace std;

unsigned int System::Score, System::Level;
GameState System::_gamestate;
Racket* System::MainRacket;
map<string, Interface*> System::_interfaces;

void System::InitRes()
{
	ResManager::Init(IOCtrl::hdc());

	ResManager::LoadResource(RESPATH + "Ball.bmp", "Ball",7,7);
	ResManager::LoadResource(RESPATH + "Racket.bmp", "Racket",120,6);
	ResManager::LoadResource(RESPATH + "Item.bmp", "Item", 15,15);
	ResManager::LoadResource(RESPATH + "Brick.bmp", "Brick", 20,8);
	ResManager::LoadResource(RESPATH + "ScoreBoard.bmp", "ScoreBoard", 100,50);
	ResManager::LoadResource(RESPATH + "MainMenu.bmp", "MainMenu",240,270);

	ResManager::LoadResource(RESPATH + "Effect0.bmp", "Spark0", 2, 2);
	ResManager::LoadResource(RESPATH + "Effect1.bmp", "Spark1", 4, 4);
	ResManager::LoadResource(RESPATH + "explosion1.bmp", "Explosion1", 30, 30);
}

void System::InitEffect()
{
	EffectType::RegisterEffect("Spark1", "Spark1",0, 3, 180, 0.01,
		[](Effect* that) {
		double x = that->GetX() + rand() % 8 - 4;
		double y = that->GetY() + rand() % 8 - 4;
		that->SetX(x);
		that->SetY(y);
	}
	);

	EffectType::RegisterEffect("ExplosionSpark", "Spark1", 0,3, 720, 0.003,
		[](Effect* that) {
		that->SetSpeed(40 + rand() % 40);
		that->SetSpeedFacing(2 * M_PI*(rand() % 1000) / 1000);
		that->SetAccelerationX(that->speedX()*-0.36);
		that->SetAccelerationY(that->speedY()*-0.36);
		}
	);

	EffectType::RegisterEffect("Explosion1", "Explosion1",0, 12, 360, 0.036,
			[](Effect* that) {
		for (int i = 0; i < 20; ++i)
			Effect::CreateEffect(that->GetX(), that->GetY(), "ExplosionSpark");
	}
	);

	EffectType::RegisterEffect("little Explosion_enum", "Spark0", 0,0, 360, 0.036,
		[](Effect* that) {
		that->SetSpeed(40 + rand() % 40);
		that->SetSpeedFacing(2 * M_PI*(rand() % 1000) / 1000);
		that->SetAccelerationY(100);
	}
	);

	EffectType::RegisterEffect("little Explosion", "Spark0", 0,0,0,0,
		[](Effect* that) {
		for (int i = 0; i < 5; ++i)
			Effect::CreateEffect(that->GetX(), that->GetY(), "little Explosion_enum");
	}
	);
}

GameMenu * System::CreateMenu(std::string name, double width, double height)
{
	if (_interfaces[name] == nullptr) {
		_interfaces[name] = new GameMenu(width, height);
	}
	return (GameMenu*)_interfaces[name];
}

Interface * System::CreateInterface(std::string name, double width, double height)
{
	if (_interfaces[name] == nullptr) {
		_interfaces[name] = new Interface(width, height);
	}
	return  _interfaces[name];
}

Interface * System::GetInterface(std::string name)
{
	return _interfaces[name];
}

GameMenu * System::GetMenu(std::string name)
{
	return (GameMenu*)_interfaces[name];
}

void System::GameMainLoop()
{
	const size_t STD_TICK_GAP = 1000 / BASEFPS;
	size_t NextLgclFrame = GetTickCount();
	while (_gamestate!=state_quit) {
		size_t Now = GetTickCount();
		if (Now > NextLgclFrame) {
			UpdateGame();
			Now = GetTickCount();
			while (NextLgclFrame<Now) NextLgclFrame += STD_TICK_GAP;
		}

		double FracFrame = (double)(Now - NextLgclFrame + STD_TICK_GAP) / (STD_TICK_GAP);
		if (FracFrame > 1) FracFrame = 1;
		DisplayGame(FracFrame);
	}
}

void System::UpdateGame()
{
	Interface::UpTop()->UpdateData();
	if (_gamestate == state_runing) {
		GetInterface("ScoreBoard")->UpdateData();
	}
}

void System::DisplayGame(double fracframe)
{
	IOCtrl::clear();
	//PatBlt(IOCtrl::hdc(), 0, 0, CXINSIDE, CYINSIDE, BLACKNESS);
	HDC hdc = IOCtrl::hdc();
	for (Interface *p : Interface::EveryInterface()) {
		if (p == Interface::UpTop()) {
			p->print(hdc,fracframe);
		}
		else {
			p->print(hdc,0);
		}

	}
	Effect::ShowEffect();
	IOCtrl::Print(0);
}

void System::InitMenu()
{
	GameMenu* MainMenu = CreateMenu("MainMenu",GAMEWINDOW_LOGICALWIDTH, GAMEWINDOW_LOGICALHEIGHT);
	MainMenu->SetTop(0);
	MainMenu->SetLeft(120);
	MainMenu->AddItem("开始游戏", 0, 120, 140);
	MainMenu->AddItem("高分榜", 0, 120, 170);
	MainMenu->AddItem("退出游戏", 0, 120, 200);
	MainMenu->SetProcFunc(MainMenuProc);
	MainMenu->SetResname("MainMenu");

	Interface* GameWindow = CreateInterface("GameWindow",GAMEWINDOW_LOGICALWIDTH, GAMEWINDOW_LOGICALHEIGHT);
	GameWindow->SetTop(0);
	GameWindow->SetLeft(120);
	GameWindow->SetUpdateFunc(GameWindowUpdate);
	GameWindow->SetProcFunc(GameWindowProc);
	GameWindow->SetPrintFunc(GameWindowPrintFunc);
	GameWindow->SetVisible(false);

	Interface* StageClearWindow = CreateInterface("StageClearWindow", GAMEWINDOW_LOGICALWIDTH, GAMEWINDOW_LOGICALHEIGHT);
	StageClearWindow->SetTop(0);
	StageClearWindow->SetLeft(120);
	StageClearWindow->SetProcFunc(StageClearWindowProc);
	StageClearWindow->SetPrintFunc(StageClearWindowPrintFunc);
	StageClearWindow->SetVisible(false);

	Interface* ScoreBoard = CreateInterface("ScoreBoard",100,50);
	ScoreBoard->SetTop(100);
	ScoreBoard->SetLeft(370);
	ScoreBoard->SetPrintFunc(ScoreBoradPrintFunc);
	ScoreBoard->SetResname("ScoreBoard");
	ScoreBoard->SetVisible(false);

	GameMenu* PauseMenu = CreateMenu("PauseMenu", 120, 120);
	PauseMenu->SetTop(120);
	PauseMenu->SetLeft(120);
	PauseMenu->AddItem("继续游戏", 0, 60, 30);
	PauseMenu->AddItem("重新开始", 0, 60, 60);
	PauseMenu->AddItem("返回主菜单", 0, 60, 90);
	PauseMenu->SetVisible(false);
	PauseMenu->SetProcFunc(PauseMenuProc);

	GameMenu* DefeatMenu = CreateMenu("DefeatMenu", 150, 150);
	DefeatMenu->SetLoc(GameWindow->Center());
	DefeatMenu->AddItem("重新开始", 0, 75, 90);
	DefeatMenu->AddItem("返回主菜单", 0, 75, 120);
	DefeatMenu->SetVisible(false);
	DefeatMenu->SetProcFunc(PauseMenuProc);
	DefeatMenu->SetPrintFunc(DefeatMenuPrintFunc);

	Obj::SetMainWindow(GameWindow);
}

int System::MainMenuProc(Interface* that, MyMsg msg)
{
	switch (msg.msg) {
	case WM_LBUTTONDOWN: {
		string sz = ((GameMenu*)that)->SelectedItemCaption();
		if (sz == "开始游戏") {
			StartGame();
			return 0;
		}
		if (sz == "退出游戏") {
			IOCtrl::QuitGame();
			return 0;
		}
	}
	case WM_KEYDOWN: {
		int keycode = *(int*)(msg.Data.get());
		switch (keycode){
		case VK_ESCAPE:
			IOCtrl::QuitGame();
			return 0;
		}
	}
	}
	return 0;

}

int System::PauseMenuProc(Interface* that, MyMsg msg)
{
	switch (msg.msg) {
	case WM_LBUTTONDOWN: {
		string sz = ((GameMenu*)that)->SelectedItemCaption();
		if (sz == "继续游戏") {
			ResumeGame();
			return 0;
		}
		if (sz == "返回主菜单") {
			EndGame();
			return 0;
		}
		if (sz == "重新开始") {
			StartGame();
			return 0;
		}
	}
	}
	return 0;
}

int System::GameWindowProc(Interface* that, MyMsg msg)
{
	switch (msg.msg) {
	case WM_LBUTTONDOWN: {
		MainRacket->SetTeeOff();
		return 0;
	}
	case WM_KEYDOWN: {
		int keycode = *(int*)(msg.Data.get());
		switch (keycode) {
		case VK_ESCAPE:
			PauseGame();
			return 0;
		}
	}
	case WM_OBJUNLOAD: {
		string ObjClassName = *(string*)(msg.Data.get());
		if (ObjClassName == "Ball") {
			int n = Ball::EveryBall().size();
			if (n == 0) PauseGame(true);
			return 0;
		}
		if (ObjClassName == "Brick") {
			int n = Brick::EveryBrick().size();
			if (n == 0) StageClear();
			System::Score += 10;
			return 0;
		}
	}
	}
	return 0;
}


int System::StageClearWindowProc(Interface * that, MyMsg msg)
{
	switch (msg.msg) {
	case WM_LBUTTONDOWN:
	case WM_KEYDOWN: {
		Level++;
		GenerateStage(Level);
		_gamestate = state_runing;
		that->SetVisible(false);
		SendMessage(IOCtrl::m_hwnd(), WM_HIDECURSOR, 0, 0);
		ResetCursor();
	}
	}
	return 0;
}

int System::GameWindowUpdate(Interface* that)
{
	//检测并重置鼠标位置;
	location loc = IOCtrl::GetCursorLoc();
	MainRacket->SetSpeedX(loc.x - that->xCenter());
	ResetCursor();

	return 0;
}

int System::ScoreBoradPrintFunc(Interface* that, HDC hdc, location loc)
{
	SetTextAlign(IOCtrl::hdc(), TA_RIGHT);
	that->PrintText({ 90,30 }, to_string(Score));

	return 0;
}

int System::StageClearWindowPrintFunc(Interface * that, HDC hdc, location loc)
{
	SetTextAlign(IOCtrl::hdc(), TA_CENTER);

	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	that->PrintFrame(that->Center(), 1);
	SelectObject(hdc, hOldBrush);

	that->PrintText(location(that->width()/2, 50), "Stage Clear!");
	that->PrintText(location(that->width() / 2, 120), "当前得分：" + to_string(Score));
	that->PrintText(location(that->width() / 2, 190), "按任意键继续");

	return 0;
}

int System::GameWindowPrintFunc(Interface * that, HDC hdc, location loc)
{
	SetDCBrushColor(hdc, RGB(0, 40, 80));
	HBRUSH hBrush = (HBRUSH)GetStockObject(DC_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	that->PrintFrame(that->Center(), 1);
	SelectObject(hdc, hOldBrush);

	return 0;
}

int System::DefeatMenuPrintFunc(Interface * that, HDC hdc, location loc)
{
	SetTextAlign(IOCtrl::hdc(), TA_CENTER);

	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	that->PrintFrame(that->Center(), 1.0);
	SelectObject(hdc, hOldBrush);

	that->PrintText(location(that->width() / 2, 20), "Game Over");

	return 0;
}

bool System::Init(HWND hwnd)
{
	IOCtrl::Init(hwnd);
	InitRes();
	InitEffect();
	InitMenu();
	
	_gamestate = state_stop;
	UpdateWindow(hwnd);
	return true;
}

void System::Run()
{
	thread _GameMainLoop(GameMainLoop);
	IOCtrl::MessageLoop();
	_gamestate = state_quit;
	_GameMainLoop.join();
}

void System::Shutdown()
{
	for (auto pa : _interfaces) {
		if (pa.second != nullptr) delete pa.second;
	}
}

void System::PauseGame(bool Defeated)
{
	_gamestate = state_pause;
	SendMessage(IOCtrl::m_hwnd(), WM_SHOWCURSOR, 0, 0);
	if (Defeated) {
		GetMenu("DefeatMenu")->SetVisible(true);
		GetMenu("DefeatMenu")->SetUpTop();
	}
	else {
		GetMenu("PauseMenu")->SetVisible(true);
		GetMenu("PauseMenu")->SetUpTop();
	}
}

void System::ResumeGame()
{
	_gamestate = state_runing;
	GetMenu("PauseMenu")->SetVisible(false);
	GetInterface("GameWindow")->SetUpTop();

	SendMessage(IOCtrl::m_hwnd(), WM_HIDECURSOR, 0, 0);
	ResetCursor();
}

void System::StartGame()
{
	for (Obj* obj : Obj::EveryObj()) {
		delete obj;
	}
	Wall *leftwall = new Wall(-GAMEWINDOW_LOGICALWIDTH, 0, 0, GAMEWINDOW_LOGICALHEIGHT);
	Wall *upwall = new Wall(0, -GAMEWINDOW_LOGICALHEIGHT, GAMEWINDOW_LOGICALWIDTH, 0);
	Wall *rightwall = new Wall(GAMEWINDOW_LOGICALWIDTH, 0, 2*GAMEWINDOW_LOGICALWIDTH, GAMEWINDOW_LOGICALHEIGHT);

	MainRacket = new Racket(GAMEWINDOW_LOGICALWIDTH / 2, GAMEWINDOW_LOGICALHEIGHT*0.9);
	MainRacket->using_predictfunc = false;
	Ball *ball = new Ball(MainRacket, GAMEWINDOW_LOGICALWIDTH / 2, MainRacket->top());

	_gamestate = state_runing;
	GetInterface("MainMenu")->SetVisible(false);
	GetInterface("PauseMenu")->SetVisible(false);
	GetInterface("DefeatMenu")->SetVisible(false);
	GetInterface("GameWindow")->SetVisible(true);
	GetInterface("ScoreBoard")->SetVisible(true);
	GetInterface("GameWindow")->SetUpTop();
	GetInterface("GameWindow")->ClearMsgQue();

	Score = 0;
	Level = 0;
	GenerateStage(Level);

	SendMessage(IOCtrl::m_hwnd(), WM_HIDECURSOR, 0, 0);
	ResetCursor();
}

void System::GenerateStage(int slvl)
{
	switch (slvl){
	case 0:
		for (int i = 0; i < 12; ++i) {
			for (int j = 0; j < 8; ++j) {
				double x = 10 + i * 20;
				double y = 4 + j * 8;
				new Brick(x, y, rand() % 6);
			}
		}
		break;
	case 1:
		for (int i = 0; i < 12; ++i) {
			for (int j = 0; j <= i; ++j) {
				double x = 10 + j * 20;
				double y = 4 + i * 8;
				int type = rand() % 6;
				if (i == 11) type += 6;
				new Brick(x, y, type);
			}
		}
		break;
	case 2:
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 12; ++j) {
				int type = rand() % 6;
				if (i == 0) type += 6;
				if ((i & 1) == 1) {
					if ((i & 2) == 2 && j < 8) type = 12;
					if ((i & 2) == 0 && j >= 4) type = 12;
				}

				double x = 10 + j * 20;
				double y = 12 + i * 16;
				new Brick(x, y, type);
			}
		}
		break;
	default:
		for (int i = 0; i < 12; ++i) {
			for (int j = 0; j < 16; ++j) {
				double x = 10 + i * 20;
				double y = 4 + j * 8;
				new Brick(x, y, rand() % 6+6);
			}
		}
	}

}

void System::EndGame()
{
	_gamestate = state_stop;
	GetInterface("MainMenu")->SetVisible(true);
	GetInterface("MainMenu")->SetUpTop();
	GetInterface("GameWindow")->SetVisible(false);
	GetInterface("ScoreBoard")->SetVisible(false);
	GetInterface("PauseMenu")->SetVisible(false);
	GetInterface("DefeatMenu")->SetVisible(false);
}

void System::StageClear()
{
	_gamestate = state_pause;
	SendMessage(IOCtrl::m_hwnd(), WM_SHOWCURSOR, 0, 0);
	GetInterface("StageClearWindow")->SetVisible(true);
	GetInterface("StageClearWindow")->SetUpTop();

	MainRacket->SetX(GAMEWINDOW_LOGICALWIDTH / 2);
	int i = 0;
	for (auto Obj : Ball::EveryObj()) {
		if (Obj->ClassName() == "Ball") {
			if (i == 0) {
				Obj->SetX(MainRacket->xCenter());
				((Ball*)Obj)->SetRacket(MainRacket);
				Obj->SetSpeed(0);
			}
			else Obj->DelThis();
			++i;
		}
		else if (Obj->ClassName() == "Item") {
			Obj->DelThis();
		}
	}
}


void System::ResetCursor()
{
	IOCtrl::SetCursorLoc(GetInterface("GameWindow")->Center());
}
