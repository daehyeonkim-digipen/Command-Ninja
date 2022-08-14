/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ShowCollision.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 4/29/2021
-----------------------------------------------------------------*/
#include "ShowCollision.h"

ShowCollision::ShowCollision(PROJECT::InputKey::Keyboard keyToUse) : enabled(false), showCollisionKey(keyToUse) {}

void ShowCollision::Update(double) {
	if (showCollisionKey.IsKeyReleased() == true) {
		enabled = !enabled;
	}
}

bool ShowCollision::IsEnabled() {
	return enabled;
}