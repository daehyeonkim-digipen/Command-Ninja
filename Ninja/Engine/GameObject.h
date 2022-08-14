/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/14/2021
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include "Vec2.h"	
#include "TransformMatrix.h"
#include "ComponentManager.h"

enum class GameObjectType;
class NPCChatBox;

namespace PROJECT {
	class Component;

	class GameObject {
		friend class Sprite;
	public:
		GameObject();
		GameObject(math::vec2 position);
		GameObject(math::vec2 position, double rotation, math::vec2 scale);
		virtual ~GameObject();
		void Destroy() { destroyObject = true; }
		bool ShouldDestroy() { return destroyObject; }

		virtual void Update(double dt);
		virtual void Draw(math::TransformMatrix displayMatrix);
		virtual GameObjectType GetObjectType() = 0;
		virtual std::string GetObjectTypeName() = 0;
		virtual bool CanCollideWith(GameObjectType objectBType);
		bool DoesCollideWith(GameObject* objectB);
		bool DoesCollideWith(math::vec2 point);
		virtual void ResolveCollision(GameObject*);

		const math::vec2& GetPosition() const;
		const math::vec2& GetVelocity() const;
		const math::vec2& GetScale() const;
		double GetRotation() const;
		const math::TransformMatrix& GetMatrix();

		void SetPosition(math::vec2 DEBUG_NEWPosition);
		void SetVelocity(math::vec2 DEBUG_NEWVelocity);
		void SetSpeedFactor(math::vec2 scale);
		void SetFloat();

		template<typename T>
		T* GetGOComponent() { return components.GetComponent<T>(); }

		struct Attributes {
			int maxHp = 100;
			int maxStamina = 100;

			int Hp = maxHp;
			int Stamina = maxStamina;

			int Atk = 10;
		} attributes;

		bool IsFloat() { return standingOnObject == nullptr; }
		bool IsDead() { return hasCounted == false && attributes.Hp == 0; };
		bool IsInvulnerable() { return isInvulnerable; };

		void SetCount() { hasCounted = true; }
		void SetInvulnerable(bool value) { isInvulnerable = value; }
	protected:
		void AddGOComponent(Component* component) { components.AddComponent(component); }
		void UpdateGOComponents(double dt) { components.UpdateAll(dt); }
		void ClearGOComponents() { components.Clear(); }
		template<typename T>
		void RemoveGOComponent() { components.RemoveComponent<T>(); }

		class State {
		public:
			virtual void Enter(GameObject* object) = 0;
			virtual void Update(GameObject* object, double dt) = 0;
			virtual void TestForExit(GameObject* object) = 0;
			virtual std::string GetName() = 0;
		};
		class State_Nothing : public State {
		public:
			void Enter(GameObject*) override {}
			void Update(GameObject*, double) override {}
			void TestForExit(GameObject*) override {}
			std::string GetName() override { return ""; }
		};
		State_Nothing state_nothing;

		void UpdatePosition(math::vec2 addPosition);
		void UpdateVelocity(math::vec2 DEBUG_NEWVelocity);
		void SetScale(math::vec2 DEBUG_NEWScale);
		void SetRotation(double DEBUG_NEWRotationAmount);
		void UpdateRotation(double adjustRotation);
		void ChangeState(State* DEBUG_NEWState);

		State* currState;
		math::vec2 screenMousePosition;
		GameObject* standingOnObject;

	private:
		math::TransformMatrix objectMatrix;
		bool updateMatrix;

		double rotation;
		math::vec2 speedFactor;
		math::vec2 scale;
		math::vec2 position;
		math::vec2 velocity;
		ComponentManager components;
		bool destroyObject;
		bool hasCounted;
		bool isInvulnerable;
	};
};