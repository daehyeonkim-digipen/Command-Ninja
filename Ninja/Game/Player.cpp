
#include "../Engine/Engine.h"
#include "../Engine/Sprite.h"
#include "../Engine/Animation.h"
#include "../Engine/Collision.h"
#include "../Engine/UIManager.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/SkillManager.h"

#include "PopUpDamage.h"
#include "GameSkills.h"
#include "SkillWindow.h"
#include "QuestWindow.h"
#include "HitBox.h"
#include "Gravity.h"
#include "Fonts.h"
#include "Quest.h"
#include "Bar.h"
#include "Gold.h"
#include "UI.h"
#include "Player.h"

Player::Player(math::vec2 startPos)
	: GameObject(startPos), overridingItem(nullptr),
	CastKey(GameKey::Z), attackKey(GameKey::X), jumpKey(GameKey::C), collectKey(GameKey::Down),
	moveLeftKey(GameKey::Left), moveRightKey(GameKey::Right),
	signKey1(GameKey::A), signKey2(GameKey::S), signKey3(GameKey::D), signKey4(GameKey::F),
	attackTime(0)
{
	AddGOComponent(DEBUG_NEW PROJECT::Sprite("assets/Player/Player.spt", this));
	AddGOComponent(DEBUG_NEW Gold());
	ChangeState(&stateIdle);

	ResetLevel();

	PROJECT::UIManager* UIM = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>();
	UIM->AddUI(DEBUG_NEW HPBar({ 90, 750 }, UIType::PlayerBar, "assets/UI/ui_hp_empty.png", this));
	UIM->AddUI(DEBUG_NEW StaminaBar({ 110, 725 }, UIType::PlayerBar, "assets/UI/ui_stamina_empty.png", this));

	UIM->AddUI(DEBUG_NEW SkillWindow(GameKey::K, this));
	UIM->AddUI(DEBUG_NEW QuestWindow(GameKey::I, this));

	Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->Unlock("Basic Attack");
}

Player::~Player() {
	// Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>()->RemoveUI(this);
}

void Player::ResetLevel() {
	attributes.Atk = 10;
	attributes.Hp = attributes.maxHp = 100;
}

void Player::Update(double dt) {
	if (IsDead() == true)
		return;

	GameObject::Update(dt);
	if (Engine::GetInput().IsKeyPressed(PROJECT::InputKey::Keyboard::H)) {
		attributes.Hp = attributes.maxHp;
	}
	if (Engine::GetInput().IsKeyPressed(PROJECT::InputKey::Keyboard::G)) {
		GetGOComponent<Gold>()->Add(100);
		Engine::GetLogger().LogDebug("Gold earned! current Gold : " + std::to_string(GetGOComponent<Gold>()->GetValue()));
	}

	if (Engine::GetInput().IsKeyDown(PROJECT::InputKey::Keyboard::Ctrl)) {
		UpdateSignCasting();
	}
	else if (Engine::GetInput().IsKeyReleased(PROJECT::InputKey::Keyboard::Ctrl)) {
		if (signCommand.empty() == false) {
			Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->Unlock(signCommand);
			signCommand.clear();
		}
	}


	if (overridingItem != nullptr && DoesCollideWith(overridingItem) == true && collectKey.IsKeyPressed() == true) {
		overridingItem->ResolveCollision(this);

		overridingItem = nullptr;
	}

	0 <= attackMilliSecond && attackMilliSecond < Player::attackms ? attackMilliSecond++ : attackMilliSecond = -1;
	if (attackMilliSecond == -1) attackTime = 0;
	// Engine::GetLogger().LogDebug(std::to_string(attackMilliSecond));

	UpdateStaminaRegen(dt);

	if (quest.IsAvailable()) {
		if (quest.IsQuestDone() == false)
			quest.Update(dt);
	}

	if (IsDead())
		SetVelocity({ 0, 0 });
}

void Player::Draw(math::TransformMatrix displayMatrix) {
	GameObject::Draw(displayMatrix);

	if (signCommand.empty() == false) {
		PROJECT::Texture commandFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(signCommand, 0xFFFFFFFF, true);
		commandFont.Draw(displayMatrix * math::TranslateMatrix{ GetPosition() + math::vec2{ -commandFont.GetSize().x / 2.0, 80 } });
	}

	if (IsInvulnerable() == true) {
		PROJECT::Texture commandFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture("V", 0xFFFFFFFF, true);
		commandFont.Draw(displayMatrix * math::TranslateMatrix{ GetPosition() + math::vec2{ -commandFont.GetSize().x / 2.0, 30 } });
	}
}

