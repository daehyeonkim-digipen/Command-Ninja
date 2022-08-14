/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Rect.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/12/2021
-----------------------------------------------------------------*/
#pragma once
#include <utility>
#include "Vec2.h"

namespace math {
    struct [[nodiscard]] rect2 {
        vec2 point1{ 0.0, 0.0 };
        vec2 point2{ 0.0, 0.0 };

        constexpr vec2 Size() const noexcept { return { Right() - Left(), Top() - Bottom() }; }
        constexpr double Left() const noexcept { return std::min(point1.x, point2.x); }     // if the rect was flipped, then bottomLeft.x > topRight.x
        constexpr double Right() const noexcept { return std::max(point1.x, point2.x); }    // if the rect was flipped, then bottomLeft.x > topRight.x
        constexpr double Top() const noexcept { return std::max(point1.y, point2.y); }   // if the rect uses top left as {0, 0} or was flipped in the y direction
        constexpr double Bottom() const noexcept { return std::min(point1.y, point2.y); }   // if the rect uses top left as {0, 0} or was flipped in the y direction
    };
    struct [[nodiscard]] irect2 {
        ivec2 point1{ 0, 0 };
        ivec2 point2{ 0, 0 };

        constexpr ivec2 Size() const noexcept { return { Right() - Left(), Top() - Bottom() }; }
        constexpr int Left() const noexcept { return std::min(point1.x, point2.x); }     // if the rect was flipped, then bottomLeft.x > topRight.x
        constexpr int Right() const noexcept { return std::max(point1.x, point2.x); }    // if the rect was flipped, then bottomLeft.x > topRight.x
        constexpr int Top() const noexcept { return std::max(point1.y, point2.y); }   // if the rect uses top left as {0, 0} or was flipped in the y direction
        constexpr int Bottom() const noexcept { return std::min(point1.y, point2.y); }   // if the rect uses top left as {0, 0} or was flipped in the y direction
    };
}
