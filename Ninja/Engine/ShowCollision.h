/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ShowCollision.h
Project: PROJECT
Author: Kevin Wright
Creation date: 4/29/2021
-----------------------------------------------------------------*/
#pragma once
#include "Input.h"
#include "Component.h"

class ShowCollision : public PROJECT::Component {
public:
    ShowCollision(PROJECT::InputKey::Keyboard keyToUse);
    void Update(double dt) override;
    bool IsEnabled();
private:
    bool enabled;
    PROJECT::InputKey showCollisionKey;
};