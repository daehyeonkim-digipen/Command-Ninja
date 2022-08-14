/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Engine.cpp
Purpose: This is the service provider for our games engine services
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include "Engine.h"

Engine::Engine() : frameCount(0), lastTick(std::chrono::system_clock::now()), ElapsedTime(0),
#ifdef _DEBUG				
logger(PROJECT::Logger::Severity::Debug, true, lastTick)
#else 						
logger(PROJECT::Logger::Severity::Event, false, lastTick)
#endif
{}

Engine::~Engine() {}

void Engine::Init(std::string windowName) {
	logger.LogEvent("Engine Init");
	window.Init(windowName);
	audio.Setup();
	fpsCalcTime = lastTick;

	int seed = static_cast<unsigned int>(time(NULL));
	srand(seed);
	Engine::GetLogger().LogEvent("Seed = " + std::to_string(seed));
}

void Engine::Shutdown() {
	logger.LogEvent("Engine Shutdown");
}

void Engine::Update() {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	double dt = std::chrono::duration<double>(now - lastTick).count();
	ElapsedTime += dt;

#ifdef _DEBUG
	if (dt > 2 / Engine::Target_FPS) {
		dt = 1 / Engine::Target_FPS;
		Engine::GetLogger().LogEvent("Long Frame detected!");
	}
#endif

	if (dt >= 1 / Engine::Target_FPS) 	{
		logger.LogVerbose("Engine Update");
		lastTick = now;

		if (++frameCount >= Engine::FPS_IntervalFrameCount) {
			double actualTime = std::chrono::duration<double>((now - fpsCalcTime)).count();
			logger.LogEvent("FPS:  " + std::to_string(frameCount / actualTime));
			frameCount = 0;
			fpsCalcTime = now;
		}
		gameStateManager.Update(dt);
		input.Update();
		window.Update();
	}
}

bool Engine::HasGameEnded() {
	return gameStateManager.HasGameEnded();
}

void Engine::AddSpriteFont(const std::filesystem::path& fileName) {
	fonts.push_back(PROJECT::SpriteFont(fileName));
}