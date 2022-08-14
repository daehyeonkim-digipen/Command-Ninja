/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include <stdexcept>	//std::exception
#include <iostream>		//std::cerr
#include <crtdbg.h>
#define CRTDBG_MAP_ALLOC

#include "Engine/Engine.h"
#include "Engine/ComponentManager.h"
#include "Game/Village.h"
#include "Game/Field1.h"
#include "Game/Field2.h"
#include "Game/Field3.h"
#include "Game/BossField1.h"
#include "Game/Screens.h"
#include "Game/MainMenu.h"
#include "Game/Settings.h"

int main(void) {
	try {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		Engine& engine = Engine::Instance();
		engine.Init("Sub Project");
		MainMenu mainmenu;
		Settings settings;
		Village village;
		Field1 field1;
		Field2 field2;
		Field3 field3;
		BossField1 bossField1;

		engine.GetGameStateManager().AddGameState(mainmenu);
		engine.GetGameStateManager().AddGameState(settings);
		engine.GetGameStateManager().AddGameState(village);
		engine.GetGameStateManager().AddGameState(field1);
		engine.GetGameStateManager().AddGameState(field2);
		engine.GetGameStateManager().AddGameState(field3);
		engine.GetGameStateManager().AddGameState(bossField1);
		engine.AddSpriteFont("assets/font.png");
		engine.AddSpriteFont("assets/font2.png");

		while (engine.HasGameEnded() == false) {
			engine.Update();
		}
		engine.Shutdown();

		return 0;
	} catch (std::exception& e) {
		std::cerr << e.what() << "\n";
		return -1;
	}
}