bool Player::CanCollideWith(GameObjectType objectType) {
	switch (objectType)
	{
	case GameObjectType::OniBoss: [[fallthrough]];
	case GameObjectType::Particle: [[fallthrough]];
	case GameObjectType::NPC: [[fallthrough]];
	case GameObjectType::Portal:
		return false;
	}

	return true;
}

void Player::ResolveCollision(GameObject* objectA)
{
	math::rect2 collideRect = objectA->GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect();
	math::rect2 playerRect = GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect();

	switch (objectA->GetObjectType())
	{
	case GameObjectType::Floor:
		if (GetPosition().x < collideRect.Right() && GetPosition().x > collideRect.Left())
		{
			if (currState == &stateFalling || currState == &stateDamaged || currState == &stateAttacking || currState == &stateCasting) {
				if (playerRect.Top() > collideRect.Top() && objectA->DoesCollideWith(GetPosition())) {
					standingOnObject = objectA;
					SetPosition({ GetPosition().x, collideRect.Top() });
					SetVelocity({ GetVelocity().x, 0 });
					currState->TestForExit(this);
					return;
				}
			}

			if (currState == &stateJumping) {
				SetVelocity({ GetVelocity().x, 0 });
				SetPosition({ GetPosition().x, collideRect.Bottom() - playerRect.Size().y });
				currState->TestForExit(this);
			}
		}
		else
		{
			if (playerRect.Left() < collideRect.Left()) {
				UpdatePosition(math::vec2{ (collideRect.Left() - playerRect.Right()), 0.0 });
				SetVelocity({ 0, GetVelocity().y });
			}
			else {
				UpdatePosition(math::vec2{ (collideRect.Right() - playerRect.Left()), 0.0 });
				SetVelocity({ 0, GetVelocity().y });
			}
		}
		break;
	case GameObjectType::OniWisp: [[fallthrough]];
	case GameObjectType::DullOni: [[fallthrough]];
	case GameObjectType::AngryOni: [[fallthrough]];
		//case GameObjectType::OniBoss: [[fallthrough]];
	case GameObjectType::Enemy:
		if (currState != &stateDamaged) {
			signCommand.clear();
			if (attributes.Hp > 0) {
				attributes.Hp -= objectA->attributes.Atk;
				if (attributes.Hp < 0)
					attributes.Hp = 0;
			}
			GetPosition().x < objectA->GetPosition().x ? SetVelocity({ -300, 130 }) : SetVelocity({ 300, 130 });
			Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>()->AddUI(
				DEBUG_NEW PlayerTakenDamage(GetPosition() + math::vec2{ 0, (double)GetGOComponent<PROJECT::Sprite>()->GetFrameSize().y * 0.8 }, objectA->attributes.Atk));
			ChangeState(&stateDamaged);
		}

		break;
	case GameObjectType::Scroll:
		overridingItem = objectA;
		break;
	case GameObjectType::Hitbox:
		if (GetObjectType() == static_cast<HitBox*>(objectA)->GetCasterObjectType())
			break;
		if (currState != &stateDamaged) {
			if (attributes.Hp > 0) {
				attributes.Hp -= static_cast<HitBox*>(objectA)->GetDamage();
				if (attributes.Hp < 0)
					attributes.Hp = 0;
			}

			Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>()->AddUI(
				DEBUG_NEW PlayerTakenDamage(GetPosition() + math::vec2{ 0, (double)GetGOComponent<PROJECT::Sprite>()->GetFrameSize().y * 0.8 }, static_cast<HitBox*>(objectA)->GetDamage()));
			ChangeState(&stateDamaged);
		}

		break;
	default:
		break;
	}

}

void Player::TakeQuest(Quest* quest_) {
	quest = *quest_;
}
void Player::ResetQuest() {
	quest = Quest();
}

