/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Window.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include "Vec2.h"

namespace PROJECT {
    class Window {
    public:
        void Init(std::string windowName);
        void Resize(int newWidth, int newHeight);
        math::ivec2 GetSize();
        void Clear(unsigned int color);
        void Update();
    private:
        math::ivec2 windowSize;
    };
}
