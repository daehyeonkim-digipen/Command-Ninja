/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Field1.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include "../Engine/Engine.h"
#include "../Engine/Camera.h"
#include "../Engine/GameObjectManager.h"

#include "Fonts.h"
#include "Floor.h"
#include "Enemy.h"
#include "Player.h"
#include "Portal.h"
#include "Field1.h"
#include "doodle/doodle.hpp"

Field1::Field1()
{}

void Field1::Load() {
	MapNameFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(GetName(), 0xFFFFFFFF, true);
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>()->SetExtent({ { -500, -100 }, { 1000, 1000 } });

	playerPtr = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>()->GetGameObject<Player>();
	if (Engine::GetGameStateManager().ComparePrestate(static_cast<int>(Screens::Village)) == true) {
		playerPtr->SetPosition({ -300, 96 });
	}
	else if (Engine::GetGameStateManager().ComparePrestate(static_cast<int>(Screens::Field2)) == true) {
		playerPtr->SetPosition({ 2000, 96 });
	}

	// playerPtr->SetVelocity({ 0,0 });

	PROJECT::GameObjectManager* gom = new PROJECT::GameObjectManager();
	AddGSComponent(gom);

	read_floor();

	gom->Add(new Portal({ { -300, 96 }, Screens::Village, "Village" }));
	gom->Add(new Portal({ { 2000, 96 }, Screens::Field2, "Field 2" }));

	gom->Add(new Wisp({ 352, 352 }));
	gom->Add(new Wisp({ 600, 100 }));
	gom->Add(new Wisp({ 1000, 100 }));
	//gom->Add(new Wisp({ 1600, 100 }));

	background.Add("assets/background/background_1.png", 0);
	background.Add("assets/background/Trees2.png", 4);
	background.Add("assets/background/Trees.png", 2);

}


void Field1::Update(double dt) {
	Map::Update(dt);
}

void Field1::Unload() {
	ClearGSComponent();
	playerPtr->SetFloat();
	playerPtr = nullptr;
}

void Field1::Draw() {
	Map::Draw();
	Map::DrawDeadScene();
}