void Player::UpdateXVelocity(double dt, bool canMove)
{
	if (canMove == true && moveLeftKey.IsKeyDown() == true) {
		UpdateVelocity({ -Player::xAccel * dt, 0 });		// apply acceleration
		if (GetVelocity().x < -Player::maxXVelocity) {
			SetVelocity({ -Player::maxXVelocity, GetVelocity().y });
		}
		SetScale({ -1, 1 });
	}
	else if (canMove == true && moveRightKey.IsKeyDown() == true) {
		UpdateVelocity({ Player::xAccel * dt, 0 });		// apply acceleration
		if (GetVelocity().x > Player::maxXVelocity) {
			SetVelocity({ Player::maxXVelocity, GetVelocity().y });
		}
		SetScale({ 1, 1 });
	}
	else {
		// no key is down, need to apply drag to slow down
		double xDragDt = Player::xDrag * dt;
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
}

void Player::UpdateStaminaRegen(double dt) {
	if (currState == &stateCasting)
		dt *= castingSlowAmount; // static stamina regen

	if (attributes.Stamina < attributes.maxStamina) {
		staminaRegenTimer -= dt;
	}

	if (staminaRegenTimer <= 0) {
		staminaRegenTimer = staminaRegenTime;
		if (attributes.Stamina < attributes.maxStamina) {
			attributes.Stamina += staminaRegenAmount;
		}
	}
}

void Player::UpdateSignCasting() {
	if (signKey1.IsKeyPressed() == true) {
		signCommand += "A";
	}
	if (signKey2.IsKeyPressed() == true) {
		signCommand += "S";
	}
	if (signKey3.IsKeyPressed() == true) {
		signCommand += "D";
	}
	if (signKey4.IsKeyPressed() == true) {
		signCommand += "F";
	}
}

void Player::alteration(std::string path) {
	RemoveGOComponent<PROJECT::Sprite>();
	AddGOComponent(DEBUG_NEW PROJECT::Sprite(path, this));
	currState = &stateIdle;
}

void Player::Player_State_Idle::Enter(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Player_Anim::Idle));
	// player->SetVelocity({ 0, 0 });
}

void Player::Player_State_Idle::Update(GameObject*, double) {}

void Player::Player_State_Idle::TestForExit(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	if (player->CastKey.IsKeyReleased() == true) {
		player->ChangeState(&player->stateCasting);
	}
	else if (player->standingOnObject == nullptr) {
		player->ChangeState(&player->stateFalling);
	}
	else if (player->moveLeftKey.IsKeyDown() == true || player->moveRightKey.IsKeyDown() == true) {
		player->ChangeState(&player->stateRunning);
	}
	else if (player->jumpKey.IsKeyDown() == true) {
		player->ChangeState(&player->stateJumping);
	}
	else if (player->attackKey.IsKeyPressed() == true) {
		player->ChangeState(&player->stateAttacking);
	}
}

void Player::Player_State_Running::Enter(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Player_Anim::Run));
}

void Player::Player_State_Running::Update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	player->UpdateXVelocity(dt, true);
	int FootstepAnim = player->GetGOComponent<PROJECT::Sprite>()->GetAnim()->GetDisplayFrame();
	if ((FootstepAnim == 9 || FootstepAnim == 5) && StepSoundSwitch == false)
	{
		if (rand() % 2 == 0)
		{
			Engine::GetAudio().PlaySound(static_cast<int>(PROJECT::Audio::Effects::Walk1));
			StepSoundSwitch = true;
		}
		else
		{
			Engine::GetAudio().PlaySound(static_cast<int>(PROJECT::Audio::Effects::Walk2));
			StepSoundSwitch = true;
		}
	}
	else if (FootstepAnim != 9 && FootstepAnim != 5 && StepSoundSwitch == true)
	{
		StepSoundSwitch = false;
	}
}

void Player::Player_State_Running::TestForExit(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	if (player->jumpKey.IsKeyDown() == true) {
		player->ChangeState(&player->stateJumping);
	}
	else if (player->attackKey.IsKeyPressed() == true) {
		player->ChangeState(&player->stateAttacking);
	}
	else if (player->CastKey.IsKeyReleased() == true) {
		player->ChangeState(&player->stateCasting);
	}
	else if (player->GetVelocity().x == 0) {
		player->ChangeState(&player->stateIdle);
	}
	else if (player->standingOnObject != nullptr && player->standingOnObject->DoesCollideWith(player->GetPosition()) == false) {
		player->standingOnObject = nullptr;
		player->ChangeState(&player->stateFalling);
	}
}

void Player::Player_State_Jumping::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Player_Anim::Jump));
	player->SetVelocity({ player->GetVelocity().x, Player::jumpVelocity });
	player->standingOnObject = nullptr;
	Engine::GetAudio().PlaySound(static_cast<int>(PROJECT::Audio::Effects::Jump));
}

void Player::Player_State_Jumping::Update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	player->UpdateVelocity({ 0, -Engine::GetGameStateManager().GetGSMComponents().GetComponent<Gravity>()->GetValue() * dt });
	player->UpdateXVelocity(dt, true);
}

void Player::Player_State_Jumping::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->attackKey.IsKeyPressed() == true) {
		player->ChangeState(&player->stateAttacking);
	}
	else if (player->CastKey.IsKeyReleased() == true) {
		player->ChangeState(&player->stateCasting);
	}
	else if (player->GetVelocity().y <= 0) {
		player->ChangeState(&player->stateFalling);
	}

}

