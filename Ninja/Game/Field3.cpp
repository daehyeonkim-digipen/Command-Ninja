#include "../Engine/Engine.h"
#include "../Engine/Camera.h"
#include "../Engine/GameObjectManager.h"

#include "Fonts.h"
#include "Floor.h"
#include "Enemy.h"
#include "Player.h"
#include "Portal.h"
#include "Field3.h"

Field3::Field3()
{}

void Field3::Load() {
	MapNameFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(GetName(), 0xFFFFFFFF, true);
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>()->SetExtent({ { -600, -100 }, { 2000, 1000 } });

	playerPtr = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>()->GetGameObject<Player>();
	if (Engine::GetGameStateManager().ComparePrestate(static_cast<int>(Screens::Field2)) == true) {
		playerPtr->SetPosition({ 150, 100 });
	}
	else if (Engine::GetGameStateManager().ComparePrestate(static_cast<int>(Screens::BossField1)) == true) {
		playerPtr->SetPosition({ 192, 864 });
	}

	PROJECT::GameObjectManager* gom = DEBUG_NEW PROJECT::GameObjectManager();
	AddGSComponent(gom);

	read_floor();
	gom->Add(DEBUG_NEW Portal({ 150, 100 }, Screens::Field2, "Field 2"));
	gom->Add(DEBUG_NEW Portal({ 192, 864 }, Screens::BossField1, "Boss Field"));

	gom->Add(DEBUG_NEW AngryOni({ 672, 224 }));
	gom->Add(DEBUG_NEW AngryOni({ 704, 736 }));
	gom->Add(DEBUG_NEW AngryOni({ 1152, 608 }));

	background.Add("assets/background/background_1.png", 0);
	background.Add("assets/background/Trees2.png", 4);
	background.Add("assets/background/Trees.png", 2);
}

void Field3::Update(double dt) {
	Map::Update(dt);
}

void Field3::Unload() {
	ClearGSComponent();
	playerPtr->SetFloat();
	playerPtr = nullptr;
}

void Field3::Draw() {
	Map::Draw();
	Map::DrawDeadScene();
}
