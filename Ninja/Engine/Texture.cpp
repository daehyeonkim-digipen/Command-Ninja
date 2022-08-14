/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/11/2021
-----------------------------------------------------------------*/
#include <doodle/drawing.hpp>	//draw_image
#include "Texture.h"
#include "TransformMatrix.h"
#include "MemManager.h"

PROJECT::Texture::Texture(const std::filesystem::path& filePath) {
	image = doodle::Image{ filePath };
}
PROJECT::Texture::Texture(doodle::Image&& doodleImage) {
	image = std::move(doodleImage);
}

void PROJECT::Texture::Draw(math::TransformMatrix displayMatrix, math::ivec2 texelPos, math::ivec2 frameSize) {
	doodle::push_settings();
	doodle::apply_matrix(displayMatrix[0][0], displayMatrix[1][0], displayMatrix[0][1], displayMatrix[1][1], displayMatrix[0][2], displayMatrix[1][2]);
	doodle::draw_image(image, 0, 0, static_cast<double>(frameSize.x), static_cast<double>(frameSize.y), texelPos.x, texelPos.y);
	doodle::pop_settings();
}

void PROJECT::Texture::Draw(math::TransformMatrix displayMatrix) {
	doodle::push_settings();
	doodle::apply_matrix(displayMatrix[0][0], displayMatrix[1][0], displayMatrix[0][1], displayMatrix[1][1], displayMatrix[0][2], displayMatrix[1][2]);
	doodle::draw_image(image, 0, 0);
	doodle::pop_settings();
}

void PROJECT::Texture::Draw(math::TransformMatrix displayMatrix, int red, int green, int blue) {
	doodle::push_settings();
	doodle::set_tint_color(red, green, blue);
	doodle::apply_matrix(displayMatrix[0][0], displayMatrix[1][0], displayMatrix[0][1], displayMatrix[1][1], displayMatrix[0][2], displayMatrix[1][2]);
	doodle::draw_image(image, 0, 0);
	doodle::pop_settings();
}

void PROJECT::Texture::Draw(math::ivec2 pos, int width, int height, int alpha) {
	doodle::push_settings();
	doodle::set_tint_color(255, alpha);
	doodle::draw_image(image, pos.x, pos.y, width, height);
	doodle::pop_settings();
}

void PROJECT::Texture::Draw(math::ivec2 pos, int alpha) {
	Texture::Draw(pos, image.GetWidth(), image.GetHeight(), alpha);
}

math::ivec2 PROJECT::Texture::GetSize() { return { image.GetWidth(), image.GetHeight() }; }

unsigned int PROJECT::Texture::GetPixel(math::ivec2 pos) {
	int index = pos.y * GetSize().x + pos.x;
	return (static_cast<int>(image[index].red)) << 24 |
		(static_cast<int>(image[index].green)) << 16 |
		(static_cast<int>(image[index].blue)) << 8 |
		(static_cast<int>(image[index].alpha));
}