void Player::Player_State_Falling::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Player_Anim::Fall));
}
void Player::Player_State_Falling::Update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	player->UpdateVelocity({ 0, -Engine::GetGameStateManager().GetGSMComponents().GetComponent<Gravity>()->GetValue() * dt });
	player->UpdateXVelocity(dt, true);
	//Engine::GetLogger().LogDebug(std::to_string(player->GetVelocity().y));

}
void Player::Player_State_Falling::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->CastKey.IsKeyReleased() == true) {
		player->ChangeState(&player->stateCasting);
	}
	else if (player->attackKey.IsKeyDown() == true) {
		player->ChangeState(&player->stateAttacking);
	}
	if (player->standingOnObject != nullptr) { // if it collides with floor
		Engine::GetAudio().PlaySound(static_cast<int>(PROJECT::Audio::Effects::Land));
		if (player->GetVelocity().x == 0) {
			player->ChangeState(&player->stateIdle);
		}
		else {
			player->ChangeState(&player->stateRunning);
		}
	}
}

void Player::Player_State_Casting::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Player_Anim::Casting));
	player->SetVelocity({ 0, 0 });
}

void Player::Player_State_Casting::Update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	player->UpdateSignCasting();
}

void Player::Player_State_Casting::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->CastKey.IsKeyReleased()) {
		// active skill excute
		Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->Cast(player->signCommand, player);

		player->signCommand.clear();

		if (player->GetVelocity().x == 0) {
			player->ChangeState(&player->stateIdle);
		}
		else if (player->standingOnObject == nullptr) {
			player->ChangeState(&player->stateFalling);
		}
	}
}

void Player::Player_State_Attacking::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Player_Anim::Attack1) + player->attackTime);

	if (player->moveLeftKey.IsKeyDown() == true) player->SetScale({ -1, 1 });
	else if (player->moveRightKey.IsKeyDown() == true) player->SetScale({ 1, 1 });

	player->attackMilliSecond = 0;

	math::vec2 dashVelocity = { 550, 0 };
	switch (player->attackTime) {
	case 0: // first
		break;
	case 1: // second
		player->SetVelocity(dashVelocity * player->GetScale());
		break;
	case 2: // final atk
		break;
	}

	player->attackTime = (player->attackTime + 1) % 2;
	player->standingOnObject = nullptr;

	// Engine::GetLogger().LogDebug(std::to_string(player->attackTime));
	Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->Cast("Basic Attack", player);
	Engine::GetAudio().PlaySound(static_cast<int>(PROJECT::Audio::Effects::Attack));
}
void Player::Player_State_Attacking::Update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);

	if (player->standingOnObject == nullptr)
		player->SetVelocity({ player->GetVelocity().x, -Engine::GetGameStateManager().GetGSMComponents().GetComponent<Gravity>()->GetValue() * dt });

	player->UpdateXVelocity(dt, false);
}
void Player::Player_State_Attacking::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);

	if (20 <= player->attackMilliSecond && player->attackKey.IsKeyPressed() == true) {
		player->ChangeState(&player->stateAttacking);
	}

	if (player->GetGOComponent<PROJECT::Sprite>()->IsAnimationDone() == true) {
		if (player->standingOnObject == nullptr) {
			player->ChangeState(&player->stateFalling);
		}
		else {
			if (player->moveLeftKey.IsKeyDown() == true || player->moveRightKey.IsKeyDown() == true) {
				player->ChangeState(&player->stateRunning);
			}
			else if (player->GetVelocity().x == 0) {
				player->ChangeState(&player->stateIdle);
			}
		}
	}
}

void Player::Player_State_Damaged::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PROJECT::Sprite>()->PlayAnimation(static_cast<int>(Player_Anim::Damaged));
	player->standingOnObject = nullptr;
}
void Player::Player_State_Damaged::Update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	if (player->standingOnObject == nullptr) {
		player->UpdateVelocity({ 0, -Engine::GetGameStateManager().GetGSMComponents().GetComponent<Gravity>()->GetValue() * dt });
	}
	player->SetSpeedFactor({ 0, 1 });
	player->UpdateXVelocity(dt, true);
}
void Player::Player_State_Damaged::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->GetGOComponent<PROJECT::Sprite>()->IsAnimationDone() == true) {
		if (player->standingOnObject != nullptr) {
			player->ChangeState(&player->stateIdle);
		}
		else {
			player->ChangeState(&player->stateFalling);
		}

		player->SetSpeedFactor({ 1, 1 });
	}
}
