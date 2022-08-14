#pragma once
#include "UI.h"

class QuestWindow : public GameWindow {
public:
	QuestWindow(PROJECT::InputKey openKey, PROJECT::GameObject* objectT);

	void Update(double dt) override;
	void Draw() override;

private:
	PROJECT::Texture questFont;
};

