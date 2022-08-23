#include "../Engine/Engine.h"
#include "../Engine/Camera.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/ShowCollision.h"
#include "../Engine/Sprite.h"
#include "Screens.h"
#include "Map.h"
#include "Village.h"
#include "Fonts.h"
#include "Floor.h"
#include "Enemy.h"
#include "Player.h"
#include "NPC.h"
#include "Portal.h"
#include "GameParticles.h"
#include "UI.h"
#include "Scroll.h"

Village::Village()
{}

void Village::Load() {
	MapNameFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(GetName(), 0xFFFFFFFF, true);

	PROJECT::GameObjectManager* gomm = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>();
	AddGSComponent(new PROJECT::GameObjectManager());
	PROJECT::GameObjectManager* gom = GetGSComponent<PROJECT::GameObjectManager>();

	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>()->SetExtent({ { 0, -100 }, { Engine::GetWindow().GetSize().x, 1000 } });

	playerPtr = gomm->GetGameObject<Player>();
	if (Engine::GetGameStateManager().ComparePrestate(static_cast<int>(Screens::Field1)) == true) {
		playerPtr->SetPosition({ 1300 , 100 });
	}
	else { playerPtr->SetPosition({ 350, 100 }); }
	playerPtr->SetVelocity({ 0,0 });

	read_floor();
	gom->Add(new Portal({ { 1300, 96 }, Screens::Field1, "Level1" }));
	gom->Add(new NPC({ 1280, 928 }, "assets/NPCs/test/test.npc"));

	background.Add( "assets/background/background_1.png",0);
	background.Add("assets/background/Trees2.png", 4);
	background.Add("assets/background/Trees.png", 2);
	//gom->Add(new Scroll({ 800, 300 }));
	Engine::GetAudio().PlayBGM(&Engine::GetAudio().BGMVillage);
}


void Village::Update(double dt) {
	Map::Update(dt);
}

void Village::Unload() {
	ClearGSComponent();
	playerPtr->SetFloat();
	playerPtr = nullptr;
}

void Village::Draw() {
	Map::Draw();
}