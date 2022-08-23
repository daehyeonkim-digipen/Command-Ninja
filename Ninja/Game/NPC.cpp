#include "../Engine/Engine.h"
#include "../Engine/Sprite.h"
#include "../Engine/Collision.h"
#include "../Engine/TransformMatrix.h"
#include "../Engine/UIManager.h"
#include "../Engine/GameObjectManager.h"

#include "NPC.h"
#include "Fonts.h"
#include "Quest.h"
#include "Player.h"
#include "Scroll.h"
#include "UI.h"

NPC::NPC(math::vec2 pos, const std::filesystem::path& npcFilePath) {
	Load(npcFilePath);
	SetPosition(pos);
	ChangeState(&stateStanding);
	nameFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture(name, 0xFFFFFFFF, true);

	AddTexture("assets/NPCs/quest_mark.png");
	AddTexture("assets/NPCs/quest_during_mark.png");
	AddTexture("assets/NPCs/quest_done_mark.png");

	if (Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>()->GetGameObject<Player>()->HasClearedNpcsQuest(GetObjectTypeName())) {
		RemoveGOComponent<Quest>();
	}
}

void NPC::AddTexture(const std::filesystem::path& filesp) {
	questMarkTextures.push_back(Engine::GetTextureManager().Load(filesp));
}

void NPC::Load(const std::filesystem::path& npcFilePath) {
	if (npcFilePath.extension() != ".npc") throw std::runtime_error("valid extension : " + npcFilePath.extension().generic_string());
	std::ifstream inFile{ npcFilePath };
	if (inFile.is_open() == false) throw std::runtime_error(".npc file doesn't open.");

	while (inFile.eof() == false) {
		std::string text;
		inFile >> text;

		if (text == "Sprite") {
			inFile >> text;
			AddGOComponent(new PROJECT::Sprite(text, this));
		}
		else if (text == "Quest") {
			inFile >> text;
			AddGOComponent(new Quest(text));
		}
		else if (text == "Name") {
			inFile >> name;
			while (name.find("_") != std::string::npos) {
				name.replace(name.find("_"), 1, " ");
			}
		}

	}
}

void NPC::Update(double dt) {
	if (IsMouseOver() == true && Engine::GetInput().IsMousePressed(PROJECT::InputMouse::MouseButton::Left)) {
		Player* player = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>()->GetGameObject<Player>();
		Quest& playerQuest = player->GetTakenQuest();
		Quest* npcQuest = GetGOComponent<Quest>();

		if (npcQuest != nullptr && playerQuest.IsAvailable() == false) {
			player->TakeQuest(GetGOComponent<Quest>());
			Engine::GetLogger().LogEvent("Quest Accepted");
		}

		if (playerQuest.IsQuestDone() == true && playerQuest.GetNpcName() == GetObjectTypeName()) {
			Engine::GetGSComponent<PROJECT::GameObjectManager>()->Add(new Scroll(GetPosition()));
			player->ResetQuest();
			if (npcQuest->IsLoop() == false) {
				player->AddQuestNpcName(GetObjectTypeName());
			}
			RemoveGOComponent<Quest>();
		}
	}

	GameObject::Update(dt);
}

void NPC::Draw(math::TransformMatrix displayMatrix) {
	GameObject::Draw(displayMatrix);

	nameFont.Draw(displayMatrix * math::TranslateMatrix{ math::vec2{ GetPosition().x - (nameFont.GetSize().x * 0.4 / 2),
		GetPosition().y - nameFont.GetSize().y * 0.4 - 5 } } *math::ScaleMatrix{ math::vec2{ 0.4, 0.4 } });

	screenMousePosition = Engine::GetInput().GetMousePosition().operator math::vec2() - (displayMatrix * math::vec2(1, 1));

	if (GetGOComponent<Quest>() != nullptr) {
		__int8 currQuestTextureIndex = 0;

		math::vec2 textureSize = questMarkTextures[currQuestTextureIndex]->GetSize().operator math::vec2();
		Quest& playerQuest = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>()->GetGameObject<Player>()->GetTakenQuest();

		if (playerQuest.IsAvailable() == true && playerQuest.GetNpcName() == GetObjectTypeName()) {
			playerQuest.IsQuestDone() ? currQuestTextureIndex = 2 : currQuestTextureIndex = 1;
		}
		else {
			currQuestTextureIndex = 0;
		}

		questMarkTextures[currQuestTextureIndex]->Draw(displayMatrix * math::TranslateMatrix{ GetPosition() + math::vec2{ -textureSize.x / 2, GetGOComponent<PROJECT::Sprite>()->GetFrameSize().y + 7.0 } });
	}
}

bool NPC::IsMouseOver() {
	math::vec2 mouse = screenMousePosition;
	math::rect2 worldCord = GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect();

	return worldCord.Left() <= mouse.x && mouse.x <= worldCord.Right() && worldCord.Bottom() <= mouse.y && mouse.y <= worldCord.Top();
}



void NPC::NPC_State_Standing::Enter(GameObject* object) {
	NPC* npc = static_cast<NPC*>(object);
	npc->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(NPC_Anim::Idle));
}
void NPC::NPC_State_Standing::Update(GameObject*, double) {}
void NPC::NPC_State_Standing::TestForExit(GameObject* object) {
	NPC* npc = static_cast<NPC*>(object);
	if (npc->GetGOComponent<PROJECT::Sprite>()->IsAnimationDone() == true) {
		npc->ChangeState(&npc->stateStanding);
	}
}

