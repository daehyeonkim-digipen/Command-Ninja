/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Background.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/11/2021
-----------------------------------------------------------------*/
#pragma once
#include <filesystem>
#include "../Engine/Component.h"

namespace PROJECT {
    class Camera;
    class Texture;
}

class Background : public PROJECT::Component {
public:
    void Add(const std::filesystem::path &texturePath, int level);
    void Unload();
    void Draw();
    math::ivec2 Size();
private:
    struct ParallaxInfo {
        PROJECT::Texture *texturePtr;
        int level;
    };
    std::vector<ParallaxInfo> backgrounds;
};