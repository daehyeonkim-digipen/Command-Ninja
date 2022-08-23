/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Input.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#pragma once
#include "Vec2.h"
#include "TransformMatrix.h"
#include "Texture.h"
#include <vector>

namespace PROJECT {
	class InputKey {
	public:
		enum class Keyboard {
			None, Enter, Escape, Space, Left, Up, Right, Down,
			A, B, C, D, E, F, G, H, I, J,
			K, L, M, N, O, P, Q, R, S, T,
			U, V, W, X, Y, Z, Tilde, Ctrl,
			Count
		};

		InputKey(Keyboard button);
		bool IsKeyDown() const;
		bool IsKeyReleased() const;
		bool IsKeyPressed() const;
	private:
		Keyboard button;
	};

	class InputMouse {
	public:
		enum class MouseButton {
			None, Left, Right, Center, Count
		};

		InputMouse(MouseButton button);
		bool IsMouseDown() const;
		bool IsMouseReleased() const;
		bool IsMousePressed() const;
	private:
		MouseButton button;
	};

	class Input {
	public:
		Input();
		bool IsKeyDown(InputKey::Keyboard key) const;
		bool IsKeyReleased(InputKey::Keyboard key) const;
		bool IsKeyPressed(InputKey::Keyboard key) const;
		void SetKeyDown(InputKey::Keyboard key, bool value);

		bool IsMouseDown(InputMouse::MouseButton button) const;
		bool IsMouseReleased(InputMouse::MouseButton button) const;
		bool IsMousePressed(InputMouse::MouseButton button) const;
		void SetMouseDown(InputMouse::MouseButton button, bool value);
		
		void SetMousePosition(math::ivec2 newPosition);
		void SetPrevMousePosition(math::ivec2 newPosition);
		math::ivec2 GetMousePosition() const;
		math::ivec2 GetPrevMousePosition() const;
		math::ivec2 GetMouseDirection() const;

		void Update();

	private:
		std::vector<bool> keyDown;
		std::vector<bool> wasKeyDown;

		std::vector<bool> buttonDown;
		std::vector<bool> wasButtonDown;
		
		math::ivec2 mousePosition;
		math::ivec2 previouseMousePosition;
	};
}