#include "../Engine/GameStateManager.h"
#include "../Engine/Collision.h"
#include "../Engine/SkillManager.h"

#include "Gravity.h"
#include "Scroll.h"

Scroll::Scroll(math::vec2 pos)
	: isFalling(true)
{
	texture = Engine::GetTextureManager().Load("assets/Scrolls/blue.png");
	AddGOComponent(DEBUG_NEW PROJECT::RectCollision({ { texture->GetSize().x / 4, 0 }, { texture->GetSize().x * 3 / 4, texture->GetSize().y / 2 } }, this));
	SetPosition(pos - math::vec2{ texture->GetSize().x / 2.0, 0 });
	ChangeState(&state_nothing);
	SetVelocity({ 0, 500 });
}

void Scroll::Update(double dt) {
	GameObject::Update(dt);

	if (isFalling == true) {
		UpdateVelocity({ 0, -Engine::GetGameStateManager().GetGSMComponents().GetComponent<Gravity>()->GetValue() * dt / 1.5 });
	}
}

void Scroll::Draw(math::TransformMatrix displayMatrix) {
	math::TransformMatrix matrix = displayMatrix * math::TranslateMatrix{ GetPosition() };
	if (isFalling == false) matrix *= math::TranslateMatrix{ math::vec2{ 0, std::sin(Engine::GetElapsedTime() / 2000) * 5 } };
	texture->Draw(matrix);

	GameObject::Draw(displayMatrix);
}

bool Scroll::CanCollideWith(GameObjectType objectType) {
	return objectType == GameObjectType::Floor;
}

void Scroll::ResolveCollision(GameObject* objectA) {
	if (objectA->GetObjectType() == GameObjectType::Floor) {
		math::rect2 collideRect = objectA->GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect();
		math::rect2 rect = GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect();
		if (GetVelocity().y < 0 && rect.Bottom() <= collideRect.Top() && rect.Top() >= collideRect.Top()) {
			isFalling = false;
			SetVelocity({ 0, 0 });
			SetPosition({ GetPosition().x, objectA->GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect().Top() });
		}
	}
	else if (objectA->GetObjectType() == GameObjectType::Player) {
		Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->RandomUnlock();
		Destroy();
	}
}
