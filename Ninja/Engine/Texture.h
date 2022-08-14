/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/11/2021
-----------------------------------------------------------------*/
#pragma once
#include <doodle/image.hpp>	//doodle::Image
#include "Vec2.h"

namespace math {
	class TransformMatrix;
}

namespace PROJECT {
	class Texture {
		friend class TextureManager;
		friend class SpriteFont;
	public:
		Texture() {}
		void Draw(math::TransformMatrix displayMatrix);
		void Draw(math::TransformMatrix displayMatrix, int red, int green, int blue);
		void Draw(math::TransformMatrix displayMatrix, math::ivec2 texelPos, math::ivec2 frameSize);
		void Draw(math::ivec2 pos, int width, int height, int alpha = 255);
		void Draw(math::ivec2 pos, int alpha = 255);
		math::ivec2 GetSize();
		unsigned int GetPixel(math::ivec2 pos);
	private:
		Texture(const std::filesystem::path& filePath);
		Texture(doodle::Image&& doodleImage);
		doodle::Image image;
	};
}
