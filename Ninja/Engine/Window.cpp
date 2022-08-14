/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Window.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include <doodle/window.hpp>
#include <doodle/drawing.hpp>     // set_frame_of_reference, set_image_mode, clear_background
#include "Engine.h"
#include "Window.h"

void PROJECT::Window::Init(std::string windowName) {
    doodle::create_window(windowName, 1440, 810);
    doodle::set_frame_of_reference(doodle::FrameOfReference::RightHanded_OriginBottomLeft);
}

void PROJECT::Window::Resize(int DEBUG_NEWWidth, int DEBUG_NEWHeight) {
    windowSize.x = DEBUG_NEWWidth;
    windowSize.y = DEBUG_NEWHeight;
}

math::ivec2 PROJECT::Window::GetSize() {
    return windowSize;
}

void PROJECT::Window::Clear(unsigned int color) {
    doodle::clear_background(doodle::HexColor{ color });
}

void PROJECT::Window::Update() {
    doodle::update_window();
}

void on_window_resized(int DEBUG_NEW_width, int DEBUG_NEW_height) {
    Engine::GetLogger().LogEvent("Window Resized");
    Engine::GetWindow().Resize(DEBUG_NEW_width, DEBUG_NEW_height);
}