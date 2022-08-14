#include "../Engine/Engine.h"
#include "../Engine/Sprite.h"
#include "../Engine/Camera.h"

#include "Fonts.h"
#include "PopUpDamage.h"

PopUpDamage::PopUpDamage(math::vec2 pos, int damageAmount)
	: UI(nullptr, UIType::PopUpDamage), lifeTime(1)
{
	SetPosition(pos);
	limitYPosition = pos.y + 40;
	damageFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(std::to_string(damageAmount), 0xFFFFFFFF, true);
	velocity = { 0, 900 };
}

void PopUpDamage::Update(double dt) {
	if (lifeTime > 0)
		lifeTime -= dt;
	else
		UIDestroy();
	
	if (GetPosition().y < limitYPosition)
		SetPosition(GetPosition() + velocity * dt); // updating position
}

void PopUpDamage::Draw() {
	math::vec2 cameraPosition = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>()->GetPosition();
	damageFont.Draw(math::TranslateMatrix{ GetPosition() - cameraPosition + math::vec2{ -damageFont.GetSize().x * 0.6 / 2.0, 0 } } *math::ScaleMatrix{ math::vec2{ 0.6, 0.6 } });
}

void PlayerTakenDamage::Draw() {
	math::vec2 cameraPosition = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>()->GetPosition();
	math::TransformMatrix matrix = math::TranslateMatrix{ GetPosition() - cameraPosition + math::vec2{ -damageFont.GetSize().x * 0.6 / 2.0, 0 } } *math::ScaleMatrix{ math::vec2{ 0.6, 0.6 } };
	damageFont.Draw(matrix, 230, 100, 230);
}
