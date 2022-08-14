#pragma once
#include "../Engine/Engine.h"
#include "UI.h"

class Button : public UI {
	using GameMouse = PROJECT::InputMouse::MouseButton;
public:
	Button() : Button({ 0, 0 }, "assets/UI/buttons/sampleButton.png") {};
	Button(math::vec2 pos, std::string filePath_normal);
	//~Button();

	void Draw() override;

	bool DoesClicked();
	bool IsPressing();

	math::ivec2 GetSize() { return GetTexture(0)->GetSize(); }
};