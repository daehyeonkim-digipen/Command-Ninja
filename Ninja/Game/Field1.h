#pragma once
#include "..\Engine\GameState.h"
#include "Map.h"

class Field1 : public Map {
public:

	Field1();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;

	std::string GetName() override { return "Field 1"; }
};

	