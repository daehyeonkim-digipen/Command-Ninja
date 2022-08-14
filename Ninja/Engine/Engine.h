/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Engine.h
Purpose: This is the service provider for our games engine services
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#pragma once
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ ) 
#include <chrono>
#include "GameStateManager.h"
#include "Input.h"
#include "Window.h"
#include "Logger.h"
#include "TextureManager.h"
#include "SpriteFont.h"
#include "Audio.h"

class Engine {
public:
    static Engine& Instance() { static Engine instance; return instance; }
    static PROJECT::Logger& GetLogger() { return Instance().logger; };
    static PROJECT::Input& GetInput() { return Instance().input; }
    static PROJECT::Window& GetWindow() { return Instance().window; }
    static PROJECT::GameStateManager& GetGameStateManager() { return Instance().gameStateManager; }
    static PROJECT::TextureManager& GetTextureManager() { return Instance().textureManager; }
    static PROJECT::Audio& GetAudio() { return Instance().audio; }
    static PROJECT::SpriteFont& GetSpriteFont(int index) { return Instance().fonts[index]; }

    static double GetElapsedTime() { return Instance().ElapsedTime; };

    void Init(std::string windowName);
    void Shutdown();
    void Update();
    bool HasGameEnded();

    void AddSpriteFont(const std::filesystem::path& fileName);
    template<typename T>
    static T* GetGSComponent() { return GetGameStateManager().GetGSComponent<T>(); }
private:
    Engine();
    ~Engine();

    std::chrono::system_clock::time_point lastTick;
    std::chrono::system_clock::time_point fpsCalcTime;

    double ElapsedTime;
    int frameCount;

    PROJECT::Logger logger;
    PROJECT::TextureManager textureManager;
    PROJECT::GameStateManager gameStateManager;
    PROJECT::Input input;
    PROJECT::Window window;
    PROJECT::Audio audio;
	
    std::vector<PROJECT::SpriteFont> fonts;

    static constexpr double Target_FPS = 60.0;
    static constexpr int FPS_IntervalSec = 5;
    static constexpr int FPS_IntervalFrameCount = static_cast<int>(FPS_IntervalSec * Target_FPS);
};
