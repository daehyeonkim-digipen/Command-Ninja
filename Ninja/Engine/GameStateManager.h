/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameStateManager.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#pragma once
#include <vector>
#include "GameState.h"

namespace PROJECT {
	class GameStateManager {
	public:
		GameStateManager();

		void AddGameState(GameState& gameState);
		void Update(double dt);
		void SetNextState(int initState);
		void Shutdown();
		void ReloadState();
		void CollideTest();
		bool ComparePrestate(int stateNum);
		bool HasGameEnded() { return state == State::EXIT; }
		bool HasNextEnded() { return nextGameState == nullptr; }

		void AddGSMComponent(Component* component) { GSMcomponents.AddComponent(component); }
		void UpdateGSMComponents(double dt) { GSMcomponents.UpdateAll(dt); }
		PROJECT::ComponentManager& GetGSMComponents() { return GSMcomponents; };
		void ClearGSMComponent() { GSMcomponents.Clear(); }

		GameState* GetCurrState() { return currGameState; }

		template<typename T>
		T* GetGSComponent() { return currGameState->GetGSComponent<T>(); }
	private:
		enum class State {
			START,
			LOAD,
			UPDATE,
			UNLOAD,
			SHUTDOWN,
			EXIT,
		};
		
		PROJECT::ComponentManager GSMcomponents;
		std::vector<GameState*> gameStates;
		State state;
		GameState* PreGameState;
		GameState* currGameState;
		GameState* nextGameState;
	};
}