/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameStateManager.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include "Engine.h"		//logger
#include "Collision.h"		
#include "GameStateManager.h"
#include "GameState.h"
#include "GameObjectManager.h"
#include "../Game/GameObjectTypes.h"

PROJECT::GameStateManager::GameStateManager() : PreGameState(nullptr), currGameState(nullptr), nextGameState(nullptr), state(State::START) {}

void PROJECT::GameStateManager::AddGameState(GameState& gameState) {
	gameStates.push_back(&gameState);
}

void PROJECT::GameStateManager::Update(double dt) {
	switch (state) {
	case State::START:
		if (gameStates.size() == 0) {
			Engine::GetLogger().LogError("No States have been loaded");
			state = State::SHUTDOWN;
		} else {
			nextGameState = gameStates[0];
			state = State::LOAD;
		}
		break;
	case State::LOAD:
		currGameState = nextGameState;
		Engine::GetLogger().LogEvent("Load " + currGameState->GetName());
		currGameState->Load();
		Engine::GetLogger().LogEvent("Load Complete");
		state = State::UPDATE;
		break;
	case State::UPDATE:
		if (currGameState != nextGameState) {
			state = State::UNLOAD;
		} else {
			Engine::GetLogger().LogVerbose("Update " + currGameState->GetName());
			currGameState->Update(dt);
			if (Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>() != nullptr) {
				Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::GameObjectManager>()->CollideTest();
				Engine::GetGSComponent<PROJECT::GameObjectManager>()->CollideTest();
				CollideTest();
			}
			currGameState->Draw();
		}
		break;
	case State::UNLOAD:
		Engine::GetLogger().LogEvent("Unload " + currGameState->GetName());
		currGameState->Unload();
		if (nextGameState == nullptr) {
			state = State::SHUTDOWN;
			break;
		}
		state = State::LOAD;
		break;
	case State::SHUTDOWN:
		state = State::EXIT;
		break;
	case State::EXIT:
		break;
	}
}

void PROJECT::GameStateManager::SetNextState(int initState) {
	PreGameState = nextGameState;
	nextGameState = gameStates[initState];
}

void PROJECT::GameStateManager::ReloadState() {
	state = State::UNLOAD;
}

void PROJECT::GameStateManager::Shutdown() {
	state = State::UNLOAD;
	nextGameState = nullptr;
}


bool  PROJECT::GameStateManager::ComparePrestate(int stateNum) {
	if (PreGameState == gameStates[stateNum]) {
		return true;
	}
	return false;
}


void PROJECT::GameStateManager:: CollideTest() {
	for (GameObject* objectA : GetGSMComponents().GetComponent<PROJECT::GameObjectManager>()->GetGameObjects() ) {
		for (GameObject* objectB : GetGSComponent<PROJECT::GameObjectManager>()->GetGameObjects() ) {
			if (objectA->IsInvulnerable() == true) {
				switch (objectB->GetObjectType())
				{
				case GameObjectType::Floor:
					break;
				default:
					continue;
				}
			}

			if (objectA != objectB && objectA->CanCollideWith(objectB->GetObjectType()) && objectB->GetGOComponent<PROJECT::Collision>() != nullptr) {
				if (objectA->DoesCollideWith(objectB)) {
					Engine::GetLogger().LogEvent("Collision Detected: " + objectA->GetObjectTypeName() + " and " + objectB->GetObjectTypeName());
					objectA->ResolveCollision(objectB);
				}
			}
			if (objectB != objectA && objectB->CanCollideWith(objectA->GetObjectType()) && objectA->GetGOComponent<PROJECT::Collision>() != nullptr) {
				if (objectB->DoesCollideWith(objectA)) {
					Engine::GetLogger().LogEvent("Collision Detected: " + objectB->GetObjectTypeName() + " and " + objectA->GetObjectTypeName());
					objectB->ResolveCollision(objectA);
				}
			}
		}
	}
}