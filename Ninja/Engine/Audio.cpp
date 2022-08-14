#include "Audio.h"
#include "Engine.h"


void PROJECT::Audio::EffectBarUpdate(float input)
{
	if(input == -10 && effectSound > 0 || input == 10 && effectSound < 100)
	{
        effectSound += input;
	}
}

void PROJECT::Audio::BGMBarUpdate(float input)
{
    if (input == -10 && bgmSound > 0 || input == 10 && bgmSound < 100)
    {
        bgmSound += input;
        nowPlayBGM->setVolume(bgmSound);
    }
}

void PROJECT::Audio::LoadSound([[maybe_unused]] const std::string& file_path)
{
    SoundBuffers.emplace_back();
    sf::SoundBuffer& buffer = SoundBuffers.back();
    if(!buffer.loadFromFile(file_path))
    {
        Engine::GetLogger().LogError("Failed to Load");
    }

}

void PROJECT::Audio::PlaySound([[maybe_unused]] int index)
{
    for (auto& sound : Sounds)
    {
        if (sound.getStatus() != sf::SoundSource::Playing)
        {
            sound.setBuffer(SoundBuffers[index]);
            sound.setVolume(effectSound);
            sound.play();
            return;
        }
    }
    Sounds.emplace_back(SoundBuffers[index]);
    Sounds.back().setVolume(effectSound);
    Sounds.back().play();
}

void PROJECT::Audio::PlayBGM([[maybe_unused]] sf::Music* input)
{
	if(nowPlayBGM != input)
	{
        if (nowPlayBGM != nullptr)
        {
            nowPlayBGM->stop();
        }
        nowPlayBGM = input;
        nowPlayBGM->setLoop(true);
        nowPlayBGM->setVolume(bgmSound);
        nowPlayBGM->play();
	}
}

void PROJECT::Audio::Setup()
{
	if (!BGMMainMenu.openFromFile("assets/mapData/BGMMainMenu.ogg"))
    {
        Engine::GetLogger().LogError("Failed to load the music file: assets/mapData/BGMMainMenu.ogg");
    }
    if (!BGMVillage.openFromFile("assets/mapData/BGMVillage.ogg"))
    {
        Engine::GetLogger().LogError("Failed to load the music file: assets/mapData/BGMVillage.ogg");
    }
    LoadSound("assets/Player/Walk1.ogg");
    LoadSound("assets/Player/Walk2.ogg");
    LoadSound("assets/Player/Jump.ogg");
    LoadSound("assets/Player/Land.ogg");
    LoadSound("assets/Player/Attack.ogg");
	
}
