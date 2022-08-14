#include <doodle/drawing.hpp>
#include "../Engine/Engine.h"	//GetGameStateManager GetSpriteFont
#include "../Engine/TransformMatrix.h"
#include "Screens.h"
#include "MainMenu.h"
#include "Fonts.h"




MainMenu::OptionData MainMenu::optionsData[static_cast<int>(MainMenu::Options::Count)] = {
	{"New Game", {0.5, 0.45}, {} },
	{"Settings", {0.5, 0.325}, {} },
	{"Quit", {0.5, 0.2}, {} },
};

MainMenu::MainMenu() : selectedIndex(0), upKey(PROJECT::InputKey::Keyboard::Up),
							downKey(PROJECT::InputKey::Keyboard::Down), selectKey(PROJECT::InputKey::Keyboard::Enter),
							ingameStartedCheck(false) {
}

void MainMenu::Load() {
	title = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture("Sub Project", 0xFFFFFFFF, true);
	ingameStartedCheck = false;
	for (OptionData& data : optionsData) {
		RenderOption(data, false);
	}
	selectedIndex = static_cast<int>(Options::NewGame);
	RenderOption(optionsData[selectedIndex], true);
	Engine::GetAudio().PlayBGM(&Engine::GetAudio().BGMMainMenu);
	Engine::GetGameStateManager().GetGSMComponents().Clear();
}

void MainMenu::Update(double) {
	if (upKey.IsKeyReleased() == true) {
		RenderOption(optionsData[selectedIndex], false);
		if (selectedIndex != 0) {
			selectedIndex--;
		}
		RenderOption(optionsData[selectedIndex], true);
	}
	if (downKey.IsKeyReleased() == true) {
		RenderOption(optionsData[selectedIndex], false);
		if (selectedIndex != static_cast<int>(Options::Count) -1) {
			selectedIndex++;
		}
		RenderOption(optionsData[selectedIndex], true);
	}
	if (selectKey.IsKeyReleased() == true) {
		switch (static_cast<Options>(selectedIndex)) {
		case Options::NewGame:
			Engine::Instance().GetGameStateManager().SetNextState(static_cast<int>(Screens::Village));
			ingameStartedCheck = true;
			break;
		case Options::Settings:
			Engine::Instance().GetGameStateManager().SetNextState(static_cast<int>(Screens::Settings));
			break;
		case Options::Quit:
			Engine::GetGameStateManager().Shutdown();
			break;
		}
	}
}

void MainMenu::Unload() {
	if (Engine::GetGameStateManager().HasNextEnded() == false && ingameStartedCheck == true) {
		Map::load_Map();
	}
}

void MainMenu::Draw() {
	Engine::GetWindow().Clear(0x3399DAFF);
	math::ivec2 winSize = Engine::GetWindow().GetSize();

	math::ivec2 position = math::ivec2{ winSize.x / 2 - title.GetSize().x + 75, winSize.y / 2 + 130 };
	title.Draw(math::TranslateMatrix(position) * math::ScaleMatrix(math::vec2{ 1.5, 1.5 }));

	for (OptionData& data : optionsData) {
		position = { static_cast<int>(winSize.x * data.positionPercent.x),
						static_cast<int>(winSize.y * data.positionPercent.y) };
		data.texture.Draw(math::TranslateMatrix(position - data.texture.GetSize() / 2));
	}
}

void MainMenu::RenderOption(OptionData& data, bool isHighlighted) {
	data.texture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture(
								data.text, (isHighlighted == true) ? MainMenu::onColor : MainMenu::offColor, true);
}