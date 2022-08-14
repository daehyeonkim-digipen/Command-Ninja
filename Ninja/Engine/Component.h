/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Component.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/16/2021
-----------------------------------------------------------------*/

#pragma once

namespace PROJECT {
    class Component {
    public:
        virtual ~Component() {};
        virtual void Update(double) {};
    };
}