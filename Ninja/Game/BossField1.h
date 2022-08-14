#pragma once
#include "../Engine/GameState.h"
#include "Map.h"

namespace PROJECT { class GameObjectManager; }

class Boss1;
class BossField1 : public Map {
public:

	BossField1();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;

	std::string GetName() override { return "Boss Field"; }
private:
};

