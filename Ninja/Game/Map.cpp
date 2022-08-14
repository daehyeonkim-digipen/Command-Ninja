#include "../Engine/Engine.h"
#include "../Engine/Camera.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/ShowCollision.h"
#include "../Engine/Sprite.h"
#include "../Engine/UIManager.h"
#include "../Engine/SkillManager.h"
#include "../Engine/ParticleEmitter.h"

#include "Screens.h"
#include "Map.h"
#include "Fonts.h"
#include "Gravity.h"
#include "Floor.h"
#include "Enemy.h"
#include "Player.h"
#include "GameSkills.h"
#include "GameParticles.h"
#include "UI.h" 

Map::Map() :playerPtr(nullptr),deadTime(0) {
	black_bg = Engine::GetTextureManager().Load("assets/background_black.png");
	U_DIED = Engine::GetTextureManager().Load("assets/died.png");
	for(int i = 4; i < 10; i++)
	{
		floors[i] = Engine::GetTextureManager().Load("assets/mapData/tile" + std::to_string(i) + ".png");
	}
}

Map::~Map() {
	black_bg = nullptr;
	U_DIED = nullptr;
	floors.clear();
}


void Map::Unload() {
	Engine::GetGameStateManager().GetGSMComponents().Clear();
}

void Map::Update(double dt) {
	if (playerPtr->IsSignCasting() == true) {
		dt /= playerPtr->castingSlowAmount;
	}
	// GetGSMComponent<PROJECT::Camera>()->Update(playerPtr->GetPosition());
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>()->Update(playerPtr->GetPosition());

	UpdateGSComponents(dt);
	Engine::GetGameStateManager().GetGSMComponents().UpdateAll(dt);

	if (mainMenu.IsKeyReleased() == true) {
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenu));
	}
#ifdef _DEBUG
	if (playerPtr->IsDead() == false && levelReload.IsKeyReleased() == true) {
		Engine::GetGameStateManager().ReloadState();
	}
#endif

	if (playerPtr->IsDead() == true) {
		deadTime += dt;
	}
}

void Map::Draw() {
	PROJECT::Camera* cameraPtr = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>();
	Engine::GetWindow().Clear(0x3399DAFF);
	background.Draw();
	MapNameFont.Draw(math::TranslateMatrix{ math::ivec2{ 50, 600 } });
	
	math::TransformMatrix cameraMatrix = cameraPtr->GetMatrix();
	GetGSComponent<PROJECT::GameObjectManager>()->DrawAll(cameraMatrix);
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>()->DrawAll(cameraMatrix);
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>()->Draw();
}

void Map::read_floor()
{
	int          map_info;
	std::ifstream maptxt;
	constexpr int blockDefaultSize = 32;
	math::ivec2 windowSize = Engine::GetWindow().GetSize();
	std::vector<int> Size;
	maptxt.open("assets/mapData/" + GetName() + ".txt");
	if (maptxt.is_open())
	{
		while (maptxt >> map_info)
		{
			if(map_info != -1)
			{
				Size.push_back(blockDefaultSize * map_info);
			}
			else
			{
				GetGSComponent<PROJECT::GameObjectManager>()->Add(DEBUG_NEW Floor(math::irect2{ {Size[0], Size[1] }, { Size[2], Size[3] }}, floors));
				Size.clear();
			}
		}
	}
	maptxt.close();
}

void Map::DrawDeadScene() {
	if (playerPtr->IsDead() == false)
		return;
	
	math::vec2 windowSize = Engine::GetWindow().GetSize().operator math::vec2();

	black_bg->Draw(math::ivec2{ 0, 0 }, (int)(deadTime * 65));
	U_DIED->Draw(math::ivec2{ (int)(-deadTime * 140 / 2), (int)(-deadTime * 80 / 2) },
		(int)(windowSize.x + deadTime * 140), (int)(windowSize.y + deadTime * 80), (int)(std::sin(deadTime) * 250));

	if (deadTime > 4.5) {
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenu));
		deadTime = 0;
	}

}

void Map::load_Map() {
#ifdef _DEBUG
	Engine::GetGameStateManager().AddGSMComponent(DEBUG_NEW ShowCollision(PROJECT::InputKey::Keyboard::Tilde));
#endif
	Engine::GetGameStateManager().AddGSMComponent(DEBUG_NEW Gravity(Map::gravity));

	math::vec2 windowSize = Engine::GetWindow().GetSize().operator math::vec2();
	PROJECT::Camera* cameraPtr = DEBUG_NEW PROJECT::Camera({ { windowSize.x * 0.45, windowSize.y * 0.4 }, { windowSize.x * 0.55, windowSize.y * 0.5 } });
	Engine::GetGameStateManager().AddGSMComponent(cameraPtr);
	cameraPtr->SetExtent({ { 0, 0 }, { 0, 0 } });

	Engine::GetGameStateManager().AddGSMComponent(DEBUG_NEW PROJECT::UIManager());
	Engine::GetGameStateManager().AddGSMComponent(DEBUG_NEW SkillManager());

	PROJECT::GameObjectManager* gomm = DEBUG_NEW PROJECT::GameObjectManager();
	Engine::GetGameStateManager().AddGSMComponent(gomm);

	gomm->Add(DEBUG_NEW Player({ 150, 150 }));

	//Engine::GetGameStateManager().AddGSMComponent(DEBUG_NEW SkillsManager());
	cameraPtr->SetPosition(gomm->GetGameObject<Player>()->GetPosition() - Engine::GetWindow().GetSize().operator math::vec2() / 2);
	Engine::GetGameStateManager().AddGSMComponent(DEBUG_NEW AttackEmitter());
	Engine::GetGameStateManager().AddGSMComponent(DEBUG_NEW FireEmitter());
}