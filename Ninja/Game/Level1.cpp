/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Level1.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include "../Engine/Engine.h"
#include "../Engine/Camera.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/ShowCollision.h"
#include "../Engine/Sprite.h"
#include "Screens.h"
#include "Level1.h"
#include "Fonts.h"
#include "Gravity.h"
#include "Floor.h"
#include "Enemy.h"
#include "Player.h"
#include "GameParticles.h"
#include "UI.h"
#include "Portal.h"

Level1::Level1()
{}

void Level1::Load() {
	MapNameFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(GetName(), 0xFFFFFFFF, true);

	PROJECT::GameObjectManager* gomm = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>();
	PROJECT::GameObjectManager* gom = DEBUG_NEW PROJECT::GameObjectManager();

	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>()->SetExtent({ { -500, 0 }, { 1000, 0 } });

	AddGSComponent(gom);

	playerPtr = gomm->GetGameObject<Player>();
	playerPtr->SetPosition({ 196, 100 });
	playerPtr->SetVelocity({ 0,0 });

	
	gom->Add(DEBUG_NEW Portal({ { 100, 100 }, Screens::Village, "Village" }));
	gom->Add(DEBUG_NEW Enemy({ 650, 600 }, 100, playerPtr));
}


void Level1::Update(double dt) {
	
	Map::Update(dt);
}

void Level1::Unload() {
	ClearGSComponent();
	playerPtr->standingOnObject = nullptr;
	playerPtr = nullptr;
}

void Level1::Draw() {
	Map::Draw();
	Map::DrawDeadScene();
}