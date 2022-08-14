/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/14/2021
-----------------------------------------------------------------*/
#include "Engine.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Sprite.h"
#include "Collision.h"
#include "ShowCollision.h"
#include "UIManager.h"

#include "../Game/UI.h"

PROJECT::GameObject::GameObject() : GameObject({ 0,0 }) {}

PROJECT::GameObject::GameObject(math::vec2 position) : GameObject(position, 0, { 1, 1 }) {}

PROJECT::GameObject::GameObject(math::vec2 position, double rotation, math::vec2 scale)
					: velocity{ 0,0 }, position(position), screenMousePosition(position), updateMatrix(true), speedFactor({ 1, 1 }),
							scale(scale), rotation(rotation), currState(&state_nothing), destroyObject(false), hasCounted(false), isInvulnerable(false), 
						standingOnObject(nullptr)
{
	attributes.Hp = attributes.maxHp;
	attributes.Stamina = attributes.maxStamina;
}

PROJECT::GameObject::~GameObject() {
	// Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>()->RemoveUI(this);
	ClearGOComponents();
}

void PROJECT::GameObject::Update(double dt) {
	if (attributes.Hp <= 0)
		attributes.Hp = 0;
	else if (attributes.Hp > attributes.maxHp)
		attributes.Hp = attributes.maxHp;

	if (attributes.Stamina <= 0)
		attributes.Stamina = 0;
	else if (attributes.Stamina > attributes.maxStamina)
		attributes.Stamina = attributes.maxStamina;

	currState->Update(this, dt);
	if (velocity.x != 0 || velocity.y != 0) {
		UpdatePosition(velocity * speedFactor * dt);
	}
	UpdateGOComponents(dt);
	currState->TestForExit(this);
}

void PROJECT::GameObject::ChangeState(State* DEBUG_NEWState) {
	currState = DEBUG_NEWState;
	currState->Enter(this);
}

void PROJECT::GameObject::Draw(math::TransformMatrix displayMatrix) {
	Sprite* spritePtr = GetGOComponent<Sprite>();
	if (spritePtr != nullptr) {
		spritePtr->Draw(displayMatrix * GetMatrix());
	}
	ShowCollision* showCollisionPtr = Engine::GetGameStateManager().GetGSMComponents().GetComponent<ShowCollision>();
	if (showCollisionPtr != nullptr && showCollisionPtr->IsEnabled() == true) {
		Collision* collisionPtr = GetGOComponent<Collision>();
		if (collisionPtr != nullptr) {
			collisionPtr->Draw(displayMatrix);
		}
	}

	screenMousePosition = Engine::GetInput().GetMousePosition().operator math::vec2() - (displayMatrix * math::vec2(1, 1));
}

const math::vec2& PROJECT::GameObject::GetPosition() const { return position; }
const math::vec2& PROJECT::GameObject::GetVelocity() const { return velocity; }
const math::vec2& PROJECT::GameObject::GetScale() const { return scale; }
double PROJECT::GameObject::GetRotation() const { return rotation; }
const math::TransformMatrix& PROJECT::GameObject::GetMatrix() {
	if (updateMatrix == true) {
		objectMatrix = math::TranslateMatrix(position) * math::RotateMatrix(rotation) * math::ScaleMatrix(scale);
		updateMatrix = false;
	}
	return objectMatrix;
}

void PROJECT::GameObject::SetPosition(math::vec2 DEBUG_NEWPosition) {
	position = DEBUG_NEWPosition;
	updateMatrix = true;
}

void PROJECT::GameObject::SetVelocity(math::vec2 DEBUG_NEWVelocity) {
	velocity = DEBUG_NEWVelocity;
}

void PROJECT::GameObject::SetSpeedFactor(math::vec2 scale) {
	speedFactor = scale;
}

void PROJECT::GameObject::SetFloat() {
	standingOnObject = nullptr;
}

void PROJECT::GameObject::UpdateVelocity(math::vec2 DEBUG_NEWVelocity) {
	velocity += DEBUG_NEWVelocity;
}

void PROJECT::GameObject::UpdatePosition(math::vec2 addPosition) {
	position += addPosition;
	updateMatrix = true;
}

void PROJECT::GameObject::SetScale(math::vec2 DEBUG_NEWScale) {
	scale = DEBUG_NEWScale;
	updateMatrix = true;
}

void PROJECT::GameObject::SetRotation(double DEBUG_NEWRotationAmount) {
	rotation = DEBUG_NEWRotationAmount;
	updateMatrix = true;
}
void PROJECT::GameObject::UpdateRotation(double adjustRotation) {
	rotation += adjustRotation;
	updateMatrix = true;
}

bool PROJECT::GameObject::CanCollideWith(GameObjectType) {
	return false;
}

bool PROJECT::GameObject::DoesCollideWith(PROJECT::GameObject* objectB) {
	return GetGOComponent<Collision>() != nullptr && objectB->GetGOComponent<Collision>() != nullptr 
				&& GetGOComponent<Collision>()->DoesCollideWith(objectB);
}

bool PROJECT::GameObject::DoesCollideWith(math::vec2 point) {
	return GetGOComponent<Collision>() != nullptr && GetGOComponent<Collision>()->DoesCollideWith(point);
}

void PROJECT::GameObject::ResolveCollision(PROJECT::GameObject*) {
	Engine::GetLogger().LogError("Base class collision resolution should not be called");
}