#include "Settings.h"
#include "..\Engine\Engine.h"
#include "Fonts.h"
#include "Screens.h"

Settings::OptionData Settings::optionsData[static_cast<int>(Settings::Options::Count)] = {
	{"Effect Sound Scale", {0.5, 0.7}, {} },
	{"BGM Sound Scale", {0.5, 0.45}, {} },
	{"Back", {0.5, 0.2}, {} },
};

Settings::Settings() : selectedIndex(0), EffectDragX(static_cast<int>(Engine::GetWindow().GetSize().x/2)), BGMDragX(static_cast<int>(Engine::GetWindow().GetSize().x / 2)),IncreaseKey(PROJECT::InputKey::Keyboard::Right), DecreaseKey(PROJECT::InputKey::Keyboard::Left), upKey(PROJECT::InputKey::Keyboard::Up), downKey(PROJECT::InputKey::Keyboard::Down), SelectKey(PROJECT::InputKey::Keyboard::Enter), EffectDrag(Engine::GetTextureManager().Load("assets/Settings/drag.png")), BGMDrag(Engine::GetTextureManager().Load("assets/Settings/drag.png")), EffectPad(Engine::GetTextureManager().Load("assets/Settings/pad.png")), BGMPad(Engine::GetTextureManager().Load("assets/Settings/pad.png"))
{
}

void Settings::Load()
{
	for (OptionData& data : optionsData) {
		RenderOption(data, false);
	}
	RenderOption(optionsData[selectedIndex], true);
	selectedIndex = static_cast<int>(Options::EffectSoundScale);
}

void Settings::Update(double)
{
	if (upKey.IsKeyReleased() == true) {
		RenderOption(optionsData[selectedIndex], false);
		if (selectedIndex != 0) {
			selectedIndex--;
		}
		RenderOption(optionsData[selectedIndex], true);
	}
	if (downKey.IsKeyReleased() == true) {
		RenderOption(optionsData[selectedIndex], false);
		if (selectedIndex != static_cast<int>(Options::Count) - 1) {
			selectedIndex++;
		}
		RenderOption(optionsData[selectedIndex], true);
	}
	switch(selectedIndex)
	{
	case (int)Options::EffectSoundScale:
		if (IncreaseKey.IsKeyReleased())
		{
			Engine::GetAudio().EffectBarUpdate(10);
		}
		else if (DecreaseKey.IsKeyReleased())
		{
			Engine::GetAudio().EffectBarUpdate(-10);
		}
		break;
	case (int)Options::BGMSoundScale:
		if (IncreaseKey.IsKeyReleased())
		{
			Engine::GetAudio().BGMBarUpdate(10);
		}
		else if (DecreaseKey.IsKeyReleased())
		{
			Engine::GetAudio().BGMBarUpdate(-10);
		}
		break;
	case (int)Options::Back:
		if (SelectKey.IsKeyReleased())
		{
			Engine::Instance().GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenu));
		}
		break;
	}
	math::ivec2 winSize = Engine::GetWindow().GetSize();
	EffectDragX = winSize.x / 2 + (Engine::GetAudio().getEffectSound() - 50) * 3;
	BGMDragX = winSize.x / 2 + (Engine::GetAudio().getBGMSound() - 50) * 3;
}

void Settings::Unload()
{
}

void Settings::Draw()
{
	Engine::GetWindow().Clear(0x3399DAFF);
	math::ivec2 winSize = Engine::GetWindow().GetSize();
	math::ivec2 position;

	EffectPad->Draw(math::TranslateMatrix(math::vec2{ winSize.x / 2.0 - EffectPad->GetSize().x / 2, 450 }));
	BGMPad->Draw(math::TranslateMatrix(math::vec2{ winSize.x / 2.0 - EffectPad->GetSize().x / 2, 250 }));
	EffectDrag->Draw(math::TranslateMatrix(math::ivec2{ EffectDragX - EffectDrag->GetSize().x / 2, 450 }));
	BGMDrag->Draw(math::TranslateMatrix(math::ivec2{ BGMDragX - BGMDrag->GetSize().x / 2, 250 }));

	for (OptionData& data : optionsData) {
		position = { static_cast<int>(winSize.x * data.positionPercent.x),
						static_cast<int>(winSize.y * data.positionPercent.y) };
		data.texture.Draw(math::TranslateMatrix(position - data.texture.GetSize() / 2));
	}
}

void Settings::RenderOption(OptionData& data, bool isHighlighted) {
	data.texture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture(
		data.text, (isHighlighted == true) ? Settings::onColor : Settings::offColor, true);
}
