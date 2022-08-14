#include "../Engine/Engine.h"
#include "../Engine/Input.h"
#include "../Engine/SkillManager.h"

#include "Fonts.h"
#include "GameSkills.h"
#include "SkillWindow.h"

SkillWindow::SkillWindow(PROJECT::InputKey openKey_, PROJECT::GameObject* objectT)
	: GameWindow({ 300, 100 }, UIType::GameWindow, "assets/UI/board.png", openKey_, objectT)
{
	windowOrder.push_back(WindowPageType::MAIN);

	pageButtons.push_back(Button({ GetPosition(), "assets/UI/Buttons/atk_button.png" }));
	pageButtons.push_back(Button({ GetPosition(), "assets/UI/Buttons/dex_button.png" }));
	pageButtons.push_back(Button({ GetPosition(), "assets/UI/Buttons/def_button.png"}));	

	backButton = Button({ GetPosition(), "assets/UI/Buttons/back_button.png" });
}

void SkillWindow::Update(double dt) {
	GameWindow::Update(dt);

	if (openKey.IsKeyPressed() == true && IsVisible() == false) {
		windowOrder.clear();
		windowOrder.push_back(WindowPageType::MAIN);
	}

	if (IsVisible() == true) {
		if (windowOrder.back() == WindowPageType::MAIN) {
			if (pageButtons[0].DoesClicked() == true) {
				windowOrder.push_back(WindowPageType::ATK);
			}
			else if (pageButtons[1].DoesClicked() == true) {
				windowOrder.push_back(WindowPageType::DEX);
			}
			else if (pageButtons[2].DoesClicked() == true) {
				windowOrder.push_back(WindowPageType::DEF);
			}
		}
		else {
			if (backButton.DoesClicked() == true) {
				windowOrder.pop_back();
			}
		}
	}

	for (int i = 0; i < static_cast<int>(pageButtons.size()); i++)
		pageButtons[i].SetPosition(GetPosition() + math::vec2{ 200.0 * i + 40 , 75 });

	backButton.SetPosition(GetPosition() + math::vec2{ 260, 50 });

	if (Engine::GetInput().IsMousePressed(PROJECT::InputMouse::MouseButton::Right))
		Engine::GetLogger().LogDebug(std::to_string(windowOrder.size()));

}

void SkillWindow::Draw() {
	GameWindow::Draw();

	// Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->DrawIcons(math::TranslateMatrix{ GetPosition() + math::vec2{ 50, 250 } });

	auto icon = [&](const std::string& command)-> PROJECT::Texture* {
		return Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->GetIcon(command);
	};

	const std::vector<std::map<std::string, bool>>& cSkills = Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->cSkillsInfo;

	__int8 i = 0;
	PROJECT::Texture skillInfoFont;
	switch (windowOrder.back()) 
	{
	case WindowPageType::ATK: [[fallthrough]];
	case WindowPageType::DEX: [[fallthrough]];
	case WindowPageType::DEF:
		for (std::pair<const std::string, bool> pair : cSkills[static_cast<int>(windowOrder.back())]) {
			math::TransformMatrix offsetMatrix = math::TranslateMatrix{ GetPosition() + math::vec2{ 50.0 + (double)i * (double)(SkillManager::SkillIconSize + 10), 200.0 } };
			pair.second ? icon(pair.first)->Draw(offsetMatrix) : icon(pair.first)->Draw(offsetMatrix, 100, 100, 100);

			math::vec2 mouse = Engine::GetInput().GetMousePosition().operator math::vec2();
			math::vec2 offsetVector = offsetMatrix * math::vec2{ 1, 1 };
			if (offsetVector.x <= mouse.x && mouse.x <= offsetVector.x + icon(pair.first)->GetSize().x &&
				offsetVector.y <= mouse.y && mouse.y <= offsetVector.y + icon(pair.first)->GetSize().y && Engine::GetInput().IsMousePressed(PROJECT::InputMouse::MouseButton::Left)) {
				windowOrder.push_back(WindowPageType::SkillInfo);
				selectedSkillCommand = pair.first;
			}

			i++;
		}

		backButton.Draw();
		break;
	case WindowPageType::SkillInfo:
		skillInfoFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture(
			Engine::GetGameStateManager().GetGSMComponents().GetComponent<SkillManager>()->GetSkillInfo(selectedSkillCommand), 0xFFFFFFFF, true);

		skillInfoFont.Draw(math::TranslateMatrix{ GetPosition() + math::vec2{ 200, 250 } } *math::ScaleMatrix{ math::vec2{ 0.4, 0.4 } });
		icon(selectedSkillCommand)->Draw(math::TranslateMatrix{ GetPosition() + math::vec2{ 100, 200 } });
		backButton.Draw();
		break;
	case WindowPageType::MAIN:
		for (Button& b : pageButtons)
			b.Draw();
		break;
	default:
		break;
	}
}
