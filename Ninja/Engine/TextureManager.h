/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: TextureManager.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/19/2021
-----------------------------------------------------------------*/
#pragma once
#include <filesystem>
#include <map>

namespace PROJECT {
    class Texture;

    class TextureManager {
    public:
        Texture* Load(const std::filesystem::path& filePath);
        ~TextureManager();
        void removeSpriteTexture(Texture*);
    private:
        std::map<std::filesystem::path, Texture*> pathToTexture;
    };
}