#include "../Engine/Engine.h"
#include "../Engine/Sprite.h"

#include <functional>
#include "Fonts.h"
#include "UI.h"
#include "Player.h"

UI::UI(PROJECT::GameObject* objectT, UIType layer)
	: object(objectT), layerLevel(static_cast<int>(layer)), isVisible(true), shouldDestroyed(false)
{}

UI::~UI() {
	//for (PROJECT::Texture* texture : textures) {
	//	if (texture != nullptr)
	//		delete texture;
	//}
}

void UI::Draw() {
	if (textures.empty() == false) {
		textures.front()->Draw(math::TranslateMatrix{ position });
	}
}

bool UI::IsMouseOn() {
	if (isVisible == false || textures.empty() == true)
		return false;

	return position.x <= Engine::GetInput().GetMousePosition().x && Engine::GetInput().GetMousePosition().x <= position.x + textures.front()->GetSize().x
		&& position.y <= Engine::GetInput().GetMousePosition().y && Engine::GetInput().GetMousePosition().y <= position.y + textures.front()->GetSize().y;
}

void UI::AddTexture(const std::filesystem::path& textureFilePath) {
	textures.push_back(Engine::GetTextureManager().Load(textureFilePath));
}

PROJECT::Texture* UI::GetTexture(int index) {
	if (index >= static_cast<int>(textures.size())) {
		throw std::runtime_error("invalid vector index in UI::GetTexture");
	}

	return textures[index];
}
//PROJECT::GameObject* const UI::GetObject() {
//	return object;
//}
//
//void UI::AddButton(Button* newButton) {
//	buttons.push_back(newButton);
//}
//
//Button* UI::GetButton(int index) {
//	if (index >= static_cast<int>(buttons.size())) {
//		throw std::runtime_error("invalid vector index in UI::GetButton");
//	}
//
//	return buttons[index];
//}


GameWindow::GameWindow(math::vec2 pos, UIType layerLevel, const std::filesystem::path& textureFilePath, PROJECT::InputKey openButton, PROJECT::GameObject* objectT) 
	: UI(objectT, layerLevel), openKey(openButton), isfollowingMouse(false)
{
	textureFilePath.empty() ? AddTexture("assets/UI/board.png") : AddTexture(textureFilePath);

	SetPosition(pos);
	SetVisible(false);
}


void GameWindow::Update(double dt) {
	if (openKey.IsKeyPressed() == true) {
		SetVisible(!IsVisible());
	}

	if (IsVisible() == true)
		UpdateDragging(dt);
}

void GameWindow::UpdateDragging(double) {
	if (IsMouseOnDragableArea() == true && Engine::GetInput().IsMousePressed(GameMouse::Left) == true) {
		isfollowingMouse = true;
	}

	if (Engine::GetInput().IsMouseReleased(GameMouse::Left) == true) {
		isfollowingMouse = false;
	}

	if (isfollowingMouse == true && Engine::GetInput().IsMouseDown(GameMouse::Left) == true) {
		SetPosition(GetPosition() + Engine::GetInput().GetMouseDirection().operator math::vec2());
	}
}

bool GameWindow::IsMouseOnDragableArea() {
	math::vec2 mouse = Engine::GetInput().GetMousePosition().operator math::vec2();

	return GetPosition().x <= mouse.x && mouse.x <= GetPosition().x + GetTexture()->GetSize().x
		&& GetPosition().y + GetTexture()->GetSize().y - 20 <= mouse.y && mouse.y <= GetPosition().y + GetTexture()->GetSize().y;
}