#pragma once
#include "../Engine/Sprite.h"
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
//#include "HitBox.h"

enum class Boss1_Anim {
    waiting,
    walk,
    strike,
    dash,
    stump,
    summonfire,
    dead
};

class HitBox;
class Wisp;

class Boss1 : public PROJECT::GameObject {
public:
    Boss1(math::vec2 pos, const std::filesystem::path& spriteFilePath, PROJECT::GameObject* target);
    ~Boss1();

	GameObjectType GetObjectType() override { return GameObjectType::OniBoss; };
	std::string GetObjectTypeName() override { return "OniBoss"; };

	void Update(double dt) override;
    void Draw(math::TransformMatrix displayMatrix) override;

	bool CanCollideWith(GameObjectType objectType) override;
	void ResolveCollision(GameObject* objectA) override;

private:
    class Boss1_State_Waiting : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };
    class Boss1_State_Walking : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Wakling"; }
    };
    class Boss1_State_Strike : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Attack"; }
    };
    class Boss1_State_DashAttack : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Dash"; }
    };
    class Boss1_State_StumpAttack : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Jumping"; }
    };
    class Boss1_State_SummonFire : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Summon Fire"; }
    };
    class Boss1_State_Falling : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "falling"; }
    };
    class Boss1_State_Dead : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Dead"; }
    };

    Boss1_State_Waiting stateWaiting;
    Boss1_State_Walking stateWalking;
    Boss1_State_Strike stateStrike;
    Boss1_State_DashAttack stateDash;
    Boss1_State_StumpAttack stateStump;
    Boss1_State_SummonFire stateSummonFire;
    Boss1_State_Falling stateFalling;
    Boss1_State_Dead stateDead;

    void UpdateXVelocity(double dt);
    static constexpr double xDrag = 3000;
    static constexpr double xAccel = 3000;
    static constexpr double attackReadyRange = 450;

    PROJECT::GameObject* target;
    HitBox* hitboxPtr;

    float staminaTimer;
    double timer;
    static constexpr int fireSpeed = 700;
    static constexpr int fireSkillStamina = 40;
    std::vector<Wisp*> wisps;
};

