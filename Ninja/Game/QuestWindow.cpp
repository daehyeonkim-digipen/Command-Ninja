
#include "../Engine/Engine.h"

#include "QuestWindow.h"
#include "Player.h"
#include "Fonts.h"


QuestWindow::QuestWindow(PROJECT::InputKey openKey_, PROJECT::GameObject* objectT) 
	: GameWindow({ 300, 100 }, UIType::GameWindow, "assets/UI/board.png", openKey_, objectT)
{
}

void QuestWindow::Update(double dt) {
	GameWindow::Update(dt);
	
	Player* playerPtr = static_cast<Player*>(object);
	std::string text;
	if (playerPtr->quest.IsAvailable() == false) {
		text = "no quest";
	}
	else {
		text = playerPtr->quest.GetTargetName() + " : " + std::to_string(playerPtr->quest.currEnemyNumber) + " / " + std::to_string(playerPtr->quest.goalNumber);
	}

	questFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(text, 0xFFFFFFFF, true);
}

void QuestWindow::Draw() {
	GameWindow::Draw();
	questFont.Draw(math::TranslateMatrix{ GetPosition() + GetTexture(0)->GetSize() / 2.0 - questFont.GetSize() * 0.7 / 2.0 } *math::ScaleMatrix{ math::vec2{ 0.7, 0.7 } });
}

