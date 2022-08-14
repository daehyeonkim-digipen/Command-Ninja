#pragma once
#include "UI.h"

class Bar : public UI {
public:
	Bar(math::vec2 pos, UIType layer, std::string filepath_empty, PROJECT::GameObject* objectT, double isValueFixed_);
	// ~Bar();

	void Update(double dt) override;
	void Draw() override;

	bool IsMouseOn() override;
protected:

	double currValue;
	double maxValue;
	double smoothValue;

	double isValueFixed;
	static constexpr int tipPixelSize = 4;

	//PROJECT::Texture infoFont;
};

class HPBar : public Bar {
public:
	HPBar(math::vec2 pos, UIType priority, std::string filepath_empty, PROJECT::GameObject* objectT, double isValueFixed = 0)
		: Bar(pos, priority, filepath_empty, objectT, isValueFixed)
	{};
	
	void Update(double dt) override {
		maxValue = object->attributes.maxHp;
		currValue = object->attributes.Hp;
		Bar::Update(dt);
	};
};

class StaminaBar : public Bar {
public:
	StaminaBar(math::vec2 pos, UIType priority, std::string filepath_empty, PROJECT::GameObject* objectT, double isValueFixed = 0)
		: Bar(pos, priority, filepath_empty, objectT, isValueFixed)
	{};

	void Update(double dt) override {
		maxValue = object->attributes.maxStamina;
		currValue = object->attributes.Stamina;
		Bar::Update(dt);
	};;
};