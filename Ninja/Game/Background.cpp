/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Background.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/11/2021
-----------------------------------------------------------------*/
#include "../Engine/Engine.h"
#include "../Engine/Camera.h"
#include "../Engine/Texture.h"
#include "../Engine/TransformMatrix.h"
#include "Background.h"

void Background::Add(const std::filesystem::path& texturePath, int level) {
    backgrounds.push_back({ Engine::GetTextureManager().Load(texturePath), level });
}

math::ivec2 Background::Size() {
    for (ParallaxInfo& levelInfo : backgrounds) {
        if (levelInfo.level == 1) {
            return levelInfo.texturePtr->GetSize();
        }
    }
    return { 0,0 };
}

void Background::Unload() {
    backgrounds.clear();
}

void Background::Draw() {
    PROJECT::Camera* cameraPtr = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>();
    for (ParallaxInfo& levelInfo : backgrounds) {
        if (levelInfo.level == 0) {
            levelInfo.texturePtr->Draw(math::TranslateMatrix(-math::vec2{ 0, 0}));
        }
        levelInfo.texturePtr->Draw(math::TranslateMatrix(-math::vec2{cameraPtr->GetPosition().x / levelInfo.level, cameraPtr->GetPosition().y / levelInfo.level })
            * math::TranslateMatrix( -math::vec2{1440,50} ));
    }
}