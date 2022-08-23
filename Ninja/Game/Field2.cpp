#include "../Engine/Engine.h"
#include "../Engine/Camera.h"
#include "../Engine/GameObjectManager.h"

#include "Fonts.h"
#include "Floor.h"
#include "Enemy.h"
#include "Player.h"
#include "Portal.h"
#include "Field2.h"

Field2::Field2()
{}

void Field2::Load() {
	MapNameFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(GetName(), 0xFFFFFFFF, true);
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>()->SetExtent({ { -2500, -100 }, { 500, 1000 } });

	playerPtr = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>()->GetGameObject<Player>();
	if (Engine::GetGameStateManager().ComparePrestate(static_cast<int>(Screens::Field1)) == true) {
		playerPtr->SetPosition({ 100, 96 });
	}
	else if (Engine::GetGameStateManager().ComparePrestate(static_cast<int>(Screens::Field3)) == true) {
		playerPtr->SetPosition({ -2304, 480 });
	}

	PROJECT::GameObjectManager* gom = new PROJECT::GameObjectManager();
	AddGSComponent(gom);

	read_floor();
	gom->Add(new Portal({ 100, 96 }, Screens::Field1, "Field 1"));
	gom->Add(new Portal({ -2304, 480 }, Screens::Field3, "Field 3"));

	gom->Add(new DullOni({ -700, 96 }));
	gom->Add(new Wisp({ -1400, 96 }));
	gom->Add(new DullOni({ -800, 384 }));
	gom->Add(new Wisp({ -1760, 384 }));

	background.Add("assets/background/background_1.png", 0);
	background.Add("assets/background/Trees2.png", 4);
	background.Add("assets/background/Trees.png", 2);
}

void Field2::Update(double dt) {
	Map::Update(dt); 
}

void Field2::Unload() {
	ClearGSComponent();
	playerPtr->SetFloat();
	playerPtr = nullptr;
}

void Field2::Draw() {
	Map::Draw();
	Map::DrawDeadScene();
}
