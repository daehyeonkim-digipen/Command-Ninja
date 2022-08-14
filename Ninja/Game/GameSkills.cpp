#include <memory>

#include "../Engine/Engine.h"
#include "../Engine/Collision.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/Sprite.h"
#include "../Engine/ParticleEmitter.h"

#include "GameParticles.h"
#include "HitBox.h"
#include "GameSkills.h"

Skill::Skill(unsigned short cost_, const std::filesystem::path& iconTexturePath)
	: cost(cost_), isAvailable(false), shouldUpdate(false)
{
	icon = Engine::GetTextureManager().Load(iconTexturePath);
}

void Skill::Unlock() {
	isAvailable = true;
	// Engine::GetLogger().LogEvent("Skill Unlocked");
}

void Skill::DrawIcon(math::TransformMatrix displayMatrix) {
	isAvailable ? icon->Draw(displayMatrix) : icon->Draw(displayMatrix, 100, 100, 100);
}

void GameSkill::Slash::Cast(PROJECT::GameObject* caster) {
	HitBox* temp = DEBUG_NEW HitBox(caster->GetPosition(), { 100, 80 }, caster, caster->attributes.Atk);
	temp->SetLifeTime(20);
	Engine::GetGSComponent<PROJECT::GameObjectManager>()->Add(temp);
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<AttackEmitter>()->Emit(1, caster->GetPosition(), { 0, 0 }, { 0, 0 }, 0, caster->GetScale(), 0);
}

void GameSkill::Fire::Cast(PROJECT::GameObject* caster) {
	Engine::GetGSComponent<PROJECT::GameObjectManager>()->Add(DEBUG_NEW HitBox(caster->GetPosition(), { 300, 220 }, caster, 50));
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<FireEmitter>()->Emit(1, caster->GetPosition(), { 0, 0 }, { 0, 0 }, 0, caster->GetScale(), 0);
}


void GameSkill::Dodge::Cast(PROJECT::GameObject* caster_) {
	caster = caster_;
	timer = dodgeTime;
	shouldUpdate = true;
}
void GameSkill::Dodge::Update(double) {
	if (timer == 0) {
		caster->SetInvulnerable(false);
		shouldUpdate = false;
		return;
	}
	else {
		caster->SetInvulnerable(true);
	}
	
	timer--;
}



void GameSkill::Swift::Cast(PROJECT::GameObject* caster_) {
	caster = caster_;
	shouldUpdate = !shouldUpdate;
	shouldUpdate ? caster_->SetSpeedFactor({ 1.5, 1 }) : caster_->SetSpeedFactor({ 1, 1 });
}
void GameSkill::Swift::Update(double dt) {
	timer -= dt;
	if (timer <= 0) {
		timer = 1;
		caster->attributes.Stamina -= staminaPerSecond;
	}

	if (caster->attributes.Stamina < staminaPerSecond) {
		shouldUpdate = false;
		caster->SetSpeedFactor({ 1, 1 });
	}
}


void GameSkill::SkyWalk::Cast(PROJECT::GameObject* caster) {
	caster->SetVelocity(math::vec2{ caster->GetVelocity().x, 400 });
	caster->SetFloat();
}


void GameSkill::Dash::Cast(PROJECT::GameObject* caster) {
	caster->SetVelocity(math::vec2{ 1400, 0 } * caster->GetScale());
	caster->SetFloat();
} 



void GameSkill::Stump::Cast(PROJECT::GameObject* caster_) {
	shouldUpdate = true;
	floatTime = 0.5;
	caster = caster_;
	caster->SetVelocity({ caster->GetVelocity().x, 1200 });
	caster->SetFloat();
}
void GameSkill::Stump::Update(double dt) {
	if (floatTime == 0.5 && caster->GetVelocity().y < 0) {
		caster->SetVelocity({ caster->GetVelocity().x, 0 });
	}

	if (caster->GetVelocity().y == 0) {
		if (0 < floatTime && floatTime <= 0.5) {
			floatTime -= dt;
		}
		if (floatTime < 0) {
			caster->SetVelocity({ caster->GetVelocity().x, -2000 });
			floatTime = 1;
		}
	}

	if (caster->IsFloat() == false && floatTime == 1) {
		Engine::GetLogger().LogDebug("stump!");
		HitBox* box = DEBUG_NEW HitBox(caster->GetPosition() + math::vec2{ -250, 0 } *caster->GetScale(), { 500, 30 }, caster, caster->attributes.Atk);
		box->SetLifeTime(10);
		Engine::GetGSComponent<PROJECT::GameObjectManager>()->Add(box);
		shouldUpdate = false;
	}
}


void GameSkill::BrtualDash::Cast(PROJECT::GameObject* caster_) {
	shouldUpdate = true;
	timer = 2.5;
	caster = caster_;
	caster->SetVelocity(math::vec2( 0, 0 ));
	caster->SetFloat();

	hitbox = DEBUG_NEW HitBox(caster->GetPosition() - math::vec2{ 130, 0 } * caster->GetScale(), caster->GetGOComponent<PROJECT::Sprite>()->GetFrameSize(), caster, caster->attributes.Atk);
	hitbox->SetLifeTime(200);
	Engine::GetGSComponent<PROJECT::GameObjectManager>()->Add(hitbox);
}
void GameSkill::BrtualDash::Update(double dt) {
	if (timer > 0) {
		timer -= dt;
	}
	hitbox->SetPosition(caster->GetPosition() - math::vec2{ 130, 0 } *caster->GetScale());
	if (0.5 < timer && timer <= 1.5) {
		caster->SetVelocity(math::vec2{ -500 * 1.5, 0 } * caster->GetScale());
		// Engine::GetLogger().LogDebug(std::to_string(box->GetPosition().x));
	}
	else if (timer > 1.5) {
		caster->SetVelocity({ 0, 0 });
	}
	else if (timer < 0) {
		shouldUpdate = false;
	}
}


void GameSkill::GroundStrike::Cast(PROJECT::GameObject* caster_) {
	shouldUpdate = true;
	timer = 1;
	caster = caster_;
}
void GameSkill::GroundStrike::Update(double dt) {
	if (timer > 0)
		timer -= dt;

	if (timer <= 0) {
		double damage = (double)caster->attributes.Atk * 1.2;
		Engine::GetGSComponent<PROJECT::GameObjectManager>()->Add(
			DEBUG_NEW HitBox(caster->GetPosition() - math::vec2{ 170, 0 } *caster->GetScale(), { 300, 300 }, caster, (int)damage));
		shouldUpdate = false;
	}
}

