#pragma once
#include "UI.h"
#include "Button.h"
#include "../Engine/Texture.h"
#include "../Engine/Vec2.h"

class SkillWindow : public GameWindow {
	using GameKey = PROJECT::InputKey::Keyboard;
public:
	SkillWindow(PROJECT::InputKey openKey_, PROJECT::GameObject* objectT);

	void Update(double dt) override;
	void Draw() override;
private:
	std::vector<Button> pageButtons;
	Button backButton;

	enum class WindowPageType {
		ATK, DEX, DEF, MAIN, SkillInfo, Count
	};

	std::vector<WindowPageType> windowOrder;
	std::string selectedSkillCommand;
};