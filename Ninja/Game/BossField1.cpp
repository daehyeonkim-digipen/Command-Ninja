#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/Camera.h"

#include "Fonts.h"
#include "Floor.h"
#include "Boss1.h"
#include "Portal.h"

#include "BossField1.h"

BossField1::BossField1()
{}

void BossField1::Load() {
	MapNameFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(GetName(), 0xFFFFFFFF, true);
	PROJECT::GameObjectManager* gomm = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>();
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>()->SetExtent({ { 0, -100 }, { 1200, 1000 } });

	playerPtr = gomm->GetGameObject<Player>();
	playerPtr->SetPosition({ 150, 100 });

	PROJECT::GameObjectManager* gom = DEBUG_NEW PROJECT::GameObjectManager();
	AddGSComponent(gom);

	read_floor();
	if (playerPtr->HasClearedGameState(GetName()) == false) {
		gom->Add(DEBUG_NEW Boss1({ 1500, 120 }, "assets/Bosses/Boss1/Boss1.spt", playerPtr));
	}
	else {
		gom->Add(DEBUG_NEW Portal({ 50, 96 }, Screens::Field1, "Field 1"));
		gom->Add(DEBUG_NEW Portal({ 1100, 96 }, Screens::MainMenu, "Next Chapter"));
	}

	/* 5 4 10 6 -1
	  35 4 40 6 -1*/

	background.Add("assets/background/background_1.png", 0);
	background.Add("assets/background/Trees2.png", 4);
	background.Add("assets/background/Trees.png", 2);

}

void BossField1::Update(double dt) {
	Map::Update(dt);

	Boss1* boss = GetGSComponent<PROJECT::GameObjectManager>()->GetGameObject<Boss1>();
	if (boss != nullptr && boss->IsDead() == true) {
		playerPtr->AddClearedGameState(GetName());
		GetGSComponent<PROJECT::GameObjectManager>()->Add(DEBUG_NEW Portal({ 50, 96 }, Screens::Field1, "Field 1"));
		GetGSComponent<PROJECT::GameObjectManager>()->Add(DEBUG_NEW Portal({ 1100, 96 }, Screens::MainMenu, "Next Chapter"));
		boss->SetCount();
	}
}

void BossField1::Unload() {
	ClearGSComponent();
	playerPtr->SetFloat();
	playerPtr = nullptr;
}

void BossField1::Draw() {
	Map::Draw();
	Map::DrawDeadScene();
}
