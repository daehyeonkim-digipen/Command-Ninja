#pragma once
#include <SFML/Audio.hpp>

namespace PROJECT
{
    struct Audio
    {
    public:
        std::vector<sf::SoundBuffer>  SoundBuffers{};
        std::vector<sf::Sound>        Sounds{};
        sf::Music                     BGMMainMenu;
        sf::Music                     BGMVillage;
        sf::Music* nowPlayBGM;

        void EffectBarUpdate(float input);
    	void BGMBarUpdate(float input);

        void LoadSound(const std::string& file_path);
        void PlaySound(int index);
        void PlayBGM(sf::Music* input);
        void Setup();
        //void barUpdate();
        float& getEffectSound() { return effectSound; }
        float& getBGMSound() { return bgmSound; }
        enum class Effects
        {
	        Walk1,
        	Walk2,
        	Jump,
        	Land,
        	Attack,
        	Count
        };
    private:
        float effectSound = 50;
        float bgmSound = 50;
    };
}
