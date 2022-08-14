#pragma once
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"

class Scroll : public PROJECT::GameObject {
public:
	Scroll(math::vec2 pos);

	void Update(double dt) override;
	void Draw(math::TransformMatrix displayMatrix) override;

	GameObjectType GetObjectType() override { return GameObjectType::Scroll; };
	std::string GetObjectTypeName() override { return "Scroll"; };

	bool CanCollideWith(GameObjectType objectType) override;
	void ResolveCollision(GameObject* objectA) override;

private:
	PROJECT::Texture* texture;

	bool isFalling;
};

