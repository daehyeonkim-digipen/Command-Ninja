/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Input.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include "Input.h"
#include "Engine.h"		//GetInput
#include "TransformMatrix.h" // translate Matrix
#include <doodle/drawing.hpp> // drawing

PROJECT::Input::Input() {
	keyDown.resize(static_cast<int>(InputKey::Keyboard::Count));
	wasKeyDown.resize(static_cast<int>(InputKey::Keyboard::Count));
	buttonDown.resize(static_cast<int>(InputMouse::MouseButton::Count));
	wasButtonDown.resize(static_cast<int>(InputMouse::MouseButton::Count));
}

bool PROJECT::Input::IsKeyDown(InputKey::Keyboard key) const {
	return keyDown[static_cast<int>(key)];
}

bool PROJECT::Input::IsKeyReleased(InputKey::Keyboard key) const {
	return keyDown[static_cast<int>(key)] == false && wasKeyDown[static_cast<int>(key)] == true;
}

bool PROJECT::Input::IsKeyPressed(InputKey::Keyboard key) const {
	return keyDown[static_cast<int>(key)] == true && wasKeyDown[static_cast<int>(key)] == false;
}

void PROJECT::Input::SetKeyDown(InputKey::Keyboard key, bool value) {
	keyDown[static_cast<int>(key)] = value;
}


void PROJECT::Input::SetMousePosition(math::ivec2 newPosition) {
	mousePosition = newPosition;
}

void PROJECT::Input::SetPrevMousePosition(math::ivec2 newPosition) {
	previouseMousePosition = newPosition;
}

math::ivec2 PROJECT::Input::GetMousePosition() const {
	return mousePosition;
}

math::ivec2 PROJECT::Input::GetPrevMousePosition() const {
	return previouseMousePosition;
}

math::ivec2 PROJECT::Input::GetMouseDirection() const {
	return mousePosition - previouseMousePosition;
}

bool PROJECT::Input::IsMouseDown(InputMouse::MouseButton button) const {
	return buttonDown[static_cast<int>(button)];
}

bool PROJECT::Input::IsMouseReleased(InputMouse::MouseButton button) const {
	return buttonDown[static_cast<int>(button)] == false && wasButtonDown[static_cast<int>(button)] == true;
}

bool PROJECT::Input::IsMousePressed(InputMouse::MouseButton button) const {
	return buttonDown[static_cast<int>(button)] == true && wasButtonDown[static_cast<int>(button)] == false;
}

void PROJECT::Input::SetMouseDown(InputMouse::MouseButton button, bool value) {
	buttonDown[static_cast<int>(button)] = value;
}


void PROJECT::Input::Update() {
	wasKeyDown = keyDown;
	wasButtonDown = buttonDown;
}

PROJECT::InputKey::InputKey(PROJECT::InputKey::Keyboard button) : button(button) {}

bool PROJECT::InputKey::IsKeyDown() const {
	return Engine::GetInput().IsKeyDown(button);
}

bool PROJECT::InputKey::IsKeyReleased() const {
	return Engine::GetInput().IsKeyReleased(button);
}

bool PROJECT::InputKey::IsKeyPressed() const {
	return Engine::GetInput().IsKeyPressed(button);
}

PROJECT::InputMouse::InputMouse(PROJECT::InputMouse::MouseButton button) : button(button) {}

bool PROJECT::InputMouse::IsMouseDown() const {
	return Engine::GetInput().IsMouseDown(button);
}

bool PROJECT::InputMouse::IsMouseReleased() const {
	return Engine::GetInput().IsMouseReleased(button);
}

bool PROJECT::InputMouse::IsMousePressed() const {
	return Engine::GetInput().IsMousePressed(button);
}