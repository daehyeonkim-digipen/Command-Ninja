#pragma once

#include <unordered_map>
#include "../Engine/GameObject.h"
#include "../Engine/Input.h"
#include "GameObjectTypes.h"
#include "Quest.h"

enum class Player_Anim {
    Idle,
    Run,
    Jump,
    Fall,
    Casting,
    Damaged,
    Attack1,
    Attack2,
    Attack3,
};

class Player : public PROJECT::GameObject
{
    using GameKey = PROJECT::InputKey::Keyboard;
    friend class StatWindow;
    friend class QuestWindow;
public:
	Player(math::vec2 startPos);
    ~Player();

    void Update(double dt) override;
    void Draw(math::TransformMatrix displayMatrix) override;
	
	GameObjectType GetObjectType() override { return GameObjectType::Player; };
	std::string GetObjectTypeName() override { return "Player"; };

	bool CanCollideWith(GameObjectType objectType) override;
	void ResolveCollision(GameObject* objectA) override;

    void TakeQuest(Quest* quest_);
    void ResetQuest();
    Quest& GetTakenQuest() { return quest; }
    void AddQuestNpcName(const std::string& name) { clearQuestNpcNames.push_back(name); };
    bool HasClearedNpcsQuest(const std::string& name) { return std::find(clearQuestNpcNames.begin(), clearQuestNpcNames.end(), name) != clearQuestNpcNames.end(); }

    void AddClearedGameState(std::string screen) { clearedScreens.push_back(screen); }
    bool HasClearedGameState(std::string screen) { return std::find(clearedScreens.begin(), clearedScreens.end(), screen) != clearedScreens.end(); }

    GameObject* overridingItem;

    bool IsSignCasting() { return currState == &stateCasting; };
    void UpdateStemina(int stemina) { attributes.Stamina += stemina; }

    bool Alteration = false;
    double castingSlowAmount = 4.0;

    bool isSteminaDone() { return attributes.Stamina <= 0; }
    void alteration(std::string);
private:
    void UpdateStaminaRegen(double dt);

private:
    class Player_State_Idle : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };
    class Player_State_Running : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        bool StepSoundSwitch = false;
    	std::string GetName() override { return "Running"; }
    };
    class Player_State_Jumping : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Jumping"; }
    };
    class Player_State_Falling : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Falling"; }
    };
    class Player_State_Casting : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Casting"; }
    };
    class Player_State_Attacking : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Attacking"; }
    };
    class Player_State_Damaged : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Damaged"; }
    };

    Player_State_Idle stateIdle;
    Player_State_Running stateRunning;
    Player_State_Jumping stateJumping;
    Player_State_Falling stateFalling;
    Player_State_Casting stateCasting;
    Player_State_Attacking stateAttacking;
    Player_State_Damaged stateDamaged;

    void UpdateXVelocity(double dt, bool canMove);
    void UpdateSignCasting();
    void ResetLevel();

    static constexpr double xAccel = 2000;
    static constexpr double xDrag = 3000;
    static constexpr double maxXVelocity = 350;
    static constexpr double jumpVelocity = 1000;

    PROJECT::InputKey jumpKey;
    PROJECT::InputKey attackKey;
    PROJECT::InputKey CastKey;
    PROJECT::InputKey collectKey;

    PROJECT::InputKey moveLeftKey;
    PROJECT::InputKey moveRightKey;

    PROJECT::InputKey signKey1;
    PROJECT::InputKey signKey2;
    PROJECT::InputKey signKey3;
    PROJECT::InputKey signKey4;

    std::string signCommand;
    
protected:
    Quest quest;
    std::vector<std::string> clearQuestNpcNames;
    std::vector<std::string> clearedScreens;

    static constexpr int maxHpLimit = 700;
    static constexpr int maxStaminaLimit = 300;
    static constexpr double staminaRegenTime = 3;

    double staminaRegenTimer = 3;
    int staminaRegenAmount = 10;

    int attackTime;
    int attackMilliSecond = -1;
    static constexpr int attackms = 55;
};

