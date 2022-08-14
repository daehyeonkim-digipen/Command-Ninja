/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Collision.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/12/2021
-----------------------------------------------------------------*/

#include "doodle/drawing.hpp"
#include "Engine.h"

#include "TransformMatrix.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Rect.h"
#include "Collision.h"


void PROJECT::RectCollision::Draw(math::TransformMatrix cameraMatrix) {
    doodle::push_settings();
    doodle::no_fill();
    doodle::set_outline_width(2);
    doodle::set_outline_color(doodle::HexColor(0xFFFFFFFF));
    math::rect2 displayRect = GetWorldCoorRect();
    doodle::apply_matrix(cameraMatrix[0][0], cameraMatrix[1][0], cameraMatrix[0][1], cameraMatrix[1][1], cameraMatrix[0][2], cameraMatrix[1][2]);
    doodle::draw_rectangle(displayRect.Left(), displayRect.Bottom(), displayRect.Size().x, displayRect.Size().y);
    doodle::pop_settings();
}

void PROJECT::CircleCollision::Draw(math::TransformMatrix cameraMatrix) {
    doodle::push_settings();
    doodle::no_fill();
    doodle::set_outline_width(2);
    doodle::set_outline_color(doodle::HexColor(0xFFFFFFFF));
    doodle::apply_matrix(cameraMatrix[0][0], cameraMatrix[1][0], cameraMatrix[0][1], cameraMatrix[1][1], cameraMatrix[0][2], cameraMatrix[1][2]);
    doodle::draw_ellipse(objectPtr->GetPosition().x, objectPtr->GetPosition().y, GetRadius() * 2);
    doodle::pop_settings();
}

math::rect2 PROJECT::RectCollision::GetWorldCoorRect() {
    return { objectPtr->GetMatrix() * static_cast<math::vec2>(rect.point1),
                objectPtr->GetMatrix() * static_cast<math::vec2>(rect.point2) };
}

bool PROJECT::RectCollision::DoesCollideWith(GameObject* objectB) {
    Collision* collideAgainst = objectB->GetGOComponent<Collision>();
    if (collideAgainst == nullptr) {
        Engine::GetLogger().LogError("testAgainstObject collision is null");
        return false;
    }
    if (collideAgainst->GetCollideType() != CollideType::Rect_Collide) {
        Engine::GetLogger().LogError("Rect vs unsupported type");
        return false;
    }

    math::rect2 rectA = GetWorldCoorRect();
    math::rect2 rectB = dynamic_cast<RectCollision*>(collideAgainst)->GetWorldCoorRect();

    if (rectA.Right() > rectB.Left() && rectA.Left() < rectB.Right() &&
        rectA.Bottom() < rectB.Top() && rectA.Top() > rectB.Bottom()) {
        return true;
    }
    return false;
}

bool PROJECT::RectCollision::DoesCollideWith(math::vec2 point) {
    math::rect2 worldRect = GetWorldCoorRect();
    return point.x >= worldRect.Left() && point.x <= worldRect.Right() && point.y <= worldRect.Top() && point.y >= worldRect.Bottom();
}

double PROJECT::CircleCollision::GetRadius() {
    return (math::ScaleMatrix(objectPtr->GetScale()) * math::vec2{ radius, 0 }).x;
}

bool PROJECT::CircleCollision::DoesCollideWith(GameObject* objectB) {
    Collision* collideAgainst = objectB->GetGOComponent<Collision>();
    if (collideAgainst == nullptr) {
        Engine::GetLogger().LogError("testAgainstObject collision is null");
        return false;
    }
    if (collideAgainst->GetCollideType() != CollideType::Circle_Collide) {
        Engine::GetLogger().LogError("Circle vs unsupported type");
        return false;
    }

    math::vec2 dist = objectPtr->GetPosition() - objectB->GetPosition();
    double radiusA = GetRadius();
    double radiusB = dynamic_cast<CircleCollision*>(collideAgainst)->GetRadius();
    double distSquare = dist.x * dist.x + dist.y * dist.y;
    double collideDistSquare = (radiusA + radiusB) * (radiusA + radiusB);

    return distSquare < collideDistSquare;
}

bool PROJECT::CircleCollision::DoesCollideWith(math::vec2 point) {

    math::vec2 dist = objectPtr->GetPosition() - point;
    double distSquare = dist.x * dist.x + dist.y * dist.y;
    double radiusA = GetRadius();
    double collideDistSquare = radiusA * radiusA;

    return distSquare < collideDistSquare;
}