#pragma once

#include "../Engine/Input.h"
#include "../Engine/Component.h"
#include "../Engine/Texture.h"
#include "../Engine/GameObject.h"

//#include "Button.h"

namespace PROJECT {
	class UIManager;
}

enum class UIType {
	BossBar,
	EnemyBar,
	PlayerBar,
	PopUpDamage,
	GameWindow,
	None
};

class UI
{
	friend class PROJECT::UIManager;
	using GameMouse = PROJECT::InputMouse::MouseButton;
public:
	UI(PROJECT::GameObject* objectT, UIType layer);
	~UI();

	virtual void Update(double) {};
	virtual void Draw();

	int GetLevel() { return layerLevel; }
	bool IsVisible() { return isVisible; }
	void SetVisible(bool value) { isVisible = value; };
	void UIDestroy() { shouldDestroyed = true; }

	void SetPosition(math::vec2 newPos) { position = newPos; };
	const math::vec2& GetPosition() { return position; };

	virtual bool IsMouseOn();
protected:
	void AddTexture(const std::filesystem::path& textureFilePath);
	PROJECT::Texture* GetTexture(int index = 0);
	//PROJECT::GameObject* const GetObject();

	PROJECT::GameObject* object;
	bool shouldDestroyed;
private:
	bool isVisible;
	int layerLevel;

	math::vec2 position;


	std::vector<PROJECT::Texture*> textures;
};

class GameWindow : public UI {
	using GameKey = PROJECT::InputKey::Keyboard;
	using GameMouse = PROJECT::InputMouse::MouseButton;
public:
	GameWindow(math::vec2 pos, UIType priority, const std::filesystem::path& textureFilePath, PROJECT::InputKey openButton, PROJECT::GameObject* objectT = nullptr);

	void Update(double dt) override;
	bool IsMouseOnDragableArea();
protected:
	void UpdateDragging(double dt);

	bool isfollowingMouse;
	PROJECT::InputKey openKey;
};