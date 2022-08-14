#pragma once
#include "UI.h"

class PopUpDamage : public UI
{
public:
	PopUpDamage(math::vec2 pos, int damageAmt);

	void Update(double dt) override;
	void Draw() override;

protected:
	PROJECT::Texture damageFont;

private:
	double lifeTime;
	double limitYPosition;
	math::vec2 velocity;
};

class PlayerTakenDamage : public PopUpDamage {
public:
	PlayerTakenDamage(math::vec2 pos, int dmg) 
		: PopUpDamage(pos, dmg) 
	{}

	void Draw() override;
};
