#pragma once
#include "..\Engine\GameState.h"
#include "..\Engine\Input.h"
#include "Background.h"
#include "Player.h"
#include "MainMenu.h"

class Floor;
class Player;
class Map : public PROJECT::GameState {
public:

	Map();
	~Map();
	void Load() override = 0;
	void Update(double dt) override;
	void Unload() override = 0;
	void Draw() override;
	void read_floor();
	static void load_Map();
	std::string GetName() override { return "Map"; };
	
protected:
	void DrawDeadScene();

	static constexpr double gravity = 2180;
	Player* playerPtr;

	PROJECT::InputKey levelReload = PROJECT::InputKey::Keyboard::R;
	PROJECT::InputKey mainMenu = PROJECT::InputKey::Keyboard::Escape;
	PROJECT::Texture MapNameFont;

	Background background;
private:
	friend class MainMenu;

	PROJECT::Texture* black_bg;
	PROJECT::Texture* U_DIED;

	double deadTime;

	std::map<int, PROJECT::Texture*> floors;
};

