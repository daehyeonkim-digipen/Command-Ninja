#include "../Engine/Engine.h"
#include "../Engine/Sprite.h"
#include "../Engine/Collision.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/UIManager.h"

#include "Gravity.h"
#include "PopUpDamage.h"
#include "UI.h"
#include "Bar.h"
#include "GameSkills.h"
#include "Scroll.h"
#include "HitBox.h"
#include "Enemy.h"

Enemy::Enemy(math::vec2 pos, const std::filesystem::path& enmFilePath)
	: GameObject(pos), hpBarShownTimer(0), barPtr(nullptr), hitboxPtr(nullptr)
{
	Load(enmFilePath);
	ChangeState(&stateFalling);
	attributes.Hp = attributes.maxHp;

	PROJECT::UIManager* UIM = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>();
	barPtr = DEBUG_NEW HPBar(GetPosition(), UIType::EnemyBar, "assets/Enemy/enemy_hpbar_empty.png", this, 40);
	UIM->AddUI(barPtr);
}

void Enemy::Load(const std::filesystem::path& enmFilePath) {
	if (enmFilePath.extension() != ".enm") throw std::runtime_error("Expected .enm extension but taken : " + enmFilePath.extension().generic_string());
	std::ifstream inFile{ enmFilePath };
	if (inFile.is_open() == false) throw std::runtime_error("File does not opened : " + enmFilePath.generic_string());

	while (inFile.eof() == false) {
		std::string text;
		inFile >> text;

		if (text == "Sprite") {
			inFile >> text;
			AddGOComponent(DEBUG_NEW PROJECT::Sprite(text, this));
		}
		else if (text == "HP") {
			inFile >> attributes.maxHp;
		}
	}
}

Enemy::~Enemy() {
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>()->RemoveUI(this);
}

void Enemy::Update(double dt) {
	GameObject::Update(dt);

	if (hpBarShownTimer >= 0) {
		hpBarShownTimer -= dt;
	}
	barPtr->SetVisible(hpBarShownTimer > 0);

	if (GetPosition().y < -300) {
		ChangeState(&state_nothing);
		SetVelocity({ 0, 0 });
	}
	//if (Engine::GetInput().IsMousePressed(PROJECT::InputMouse::MouseButton::Left) && DoesCollideWith(screenMousePosition)) {
	//	Destroy();
	//}
}

bool Enemy::CanCollideWith(GameObjectType objectType)
{
	switch (objectType)
	{
	case GameObjectType::Hitbox:
		[[fallthrough]];
	case GameObjectType::Floor:
		return true;
	default:
		return false;
	}
}

void Enemy::ResolveCollision(PROJECT::GameObject* objectA) {
	math::rect2 collideRect = objectA->GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect();
	math::rect2 enemyRect = GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect();

	switch (objectA->GetObjectType())
	{
	case GameObjectType::Floor:
		if (GetPosition().x < collideRect.Right() && GetPosition().x > collideRect.Left() && enemyRect.Top() > collideRect.Top())
		{
			if (currState == &stateFalling || currState == &stateDamaged) {
				standingOnObject = objectA;
				SetVelocity({ GetVelocity().x, 0 });
				SetPosition({ GetPosition().x, collideRect.Top() });
				currState->TestForExit(this);
			}
		}
		break;
	case GameObjectType::Hitbox:
		if (objectA != hitboxPtr) {
			hitboxPtr = static_cast<HitBox*>(objectA);
			Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>()->AddUI(
				DEBUG_NEW PopUpDamage(GetPosition() + math::vec2{ 0, (double)GetGOComponent<PROJECT::Sprite>()->GetFrameSize().y * 0.8 }, hitboxPtr->GetDamage()));
			attributes.Hp -= hitboxPtr->GetDamage();
			ChangeState(&stateDamaged);
		}
		break;
	default:
		break;
	}
}

void Enemy::Draw(math::TransformMatrix displayMatrix) {
	GameObject::Draw(displayMatrix);

	if (barPtr != nullptr) {
		math::vec2 posoffset = GetPosition() + (displayMatrix * math::vec2(1, 1));
		math::vec2 spriteSize = GetGOComponent<PROJECT::Sprite>()->GetFrameSize().operator math::vec2();

		barPtr->SetPosition(posoffset + math::vec2{ -spriteSize.x / 3, spriteSize.y });
	}
}

