#include "../Engine/Engine.h"
#include "../Engine/UIManager.h"
#include "../Engine/Collision.h"
#include "../Engine/Camera.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/SkillManager.h"

#include "GameSkills.h"
#include "Gravity.h"
#include "Bar.h"
#include "PopUpDamage.h"
#include "HitBox.h"
#include "Enemy.h"
#include "Boss1.h"

Boss1::Boss1(math::vec2 pos, const std::filesystem::path& spriteFilePath, PROJECT::GameObject* target_)
	: timer(0), target(target_), hitboxPtr(nullptr)
{
	AddGOComponent(DEBUG_NEW PROJECT::Sprite(spriteFilePath, this));
	ChangeState(&stateFalling);
	SetPosition(pos);

	attributes.Hp = attributes.maxHp = 700;
	attributes.Stamina = attributes.maxStamina = 600;
	attributes.Atk = 20;

	PROJECT::UIManager* UIM = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>();
	UIM->AddUI(DEBUG_NEW HPBar({ 300, 40 }, UIType::BossBar, "assets/Bosses/boss_hpbar_empty.png", this, 875 / 2));
	UIM->AddUI(DEBUG_NEW StaminaBar({ 300, 35 }, UIType::BossBar, "assets/Bosses/boss_stamina_empty.png", this, 875 / 2));

	Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->Unlock("Boss1 Stump");
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->Unlock("Boss1 Dash");
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->Unlock("Boss1 Strike");

}

Boss1::~Boss1() {
	// Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>()->RemoveUI(this);
	standingOnObject = nullptr;
	target = nullptr;
}

void Boss1::Update(double dt) {
	GameObject::Update(dt);

	for (int i = 0; i < static_cast<int>(wisps.size()); i++) {
		wisps[i]->attributes.Hp--; // millisecond lifetime
		if (wisps[i]->attributes.Hp > 200) {
			wisps[i]->SetPosition(GetPosition() + math::vec2{ -150 + (double)i * 50, 150.0 });
		}
		else if (wisps[i]->attributes.Hp <= 200 && 0 < wisps[i]->attributes.Stamina) {
			math::vec2 force = (target->GetPosition() - wisps[i]->GetPosition()).Normalize() * Boss1::fireSpeed;
			wisps[i]->SetVelocity(force);
			wisps[i]->attributes.Stamina = 0;
		}
	}

	if (wisps.size() == 5 && wisps.back()->attributes.Hp <= 0) {
		for (Wisp* w : wisps) {
			w->Destroy();
			Engine::GetLogger().LogDebug("destroyed");
		}

		wisps.clear();
		Engine::GetLogger().LogDebug("cleared");
	}
}

void Boss1::Draw(math::TransformMatrix displayMatrix) {
	GameObject::Draw(displayMatrix);
}

bool Boss1::CanCollideWith(GameObjectType objectType) {
	switch (objectType) {
	case GameObjectType::Floor: [[fallthrough]];
	case GameObjectType::Hitbox:
		return true;
		break;
	}

	return false;
}

void Boss1::ResolveCollision(GameObject* objectA) {
	math::rect2 collideRect = objectA->GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect();
	math::rect2 bossRect = GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect();

	switch (objectA->GetObjectType())
	{
	case GameObjectType::Floor:
		if (GetPosition().x < collideRect.Right() && GetPosition().x > collideRect.Left())
		{
			if (currState == &stateFalling || currState == &stateWalking || currState == &stateDead) {
				if (bossRect.Top() > collideRect.Top() && objectA->DoesCollideWith(GetPosition())) {
					standingOnObject = objectA;
					SetPosition({ GetPosition().x, collideRect.Top() });
					SetVelocity({ GetVelocity().x, 0 });
					return;
				}
			}
		}
		break;
	case GameObjectType::Hitbox:
		if (GetObjectType() == static_cast<HitBox*>(objectA)->GetCasterObjectType())
			break;
		if (objectA != hitboxPtr) {
			hitboxPtr = static_cast<HitBox*>(objectA);
			Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>()->AddUI(
				DEBUG_NEW PopUpDamage(GetPosition() + math::vec2{ 0, (double)GetGOComponent<PROJECT::Sprite>()->GetFrameSize().y * 0.8 }, hitboxPtr->GetDamage()));
			attributes.Hp -= hitboxPtr->GetDamage();
			if (IsDead() == true) {
				attributes.Hp = 0;
				ChangeState(&stateDead);
			}
		}
		break;
	default:
		break;
	}
}

void Boss1::UpdateXVelocity(double dt) {
	double xDragDt = Boss1::xDrag * dt;
	if (GetVelocity().x > xDragDt) {
		UpdateVelocity({ -xDragDt, 0 });
	}
	else if (GetVelocity().x < -xDragDt) {
		UpdateVelocity({ xDragDt, 0 });
	}
	else {
		SetVelocity({ 0, GetVelocity().y });
	}
}


