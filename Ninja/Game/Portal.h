#pragma once
#include "..\Engine\GameObject.h"
#include "..\Engine\GameState.h"
#include "../Engine/input.h"
#include "GameObjectTypes.h"
#include "Screens.h"

class Portal : public PROJECT::GameObject
{
	using GameKey = PROJECT::InputKey::Keyboard;

public:
	Portal(math::ivec2 pos, Screens nextscreen , std::string name);
	GameObjectType GetObjectType() override { return GameObjectType::Portal; }
	std::string GetObjectTypeName() override { return "Portal"; }

	bool CanCollideWith(GameObjectType objectType) override;
	void ResolveCollision(GameObject* objectA) override;
	void Draw(math::TransformMatrix displayMatrix) override;
	void Update(double dt) override;
private:
	int NextScreen;
	bool CanGoToNext;
	PROJECT::InputKey moveNextKey;
	PROJECT::Texture nameFont;
	PROJECT::Texture* Texture;
};