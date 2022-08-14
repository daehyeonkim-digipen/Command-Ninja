/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectManager.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/14/2021
-----------------------------------------------------------------*/
#pragma once

#include <list>
#include "GameObject.h"
#include "Component.h"

namespace PROJECT {
	class GameObjectManager : public Component {
	public:
		~GameObjectManager();
		void Add(GameObject* obj);

		void Update(double dt) override;
		void DrawAll(math::TransformMatrix& cameraMatrix);
		void CollideTest();
		std::list<GameObject*> GetGameObjects() { return gameObjects; }
		template<typename T>
		T* GetGameObject() {
			for (GameObject* gameObject : gameObjects) {
				T* ptr = dynamic_cast<T*>(gameObject);
				if (ptr != nullptr) {
					return ptr;
				}
			}
			return nullptr;
		}
		template<typename T>
		void RemoveGameObject() {

			auto lam = [](GameObject* element) {return (dynamic_cast<T*>(element) != nullptr); };

			while (std::find_if(gameObjects.begin(), gameObjects.end(), lam) != gameObjects.end()) {
				auto it = std::find_if(gameObjects.begin(), gameObjects.end(), lam);
				delete* it;
				gameObjects.erase(it);
				}
		}

		const std::list<GameObject*> &Objects() { return gameObjects; }
	private:
		std::list<GameObject*> gameObjects;
	};
}