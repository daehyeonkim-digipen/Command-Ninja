#pragma once
#include "..\Engine\GameState.h"
#include "Map.h"

class Field2 : public Map {
public:

	Field2();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;

	std::string GetName() override { return "Field 2"; }
};

