#include "Button.h"
#include "UI.h"

Button::Button(math::vec2 pos, std::string filePath)
	: UI(nullptr, UIType::None)
{	
	SetPosition(pos);

	AddTexture(filePath);
	filePath = filePath.substr(0, filePath.size() - 4);
	AddTexture(filePath + "_mouse_on.png");
	AddTexture(filePath + "_pressing.png");
}

bool Button::DoesClicked() {
	return IsMouseOn() && Engine::GetInput().IsMouseReleased(GameMouse::Left);
}

bool Button::IsPressing() {
	return IsMouseOn() && Engine::GetInput().IsMouseDown(GameMouse::Left);
}

void Button::Draw() {
	int i = 0;
	IsMouseOn() ? (IsPressing() ? i = 2 : i = 1) : i = 0;
	GetTexture(i)->Draw(math::TranslateMatrix{ GetPosition() });
}