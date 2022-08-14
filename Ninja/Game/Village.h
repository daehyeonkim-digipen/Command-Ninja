#pragma once
#include "..\Engine\GameState.h"
#include "..\Engine\Input.h"
#include "Background.h"
#include "Map.h"

class Village : public Map {
public:

	Village();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;

	std::string GetName() override { return "Village"; }
};

