#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Texture.h"
#include "../Engine/Input.h"

#include "GameObjectTypes.h"
#include "UI.h"

enum class NPC_Anim {
	Idle
};

class NPC : public PROJECT::GameObject
{
public:
	NPC(math::vec2 pos, const std::filesystem::path& npcFilePath);

	void Update(double dt) override;
	void Draw(math::TransformMatrix displayMatrix) override;

	GameObjectType GetObjectType() override { return GameObjectType::NPC; };
	std::string GetObjectTypeName() override { return name; };

private:
	void Load(const std::filesystem::path& npcFilePath);
	bool IsMouseOver();
	void AddTexture(const std::filesystem::path& filesp);

private:
	class NPC_State_Standing : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void TestForExit(GameObject* object) override;
		std::string GetName() override { return "Standing"; }
	};
	NPC_State_Standing stateStanding;
	
	std::string name;
	PROJECT::Texture nameFont;

	std::vector<PROJECT::Texture*> questMarkTextures;
};