void Boss1::Boss1_State_Waiting::Enter(GameObject* object) {
	Boss1* boss1 = static_cast<Boss1*>(object);
	boss1->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Boss1_Anim::waiting));
	boss1->SetVelocity({ 0, 0 });
	boss1->timer = 1.8;
	boss1->GetPosition().x >= boss1->target->GetPosition().x ? boss1->SetScale({ 1, 1 }) : boss1->SetScale({ -1, 1 });
}
void Boss1::Boss1_State_Waiting::Update(GameObject* object, double dt) {
	Boss1* boss1 = static_cast<Boss1*>(object);
	if (boss1->timer > 0)
		boss1->timer -= dt;

	if (boss1->staminaTimer > 0) {
		boss1->staminaTimer -= dt;
	}
	else {
		boss1->attributes.Stamina ++;
		boss1->staminaTimer = 0.05;
	}
}
void Boss1::Boss1_State_Waiting::TestForExit(GameObject* object) {
	Boss1* boss = static_cast<Boss1*>(object);
	if (boss->timer < 0) {
		double distance = std::pow(boss->GetPosition().x - boss->target->GetPosition().x, 2);
		if (distance < std::pow(Boss1::attackReadyRange * 0.6, 2)) {
			int pattern = rand() % 100;

			if (pattern < 20) {
				if (boss->attributes.Stamina < Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->GetCost("Boss1 Stump")) {
					boss->ChangeState(&boss->stateWalking);
					return;
				}
				boss->ChangeState(&boss->stateStump);
			}
			else if (20 <= pattern && pattern < 35) {
				if (boss->attributes.Stamina < Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->GetCost("Boss1 Dash")) {
					boss->ChangeState(&boss->stateWalking);
					return;
				}
				boss->ChangeState(&boss->stateDash);
			}
			else if (35 <= pattern && pattern < 75) {
				boss->ChangeState(&boss->stateStrike);
			}
			else {
				if (boss->wisps.empty() == true) {
					if (boss->attributes.Stamina < Boss1::fireSkillStamina) {
						boss->ChangeState(&boss->stateWalking);
						return;
					}
					boss->ChangeState(&boss->stateSummonFire);
				}
				else {
					boss->ChangeState(&boss->stateWalking);
				}

			}
		}
		else if (distance < std::pow(Boss1::attackReadyRange * 0.8, 2) || distance > std::pow(Boss1::attackReadyRange * 2.2, 2)) {
			if (boss->attributes.Stamina < Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->GetCost("Boss1 Dash")) {
				boss->ChangeState(&boss->stateWalking);
				return;
			}
			boss->ChangeState(&boss->stateDash);
		}
		else if (distance < std::pow(Boss1::attackReadyRange * 1.5, 2)) {
			if (boss->attributes.Stamina < Boss1::fireSkillStamina) {
				boss->ChangeState(&boss->stateWalking);
				return;
			}
			boss->ChangeState(&boss->stateSummonFire);
		}
	}
}

void Boss1::Boss1_State_Walking::Enter(GameObject* object) {
	Boss1* boss = static_cast<Boss1*>(object);
	boss->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Boss1_Anim::walk));
	boss->timer = 2;

	boss->GetPosition().x >= boss->target->GetPosition().x ? boss->SetScale({ 1, 1 }) : boss->SetScale({ -1, 1 });
}
void Boss1::Boss1_State_Walking::Update(GameObject* object, double dt) {
	Boss1* boss = static_cast<Boss1*>(object);
	boss->UpdateXVelocity(dt);
	boss->UpdateVelocity(math::vec2{ -Boss1::xAccel * dt, 0 } *boss->GetScale());
	if (boss->timer > 0) boss->timer -= dt;
}
void Boss1::Boss1_State_Walking::TestForExit(GameObject* object) {
	Boss1* boss = static_cast<Boss1*>(object);
	double distance = std::pow(boss->GetPosition().x - boss->target->GetPosition().x, 2);
	if (boss->timer <= 0) {
		boss->ChangeState(&boss->stateWaiting);
	}
}



void Boss1::Boss1_State_DashAttack::Enter(GameObject* object) {
	Boss1* boss1 = static_cast<Boss1*>(object);
	boss1->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Boss1_Anim::dash));
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->Cast("Boss1 Dash", boss1);
}
void Boss1::Boss1_State_DashAttack::Update(GameObject* object, double dt) {
	Boss1* boss1 = static_cast<Boss1*>(object);
	boss1->UpdateXVelocity(dt);
}
void Boss1::Boss1_State_DashAttack::TestForExit(GameObject* object) {
	Boss1* boss1 = static_cast<Boss1*>(object);
	if (boss1->GetGOComponent<PROJECT::Sprite>()->IsAnimationDone() == true) {
		boss1->SetVelocity({ 0, 0 });
		boss1->ChangeState(&boss1->stateWalking);
	}
}


