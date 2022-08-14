#pragma once
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"

class HitBox : public PROJECT::GameObject {
public:
	HitBox(math::vec2 pos, math::ivec2 size, GameObject* caster, int damage);

	void Update(double dt) override;

	GameObjectType GetObjectType() override { return GameObjectType::Hitbox; };
	GameObjectType GetCasterObjectType() { return caster->GetObjectType(); };
	std::string GetObjectTypeName() override { return "HitBox"; };

	bool CanCollideWith(GameObjectType objectType) override;
	void ResolveCollision(GameObject* objectA) override;

	void SetLifeTime(int value);
	int GetDamage() { return damage; }

private:
	std::vector<GameObject*> targets;
	GameObject* caster;
	int damage;
};

