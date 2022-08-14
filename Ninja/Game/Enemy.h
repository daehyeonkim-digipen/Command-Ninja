#pragma once
#include "..\Engine\GameObject.h"
#include "..\Engine\GameState.h"
#include "../Engine/input.h"
#include "GameObjectTypes.h"

enum class Enemy_Anim {
	Walk_Anim,
	Damaged_Anim,
	Dead_Anim
};

class Bar;
class HitBox;
class Enemy : public PROJECT::GameObject {
public:
	Enemy(math::vec2 pos, const std::filesystem::path& enmFilePath);
	~Enemy();

	GameObjectType GetObjectType() override { return GameObjectType::Enemy; }
	std::string GetObjectTypeName() override { return "Enemy"; }

	bool CanCollideWith(GameObjectType objectType) override;
	void ResolveCollision(GameObject* objectA) override;
	void Draw(math::TransformMatrix displayMatrix) override;
	void Update(double dt) override;

private:
	void Load(const std::filesystem::path& enmFilePath);

private:
	class Enemy_State_Patrol : public State {
	public:
		void Enter(GameObject* object) override;
		void Update(GameObject* object, double dt) override;
		void TestForExit(GameObject* object) override;
		std::string GetName() override { return "Patrol"; }
	};
	class Enemy_State_Falling : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void TestForExit(GameObject* object) override;
		std::string GetName() override { return "Falling"; }
	};
	class Enemy_State_Damaged : public State {
	public:
		void Enter(GameObject* object) override;
		void Update(GameObject* object, double dt) override;
		void TestForExit(GameObject* object) override;
		std::string GetName() override { return "Damaged"; }
	};
	class Enemy_State_Dead : public State {
	public:
		void Enter(GameObject* object) override;
		void Update(GameObject* object, double dt) override;
		void TestForExit(GameObject* object) override;
		std::string GetName() override { return "Dead"; }
	};

	Enemy_State_Patrol statePatrol;
	Enemy_State_Falling stateFalling;
	Enemy_State_Damaged stateDamaged;
	Enemy_State_Dead stateDead;
	HitBox* hitboxPtr;
	Bar* barPtr;

	static constexpr int velocity = 75;
	static constexpr double hpBarShownTime = 2.0;
	double hpBarShownTimer;
};

class Wisp : public Enemy {
public:
	Wisp(math::vec2 pos)
		: Enemy(pos, "assets/Enemy/wisp/wisp.enm")
	{};

	GameObjectType GetObjectType() override { return GameObjectType::OniWisp; }
	std::string GetObjectTypeName() override { return "Oni Wisp"; }
};

class DullOni : public Enemy {
public:
	DullOni(math::vec2 pos)
		: Enemy(pos, "assets/Enemy/dull_oni/dull.enm")
	{};

	GameObjectType GetObjectType() override { return GameObjectType::DullOni; }
	std::string GetObjectTypeName() override { return "Dull Oni"; }
};

class AngryOni : public Enemy {
public:
	AngryOni(math::vec2 pos)
		: Enemy(pos, "assets/Enemy/angry_oni/angry.enm")
	{};

	GameObjectType GetObjectType() override { return GameObjectType::AngryOni; }
	std::string GetObjectTypeName() override { return "Angry Oni"; }
};