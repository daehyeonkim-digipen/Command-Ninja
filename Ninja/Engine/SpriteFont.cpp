/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SpriteFont.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/15/2021
-----------------------------------------------------------------*/

#include "doodle/drawing.hpp"	//set_tint_color

#include "Engine.h"
#include "SpriteFont.h"
#include "TransformMatrix.h"

PROJECT::SpriteFont::SpriteFont(const std::filesystem::path& fileName) : texture(fileName) {
	unsigned int white = 0xFFFFFFFF;

	// The top right pixel MUST be white (0xFFFFFFFF)
	unsigned int color = texture.GetPixel({ 0, 0 });
	if (color != white) {
		Engine::GetLogger().LogError("Sprite font " + fileName.string() + " not setup correctly");
		return;
	}

	// Build our rects
	SetupCharRects();
}

void PROJECT::SpriteFont::SetupCharRects() {
	unsigned int testColor = texture.GetPixel({ 0, 0 });
	unsigned int nextColor;

	int height = texture.GetSize().y;

	int xPos = 0;
	testColor = texture.GetPixel({ xPos, 0 });
	for (int index = 0; index < SpriteFont::numOfChars; index++) {
		int width = 0;
		// Todo count the pixels until the nextColor is not the same as the current color (testColor)
		do {
			width++;
			nextColor = texture.GetPixel({ xPos + width, 0 });
		} while (testColor == nextColor);

		// Test color becomes our next color, so this we be the color we are using to count
		testColor = nextColor;

		// Set the bottomLeft and topRight for each of our rectangles
		charTexels[index].point2 = { xPos + width - 1, 1 };
		charTexels[index].point1 = { xPos, charTexels[index].point2.y + height - 1 };
		xPos += width;
	}
}

math::irect2& PROJECT::SpriteFont::GetCharRect(char c) {
	if (c >= ' ' && c <= 'z') {
		return charTexels[c - ' '];
	} else {
		Engine::GetLogger().LogError("character '" + std::to_string(c) + "' not found in spriteFont");
		return charTexels[0];
	}
}

math::ivec2 PROJECT::SpriteFont::MeasureText(std::string text) {
	math::ivec2 size = { 0,0 };	
	// Todo: For each character use it's charTexel information to find the width/height of the string
	for (char c : text) {
		math::irect2 charRect = GetCharRect(c);
		size.x += GetCharRect(c).Size().x;
		if (charRect.Size().y > size.y) {
			size.y = charRect.Size().y;
		}
	}
	return size;
}

PROJECT::Texture PROJECT::SpriteFont::DrawTextToTexture(std::string text_, unsigned int color, bool drawShadow) {

	math::ivec2 textSize = MeasureText(text_);
	if (drawShadow == true) {
		textSize += math::ivec2{3, 3};
	}
	doodle::begin_drawing_to_image(textSize.x, textSize.y, true); //this starts our rendering to the texture

	math::TransformMatrix matrix;
	matrix.Reset();
	if (drawShadow == true) {
		// draw in black using the shadow position
		doodle::set_tint_color(doodle::HexColor(0x000000FF));
		for (char c : text_) {
			DrawChar(matrix, c);
		}
		matrix = math::TranslateMatrix(math::ivec2{ 3,3 });
	}

	// draw in white using the shadow position
	doodle::set_tint_color(doodle::HexColor(color));
	for (char c : text_) {
		DrawChar(matrix, c);
	}
	// Reset the tint color incase it changed
	doodle::set_tint_color(doodle::HexColor(0xFFFFFFFF));
	return PROJECT::Texture(doodle::end_drawing_to_image());
}

void PROJECT::SpriteFont::DrawChar(math::TransformMatrix& matrix, char c) {
	math::irect2& displayRect = GetCharRect(c);
	math::ivec2 topLeftTexel = { displayRect.point1.x, displayRect.point2.y };
	if (c != ' ') {
		texture.Draw(matrix, topLeftTexel, displayRect.Size());
	}
	matrix *= math::TranslateMatrix(math::ivec2{ displayRect.Size().x, 0 });
}