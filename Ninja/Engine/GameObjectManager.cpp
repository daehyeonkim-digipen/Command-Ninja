/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectManager.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/14/2021
-----------------------------------------------------------------*/
#include "Engine.h"        //LogEvent
#include "TransformMatrix.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Collision.h"

void PROJECT::GameObjectManager::Add(GameObject* obj) {
	gameObjects.push_back(obj);
}

PROJECT::GameObjectManager::~GameObjectManager() {
	for (GameObject* obj : gameObjects) {
			delete obj;
	}
	gameObjects.clear();
}

void PROJECT::GameObjectManager::Update(double dt) {
	std::vector<GameObject*> destroyList;
	for (GameObject* object : gameObjects) { 
		object->Update(dt); 
		if (object->ShouldDestroy() == true) {
			destroyList.push_back(object);
		}
	}
	for (GameObject* object : destroyList) {
		gameObjects.remove(object);
		delete object;
	}
}

void PROJECT::GameObjectManager::DrawAll(math::TransformMatrix& cameraMatrix) {
	for (GameObject* object : gameObjects) { 
		object->Draw(cameraMatrix);
	} 
}

void PROJECT::GameObjectManager::CollideTest() {
	for (GameObject* objectA : gameObjects) {
		for (GameObject* objectB : gameObjects) {
			if (objectA->IsInvulnerable() == true)
				continue;

			if (objectA != objectB && objectA->CanCollideWith(objectB->GetObjectType()) && objectB->GetGOComponent<PROJECT::Collision>() != nullptr) {
				if (objectA->DoesCollideWith(objectB)) {
  					Engine::GetLogger().LogEvent("Collision Detected: " + objectA->GetObjectTypeName() + " and " + objectB->GetObjectTypeName());
					objectA->ResolveCollision(objectB);
				}
			}
		}
	}
}
