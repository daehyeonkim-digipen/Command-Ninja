/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: TextureManager.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/19/2021
-----------------------------------------------------------------*/
#include "Engine.h"
#include "Texture.h"
#include "TextureManager.h"


PROJECT::Texture* PROJECT::TextureManager::Load(const std::filesystem::path& filePath) {
    if (pathToTexture.find(filePath) == pathToTexture.end()) {
        pathToTexture[filePath] = new Texture(filePath);
    }
    return pathToTexture[filePath];
}

PROJECT::TextureManager::~TextureManager() {
    Engine::GetLogger().LogEvent("Clear Textures");
    for (std::pair<std::filesystem::path, Texture*> pathTexturePair : pathToTexture) {
        delete pathTexturePair.second;
    }
    pathToTexture.clear();
}

void PROJECT::TextureManager::removeSpriteTexture(Texture* SpriteTexture)
{
    std::filesystem::path target;
	for(auto textures : pathToTexture)
	{
		if(textures.second == SpriteTexture)
		{
            delete SpriteTexture;
            SpriteTexture = nullptr;
            target = textures.first;
		}
	}
    pathToTexture.erase(target);
}
