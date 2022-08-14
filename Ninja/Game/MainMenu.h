#pragma once
#include <string>
#include "..\Engine\GameState.h"
#include "..\Engine\Vec2.h"
#include "..\Engine\Input.h"
#include "..\Engine\Texture.h"
#include "Map.h"

class MainMenu : public PROJECT::GameState {
public:
	enum class Options {
		NewGame,
		Settings,
		Quit,
		Count,
	};

	MainMenu();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;

	std::string GetName() override { return "MainMenu"; }
private:
	struct OptionData {
		std::string text;
		math::vec2 positionPercent;
		PROJECT::Texture texture;
	};

	bool ingameStartedCheck;
	
	void RenderOption(OptionData& data, bool isHighlighted);

	PROJECT::Texture title;
	static OptionData optionsData[static_cast<int>(MainMenu::Options::Count)];
	int selectedIndex;


	PROJECT::InputKey upKey;
	PROJECT::InputKey downKey;
	PROJECT::InputKey selectKey;

	static constexpr unsigned int offColor = 0x404040FF;
	static constexpr unsigned int onColor = 0xFFFFFFFF;

}; 

