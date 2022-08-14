#pragma once
#include "Portal.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "Fonts.h"

Portal::Portal(math::ivec2 pos, Screens nextscreen, std::string name) : NextScreen(static_cast<int>(nextscreen)),Texture(nullptr), CanGoToNext(false)
, moveNextKey(GameKey::Space)
{
	Texture = Engine::GetTextureManager().Load("assets/Portal/Portal.Png");
	SetPosition(math::vec2(pos));
	AddGOComponent(DEBUG_NEW PROJECT::RectCollision({ {0,0}, Texture->GetSize() }, this));
	nameFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture(name, 0xFFFFFFFF, true);
}

bool Portal::CanCollideWith(GameObjectType objectType) {
	if (objectType == GameObjectType::Player) {
		return true;
	}
	else { return false; }

}
void Portal::ResolveCollision(GameObject* objectA) {
	if (objectA->GetObjectType() == GameObjectType::Player)
	{
		CanGoToNext = true;
	}
}
void Portal::Draw(math::TransformMatrix displayMatrix) {
	GameObject::Draw(displayMatrix);
	Texture->Draw(displayMatrix * math::TranslateMatrix{ GetPosition() });

	if (CanGoToNext == true) {
		nameFont.Draw(displayMatrix * math::TranslateMatrix{ math::vec2{ GetPosition().x + GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect().Size().x / 2 - (nameFont.GetSize().x * 0.4 / 2),
			GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect().Top()} } *math::ScaleMatrix{ math::vec2{ 0.4, 0.4 } });
	}
	
	
}

void Portal::Update(double dt) {
	GameObject::Update(dt);

	if (CanGoToNext == true) {
		if (moveNextKey.IsKeyPressed() == true) {
			Engine::GetGameStateManager().SetNextState(static_cast<int>(NextScreen));
		}
	}

	CanGoToNext = false;
}