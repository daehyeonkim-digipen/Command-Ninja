#pragma once
#include "..\Engine\GameState.h"
#include "Map.h"

class Field3 : public Map {
public:

	Field3();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;

	std::string GetName() override { return "Field 3"; }
};

