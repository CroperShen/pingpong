#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <map>
#include <windows.h>
#include "interface.h"
#include "GameMenu.h"
#include "IOCtrl.h"
#include "Racket.h"
#include "Effect.h"


enum GameState { state_stop, state_runing, state_pause,state_quit};

class System{
private:
	static GameState _gamestate;
	static std::map<std::string,Interface*> _interfaces;
	static void InitRes();
	static void InitEffect();
public:
	static unsigned int Score,Level;
	static Racket* MainRacket;

	static GameMenu* CreateMenu(std::string name,double width,double height);
	static Interface* CreateInterface(std::string name,double width,double height);
	static Interface* GetInterface(std::string name);
	static GameMenu* GetMenu(std::string name);

	static void GameMainLoop();
	static void UpdateGame();
	static void DisplayGame(double fracframe);
	static void InitMenu();

	static int MainMenuProc(Interface* that,MyMsg msg);
	static int PauseMenuProc(Interface* that, MyMsg msg);
	static int GameWindowProc(Interface* that, MyMsg msg);
	static int StageClearWindowProc(Interface* that, MyMsg msg);

	static int GameWindowUpdate(Interface* that);
	static int ScoreBoradPrintFunc(Interface* that, HDC hdc, location loc);
	static int StageClearWindowPrintFunc(Interface* that, HDC hdc, location loc);
	static int GameWindowPrintFunc(Interface* that, HDC hdc, location loc);
	static int DefeatMenuPrintFunc(Interface* that, HDC hdc, location loc);

	static bool Init(HWND hwnd);
	static void Run();
	static void Shutdown();

	static void PauseGame(bool Defeated = false);
	static void ResumeGame();
	static void StartGame();
	static void GenerateStage(int slvl);
	static void EndGame();
	static void StageClear();

	static void ResetCursor();
};

#endif