void Boss1::Boss1_State_StumpAttack::Enter(GameObject* object) {
	Boss1* boss1 = static_cast<Boss1*>(object);
	boss1->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Boss1_Anim::stump));
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->Cast("Boss1 Stump", boss1);
}
void Boss1::Boss1_State_StumpAttack::Update(GameObject* object, double dt) {
	Boss1* boss1 = static_cast<Boss1*>(object);
	if (boss1->GetVelocity().y > 0)
		boss1->UpdateVelocity({ boss1->GetVelocity().x, -Engine::GetGameStateManager().GetGSMComponents().GetComponent<Gravity>()->GetValue() * dt });
	boss1->UpdateXVelocity(dt);
}
void Boss1::Boss1_State_StumpAttack::TestForExit(GameObject* object) {
	Boss1* boss1 = static_cast<Boss1*>(object);
	if (boss1->GetVelocity().y < -50)
		boss1->ChangeState(&boss1->stateFalling);
}


void Boss1::Boss1_State_SummonFire::Enter(GameObject* object) {
	Boss1* boss = static_cast<Boss1*>(object);
	boss->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Boss1_Anim::summonfire));
	//boss->attackEnterTime = 0;
	boss->timer = 0.5;
	boss->attributes.Stamina -= Boss1::fireSkillStamina;
}
void Boss1::Boss1_State_SummonFire::Update(GameObject* object, double dt) {
	Boss1* boss = static_cast<Boss1*>(object);
	//boss->attackEnterTime += dt;
	if (boss->timer > 0)
		boss->timer -= dt;
	else if (boss->wisps.size() < 5) {
		Wisp* wisp = DEBUG_NEW Wisp(boss->GetPosition() + math::vec2{ -150, 130 } + math::vec2{ (double)boss->wisps.size() * 50, 0 });
		wisp->attributes.Hp = wisp->attributes.maxHp = 500;
		wisp->SetInvulnerable(true);
		boss->wisps.push_back(wisp);
		Engine::GetGSComponent<PROJECT::GameObjectManager>()->Add(wisp);
		Engine::GetLogger().LogDebug("push");
		boss->timer = 0.5;
	}
}
void Boss1::Boss1_State_SummonFire::TestForExit(GameObject* object) {
	Boss1* boss = static_cast<Boss1*>(object);
	if (boss->GetGOComponent<PROJECT::Sprite>()->IsAnimationDone() == true) { // boss->fires.size() == 5
		boss->ChangeState(&boss->stateWalking);
	}
}


void Boss1::Boss1_State_Dead::Enter(GameObject* object) {
	Boss1* boss = static_cast<Boss1*>(object);
	boss->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Boss1_Anim::dead));
	boss->standingOnObject = nullptr;
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>()->RemoveUI(boss);
	boss->wisps.clear();

	if (boss->GetGOComponent<PROJECT::RectCollision>() != nullptr)
		boss->RemoveGOComponent<PROJECT::RectCollision>();

}
void Boss1::Boss1_State_Dead::Update(GameObject* object, double dt) {
}
void Boss1::Boss1_State_Dead::TestForExit(GameObject* object) {
}



void Boss1::Boss1_State_Falling::Enter(GameObject* object) {
	Engine::GetLogger().LogDebug("falling");
}
void Boss1::Boss1_State_Falling::Update(GameObject* object, double dt) {
	Boss1* boss = static_cast<Boss1*>(object);
	if (boss->GetVelocity().y > -50)
		boss->UpdateVelocity({ 0, -Engine::GetGameStateManager().GetGSMComponents().GetComponent<Gravity>()->GetValue() * dt });
}
void Boss1::Boss1_State_Falling::TestForExit(GameObject* object) {
	Boss1* boss = static_cast<Boss1*>(object);
	if (boss->standingOnObject != nullptr) {
		boss->SetVelocity({ 0, 0 });
		boss->ChangeState(&boss->stateWaiting);
	}
}


void Boss1::Boss1_State_Strike::Enter(GameObject* object) {
	Boss1* boss = static_cast<Boss1*>(object);
	boss->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Boss1_Anim::strike));
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->Cast("Boss1 Strike", boss);
}
void Boss1::Boss1_State_Strike::Update(GameObject*, double) {
}
void Boss1::Boss1_State_Strike::TestForExit(GameObject* object) {
	Boss1* boss = static_cast<Boss1*>(object);
	if (boss->GetGOComponent<PROJECT::Sprite>()->IsAnimationDone() == true) {
		boss->ChangeState(&boss->stateWaiting);
	}
}
