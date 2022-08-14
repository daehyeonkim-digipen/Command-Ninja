/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Gravity.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/17/2021
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\Component.h" 

class Gravity : public PROJECT::Component {
public:
    Gravity(double value) : gravity(value) {}
    double GetValue() { return gravity; }
private:
    double gravity;
};