void Enemy::Enemy_State_Patrol::Enter(GameObject* object) {
	Enemy* enemy = static_cast<Enemy*>(object);
	enemy->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Enemy_Anim::Walk_Anim));

	double direction = rand() % 2;
	if (direction == 0) {
		enemy->SetVelocity({ -Enemy::velocity, 0 });
		enemy->SetScale({ 1, 1 });
	}
	else if (direction == 1) {
		enemy->SetVelocity({ Enemy::velocity, 0 });
		enemy->SetScale({ -1, 1 });
	}
}
void Enemy::Enemy_State_Patrol::Update(GameObject* object, double dt) {
	Enemy* enemy = static_cast<Enemy*>(object);
	if (enemy->standingOnObject == nullptr)
		return;

	math::rect2 floor = enemy->standingOnObject->GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect();
	if (0 < enemy->GetVelocity().x && floor.Right() - 10 <= enemy->GetPosition().x) {
		enemy->SetVelocity({ -Enemy::velocity, 0 });
		enemy->SetScale({ 1, 1 });
	}
	else if (enemy->GetVelocity().x < 0 && enemy->GetPosition().x <= floor.Left() + 10) {
		enemy->SetVelocity({ Enemy::velocity, 0 });
		enemy->SetScale({ -1, 1 });
	}
}
void Enemy::Enemy_State_Patrol::TestForExit(GameObject* object) {
	Enemy* enemy = static_cast<Enemy*>(object);
	if (enemy->standingOnObject != nullptr && enemy->standingOnObject->DoesCollideWith(enemy->GetPosition()) == false) {
		enemy->standingOnObject = nullptr;
		enemy->ChangeState(&enemy->stateFalling);
	}
}


void Enemy::Enemy_State_Falling::Enter(GameObject* object) {
	Enemy* enemy = static_cast<Enemy*>(object);
	enemy->standingOnObject = nullptr;
}
void Enemy::Enemy_State_Falling::Update(GameObject* object, double dt) {
	Enemy* enemy = static_cast<Enemy*>(object);
	if (enemy->IsInvulnerable() == false)
		enemy->UpdateVelocity({ 0, -Engine::GetGameStateManager().GetGSMComponents().GetComponent<Gravity>()->GetValue() * dt });

	if (enemy->GetGOComponent<PROJECT::Sprite>()->IsAnimationDone() == true) {
		enemy->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Enemy_Anim::Walk_Anim));
	}
}
void Enemy::Enemy_State_Falling::TestForExit(GameObject* object) {
	Enemy* enemy = static_cast<Enemy*>(object);
	if (enemy->standingOnObject != nullptr) {
		enemy->ChangeState(&enemy->statePatrol);
	}
}

void Enemy::Enemy_State_Damaged::Enter(GameObject* object) {
	Enemy* enemy = static_cast<Enemy*>(object);
	enemy->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Enemy_Anim::Damaged_Anim));
	enemy->standingOnObject = nullptr;
	if (Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>()->
		GetGameObject<Player>()->GetPosition().x < enemy->GetPosition().x)
		enemy->SetVelocity({ 80, 0 });
	else {
		enemy->SetVelocity({ -80, 0 });
	}
}
void Enemy::Enemy_State_Damaged::Update(GameObject* object, double dt) {
	Enemy* enemy = static_cast<Enemy*>(object);
	if (enemy->standingOnObject == nullptr) {
		enemy->UpdateVelocity({ 0, -Engine::GetGameStateManager().GetGSMComponents().GetComponent<Gravity>()->GetValue() * dt });
	}
	enemy->hpBarShownTimer = enemy->hpBarShownTime;
}
void Enemy::Enemy_State_Damaged::TestForExit(GameObject* object) {
	Enemy* enemy = static_cast<Enemy*>(object);
	if (enemy->attributes.Hp <= 0) {
		enemy->attributes.Hp = 0;
		enemy->ChangeState(&enemy->stateDead);
	}
	else if (enemy->GetGOComponent<PROJECT::Sprite>()->IsAnimationDone() == true) {
		if (enemy->standingOnObject != nullptr) {
			enemy->ChangeState(&enemy->statePatrol);
		}
		else {
			enemy->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Enemy_Anim::Damaged_Anim));
			enemy->ChangeState(&enemy->stateFalling);
		}
	}
}


void Enemy::Enemy_State_Dead::Enter(GameObject* object) {
	Enemy* enemy = static_cast<Enemy*>(object);
	enemy->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Enemy_Anim::Dead_Anim));
	enemy->SetVelocity({ 0, 0 });
	enemy->RemoveGOComponent<PROJECT::Collision>();
	if (rand() % 100 < 10)
		Engine::GetGSComponent<PROJECT::GameObjectManager>()->Add(DEBUG_NEW Scroll(enemy->GetPosition()));

	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>()->RemoveUI(enemy);
} 
void Enemy::Enemy_State_Dead::Update(GameObject*, double) {
}
void Enemy::Enemy_State_Dead::TestForExit(GameObject* object) {
	//Enemy* enemy = static_cast<Enemy*>(object);
	//if (enemy->GetGOComponent<PROJECT::Sprite>()->IsAnimationDone() == true) {
	//	enemy->ChangeState(&enemy->state_nothing);
	//	enemy->ClearGOComponents();
	//	enemy->Destroy();
	//	return;
	//}
}