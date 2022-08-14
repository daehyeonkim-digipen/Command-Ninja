/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Input_Doodle.cpp
Purpose: Interface between doodle and our engine
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include <doodle/input.hpp>    
#include "Input.h"
#include "Engine.h"			// Logger, Input

PROJECT::InputKey::Keyboard DoodleKeyToPROJECTKey(doodle::KeyboardButtons button) {
	if (button == doodle::KeyboardButtons::Enter) {
		return PROJECT::InputKey::Keyboard::Enter;
	} else if (button == doodle::KeyboardButtons::Escape) {
		return PROJECT::InputKey::Keyboard::Escape;
	} else if (button == doodle::KeyboardButtons::Space) {
		return PROJECT::InputKey::Keyboard::Space;
	} else if (button == doodle::KeyboardButtons::Left) {
		return PROJECT::InputKey::Keyboard::Left;
	} else if (button == doodle::KeyboardButtons::Right) {
		return PROJECT::InputKey::Keyboard::Right;
	} else if (button == doodle::KeyboardButtons::Up) {
		return PROJECT::InputKey::Keyboard::Up;
	} else if (button == doodle::KeyboardButtons::Down) {
		return PROJECT::InputKey::Keyboard::Down;
	} else if (button == doodle::KeyboardButtons::Tilde) {
		return PROJECT::InputKey::Keyboard::Tilde;
	} else if (button == doodle::KeyboardButtons::Control) {
		return PROJECT::InputKey::Keyboard::Ctrl;
	} else if (button >= doodle::KeyboardButtons::A && button <= doodle::KeyboardButtons::Z) {
		int offset = static_cast<int>(button) - static_cast<int>(doodle::KeyboardButtons::A);
		return static_cast<PROJECT::InputKey::Keyboard>(static_cast<int>(PROJECT::InputKey::Keyboard::A) + offset);
	}
	return PROJECT::InputKey::Keyboard::None;
}

PROJECT::InputMouse::MouseButton DoodleMouseToPROJECTMouse(doodle::MouseButtons button) {
	switch (button)
	{
	case doodle::MouseButtons::None:
		return PROJECT::InputMouse::MouseButton::None;
		break;
	case doodle::MouseButtons::Left:
		return PROJECT::InputMouse::MouseButton::Left;
		break;
	case doodle::MouseButtons::Middle:
		return PROJECT::InputMouse::MouseButton::Center;
		break;
	case doodle::MouseButtons::Right:
		return PROJECT::InputMouse::MouseButton::Right;
		break;
	}
	return PROJECT::InputMouse::MouseButton::None;
}

void on_key_pressed(doodle::KeyboardButtons doodleButton) {
	PROJECT::InputKey::Keyboard button = DoodleKeyToPROJECTKey(doodleButton);
	if (button != PROJECT::InputKey::Keyboard::None) {
		// Engine::GetLogger().LogDebug("on_key_pressed");
		Engine::GetInput().SetKeyDown(button, true);
	}
}

void on_key_released(doodle::KeyboardButtons doodleButton) {
	PROJECT::InputKey::Keyboard button = DoodleKeyToPROJECTKey(doodleButton);
	if (button != PROJECT::InputKey::Keyboard::None) {
		// Engine::GetLogger().LogDebug("on_key_released");
		Engine::GetInput().SetKeyDown(button, false);
	}
}

void on_mouse_pressed(doodle::MouseButtons doodleMouseButton) {
	PROJECT::InputMouse::MouseButton button = DoodleMouseToPROJECTMouse(doodleMouseButton);
	if (button != PROJECT::InputMouse::MouseButton::None) {
		Engine::GetInput().SetMouseDown(button, true);
	}
}

void on_mouse_released(doodle::MouseButtons doodleMouseButton) {
	PROJECT::InputMouse::MouseButton button = DoodleMouseToPROJECTMouse(doodleMouseButton);
	if (button != PROJECT::InputMouse::MouseButton::None) {
		Engine::GetInput().SetMouseDown(button, false);
	}
}

void on_mouse_moved(int x, int y) {
	Engine::GetInput().SetMousePosition(math::ivec2{ x, y });
	Engine::GetInput().SetPrevMousePosition(math::ivec2{ doodle::get_previous_mouse_x(), doodle::get_previous_mouse_y() });
}