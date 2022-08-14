/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Floor.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/19/2021
-----------------------------------------------------------------*/
#pragma once

#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"

namespace math {
    struct rect2;
}

class Floor : public PROJECT::GameObject {
public:
    Floor(math::irect2, std::map<int, PROJECT::Texture*>);
    std::string GetObjectTypeName() override { return "Floor"; }
    virtual GameObjectType GetObjectType() override { return GameObjectType::Floor; }
    virtual bool CanCollideWith(GameObjectType objectBType) override;
    void Draw(math::TransformMatrix displayMatrix) override;
private:
    std::map<int, PROJECT::Texture*> tiles;
};