/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/11/2021
-----------------------------------------------------------------*/
#include "Engine.h"
#include "TransformMatrix.h"
#include "Camera.h"

PROJECT::Camera::Camera(math::rect2 movableRange) : movableRange(movableRange), position({ 0, 0 }) {}

void PROJECT::Camera::SetPosition(math::vec2 newPosition) {
	position = newPosition;
}

const math::vec2 &PROJECT::Camera::GetPosition() const {
	return position;
}

void PROJECT::Camera::SetExtent(math::irect2 newExtent) {
	extent = newExtent;
}

void PROJECT::Camera::Update(const math::vec2& followObjPos) {
	if (followObjPos.x > movableRange.Right() + position.x) {
		position.x = followObjPos.x - movableRange.Right();
	}
	if (followObjPos.x - position.x < movableRange.Left()) {
		position.x = followObjPos.x - movableRange.Left();
	}
	if (followObjPos.y > position.y + movableRange.Top()) {
		position.y = followObjPos.y - movableRange.Top();
	}
	if (followObjPos.y - position.y < movableRange.Bottom()) {
		position.y = followObjPos.y - movableRange.Bottom();
	}

	if (position.x < extent.Left()) {
		position.x = extent.Left();
	}
	if (position.x > extent.Right()) {
		position.x = extent.Right();
	}
	if (position.y < extent.Bottom()) {
		position.y = extent.Bottom();
	}
	if (position.y > extent.Top()) {
		position.y = extent.Top();
	}
}

math::TransformMatrix PROJECT::Camera::GetMatrix() {
	return math::TranslateMatrix(-position);
}