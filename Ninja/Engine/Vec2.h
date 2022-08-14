/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Vec2.h
Project: PROJECT
Author: Rudy Castan
Creation date:
-----------------------------------------------------------------*/
#pragma once
#include <limits>

namespace math {
    [[nodiscard]] constexpr double abs(double d) noexcept { return (d < 0.0) ? -d : d; }
    [[nodiscard]] constexpr bool   is_equal(double d1, double d2) noexcept {
        return abs(d1 - d2) <= std::numeric_limits<double>::epsilon() * abs(d1 + d2);
    }
    [[nodiscard]] constexpr bool   is_equal(int i1, int i2) noexcept {
        return i1 == i2;
    }

    struct [[nodiscard]] vec2 {
        double x{ 0.0 };
        double y{ 0.0 };


        constexpr vec2() noexcept = default;
        constexpr vec2(double x_value, double y_value) noexcept : x{ x_value }, y{ y_value } {};

        [[nodiscard]] constexpr bool operator==(const vec2& v) const noexcept;
        [[nodiscard]] constexpr bool operator!=(const vec2& v) const noexcept;

        constexpr vec2  operator+(const vec2& v) const noexcept;
        constexpr vec2& operator+=(const vec2& v) noexcept;

        constexpr vec2  operator-(const vec2& v) const noexcept;
        constexpr vec2& operator-=(const vec2& v) noexcept;

        constexpr vec2 operator-() const noexcept;

        constexpr vec2  operator*(double scale) const noexcept;
        constexpr vec2  operator/(double divisor) const noexcept;
        constexpr vec2& operator*=(double scale) noexcept;
        constexpr vec2& operator/=(double divisor) noexcept;
        
        constexpr vec2 operator*(vec2 scale) noexcept;
        constexpr vec2& operator*=(vec2 scale) noexcept;

        vec2 Normalize() const noexcept;
        constexpr double Cross(const vec2& v) const noexcept;
    };

    constexpr vec2 operator*(double scale, const vec2& v) noexcept;

    struct [[nodiscard]] ivec2 {
        int x{ 0 };
        int y{ 0 };


        constexpr ivec2() noexcept = default;
        constexpr ivec2(int x_value, int y_value) noexcept : x{ x_value }, y{ y_value } {};
        explicit constexpr operator vec2() { return vec2{ static_cast<double>(x),static_cast<double>(y) }; }

        [[nodiscard]] constexpr bool operator==(const ivec2& v) const noexcept;
        [[nodiscard]] constexpr bool operator!=(const ivec2& v) const noexcept;

        constexpr ivec2  operator+(const ivec2& v) const noexcept;
        constexpr ivec2& operator+=(const ivec2& v) noexcept;

        constexpr ivec2  operator-(const ivec2& v) const noexcept;
        constexpr ivec2& operator-=(const ivec2& v) noexcept;

        constexpr ivec2 operator-() const noexcept;

        constexpr ivec2  operator*(int scale) const noexcept;
        constexpr ivec2  operator/(int divisor) const noexcept;
        constexpr ivec2& operator*=(int scale) noexcept;
        constexpr ivec2& operator/=(int divisor) noexcept;

        constexpr vec2  operator*(double scale) const noexcept;
        constexpr vec2  operator/(double divisor) const noexcept;
        constexpr vec2& operator*=(double scale) noexcept;
        constexpr vec2& operator/=(double divisor) noexcept;
    };
}

#include "Vec2.inl"
