#pragma once
#include <string>
#include "..\Engine\GameState.h"
#include "..\Engine\Texture.h"
#include "..\Engine\Input.h"
class Settings : public PROJECT::GameState {
public:
	enum class Options {
		EffectSoundScale,
		BGMSoundScale,
		Back,
		Count,
	};
	Settings();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;

	std::string GetName() override { return "Settings"; }
private:
	struct OptionData {
		std::string text;
		math::vec2 positionPercent;
		PROJECT::Texture texture;
	};
	static OptionData optionsData[static_cast<int>(Settings::Options::Count)];
	int selectedIndex;
	void RenderOption(OptionData& data, bool isHighlighted);
	
	PROJECT::Texture EffectSoundScale;
	PROJECT::Texture BGMSoundScale;
	PROJECT::Texture Back;
	
	PROJECT::Texture* EffectDrag;
	PROJECT::Texture* BGMDrag;
	PROJECT::Texture* EffectPad;
	PROJECT::Texture* BGMPad;
	
	PROJECT::InputKey IncreaseKey;
	PROJECT::InputKey DecreaseKey;
	PROJECT::InputKey upKey;
	PROJECT::InputKey downKey;
	PROJECT::InputKey SelectKey;

	int EffectDragX;
	int BGMDragX;
	static constexpr unsigned int offColor = 0x404040FF;
	static constexpr unsigned int onColor = 0xFFFFFFFF;
};
